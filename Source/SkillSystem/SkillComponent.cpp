#include "SkillComponent.h"
#include "EnhancedInputComponent.h"
#include "Skill.h"
#include "SkillEffect.h"
#include "GameFramework/Pawn.h"
#include "Net/UnrealNetwork.h"

USkillComponent::USkillComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	bReplicateUsingRegisteredSubObjectList = true;
}

AController* USkillComponent::GetOwningController() const
{
	const APawn* OwningPawn = GetOwningPawn();
	return OwningPawn ? OwningPawn->GetController() : nullptr;
}

bool USkillComponent::IsLocallyControlled() const
{
	const AController* OwningController = GetOwningController();
	return OwningController && OwningController->IsLocalController();
}

USkill* USkillComponent::GetSkillOfClass(TSubclassOf<USkill> SkillClass)
{
	for (USkill* Skill : OwnedSkills)
		if (Skill && Skill->GetClass() == SkillClass)
			return Skill;
	
	return nullptr;
}

bool USkillComponent::HasSkill(const USkill* Skill) const
{
	return Skill && OwnedSkills.Contains(Skill);
}

bool USkillComponent::RegisterSkill(USkill* Skill)
{
	if (Skill && !OwnedSkills.Contains(Skill))
	{
		AddReplicatedSubObject(Skill);
		OwnedSkills.Add(Skill);
		return true;
	}
	return false;
}

bool USkillComponent::DeregisterSkill(USkill* Skill)
{
	if (OwnedSkills.Remove(Skill))
	{
		RemoveReplicatedSubObject(Skill);
		return true;
	}
	return false;
}

void USkillComponent::ProcessSkillData(const FSkillData& InData)
{
	USkill* Skill = GetSkillOfClass(InData.SkillClass);
	if (!Skill)
	{
		// If the skill does not exist, create a new one.
		Skill = NewObject<USkill>(this, InData.SkillClass);
		if (!RegisterSkill(Skill)) return;
	}
	Skill->UpdateSkillData(InData);
}

void USkillComponent::ExecuteSkill(USkill* Skill)
{
	if (!HasSkill(Skill)) return;

	if (Skill->CastTime > 0)
	{
		TryCastSkill(Skill);
	}
	else // CastTime <= 0
	{
		TryActivateSkill(Skill);
	}
}

void USkillComponent::TryCastSkill(USkill* Skill)
{
	bool bValidated;
	FString ErrorLog;
	ValidateSkillPreCast(Skill, bValidated, ErrorLog);
	
	if (bValidated)
	{
		CastSkill(Skill);
		Skill->CastSkill();
	}
	else
	{
		OnSkillPreCastValidationError.Broadcast(Skill, ErrorLog);
	}
}

void USkillComponent::TryMaintainSkillCast(USkill* Skill)
{
	bool bValidated;
	FString ErrorLog;
	ValidateSkillMidCast(Skill, bValidated, ErrorLog);
	
	if (!bValidated)
	{
		OnSkillMidCastValidationError.Broadcast(Skill, ErrorLog);
		CancelSkillCast(Skill);
		Skill->CancelSkillCast();
	}
}

void USkillComponent::TryActivateSkill(USkill* Skill)
{
	bool bValidated;
	FString ErrorLog;
	ValidateSkillPreActivation(Skill, bValidated, ErrorLog);
	
	if (bValidated)
	{
		ActivateSkill(Skill);
		Skill->ActivateSkill();
	}
	else
	{
		OnSkillPreActivationValidationError.Broadcast(Skill, ErrorLog);
	}
}

void USkillComponent::ApplySkillEffect(USkillEffect* Effect)
{
	AppliedEffects.Add(Effect);
}

bool USkillComponent::BindSkillToInput(const TSubclassOf<USkill> SkillClass, const FName InputActionName, const EInputEvent InputEvent)
{
	USkill* Skill = GetSkillOfClass(SkillClass);
	const AController* Controller = GetOwningController();
	
	if (!Skill || !Controller || !Controller->InputComponent || !Controller->IsLocalPlayerController())
		return false;

	FInputActionBinding& Binding = Controller->InputComponent->BindAction(InputActionName, InputEvent, Skill, &USkill::RequestOwnerToExecute);
	return true;
}

bool USkillComponent::BindSkillToEnhancedInput(const TSubclassOf<USkill> SkillClass, const UInputAction* InputAction, const ETriggerEvent TriggerEvent)
{
	USkill* Skill = GetSkillOfClass(SkillClass);
	const AController* Controller = GetOwningController();
	
	if (!InputAction || !Skill || !Controller || !Controller->InputComponent || !Controller->IsLocalPlayerController())
		return false;

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(Controller->InputComponent))
	{
		FEnhancedInputActionEventBinding& Binding = EnhancedInputComponent->BindAction(InputAction, TriggerEvent, Skill, &USkill::RequestOwnerToExecute);
		return true;
	}
	return false;
}

TArray<FSkillData> USkillComponent::GetClientSkillData_Implementation()
{
	return ClientSkillData;
}

void USkillComponent::ClientUploadSkillData_Implementation()
{
	ServerDownloadSkillData(GetClientSkillData());
}

void USkillComponent::ServerDownloadSkillData_Implementation(const TArray<FSkillData>& InDataArray)
{
	// Cache the client skill data
	ClientSkillData = InDataArray;
	
	for (int i = 0; i < InDataArray.Num(); ++i)
	{
		ProcessSkillData(InDataArray[i]);
	}
}

FString USkillComponent::GetOwnerName() const
{
	const AActor* Owner = GetOwner();
	return Owner ? Owner->GetName() : "Invalid";
}

FString USkillComponent::GetOwnerNetRoleAsString() const
{
	const UEnum* EnumPtr = FindFirstObjectSafe<UEnum>(TEXT("ENetRole"));
	return EnumPtr ? EnumPtr->GetNameStringByValue(GetOwnerRole()) : "Invalid";
}

void USkillComponent::OnRegister()
{
	Super::OnRegister();
	
	if (GetWorld() && GetWorld()->IsGameWorld() && HasAuthority())
	{
		// Initialize the preset skills
		for (int i = 0; i < PresetSkills.Num(); ++i)
		{
			ProcessSkillData(PresetSkills[i]);
		}
	}
}

void USkillComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USkillComponent, OwnedSkills);
	DOREPLIFETIME(USkillComponent, AppliedEffects);
}

void USkillComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (USkill* Skill : OwnedSkills)
	{
		Skill->Tick(DeltaTime);
	}
	
	for (USkillEffect* Effect : AppliedEffects)
	{
		Effect->NativeTick(DeltaTime);
	}
}

void USkillComponent::ValidateSkillPreCast(USkill* Skill, bool& bValidated, FString& ErrorLog) const
{
	bValidated = HasSkill(Skill) && CanSkillBeCast(Skill, ErrorLog) && Skill->CanSkillBeCast(ErrorLog);
}

void USkillComponent::ValidateSkillMidCast(USkill* Skill, bool& bValidated, FString& ErrorLog) const
{
	bValidated = HasSkill(Skill) && CanSkillBeCast(Skill, ErrorLog) && Skill->CanSkillBeCast(ErrorLog);
}

void USkillComponent::ValidateSkillPreActivation(USkill* Skill, bool& bValidated, FString& ErrorLog) const
{
	bValidated = HasSkill(Skill) && CanSkillBeActivated(Skill, ErrorLog) && Skill->CanSkillBeActivated(ErrorLog);
}
