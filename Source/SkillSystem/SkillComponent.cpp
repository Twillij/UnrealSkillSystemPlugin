#include "SkillComponent.h"
#include "Skill.h"
#include "GameFramework/Pawn.h"
#include "Net/UnrealNetwork.h"

USkillComponent::USkillComponent()
{
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
	for (USkill* Skill : Skills)
		if (Skill && Skill->GetClass() == SkillClass)
			return Skill;
	
	return nullptr;
}

bool USkillComponent::RegisterSkill(USkill* Skill)
{
	if (Skill && !GetSkillOfClass(Skill->GetClass()))
	{
		AddReplicatedSubObject(Skill);
		Skills.Add(Skill);
		Skill->SetOwningComponent(this);
		return true;
	}
	return false;
}

bool USkillComponent::DeregisterSkill(USkill* Skill)
{
	if (Skills.Remove(Skill))
	{
		RemoveReplicatedSubObject(Skill);
		Skill->SetOwningComponent(nullptr);
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
	DOREPLIFETIME(USkillComponent, Skills);
}
