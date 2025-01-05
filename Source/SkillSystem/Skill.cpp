#include "Skill.h"
#include "SkillComponent.h"
#include "SkillEffect.h"
#include "SkillSystem.h"
#include "Net/UnrealNetwork.h"

FSkillData::FSkillData(const USkill* InSkill)
{
	if (!InSkill) return;
	SkillClass = InSkill->GetClass();
	bUnlocked = InSkill->bUnlocked;
	SkillLevel = InSkill->SkillLevel;
	CastTime = InSkill->CastTime;
	Duration = InSkill->Duration;
	Cooldown = InSkill->Cooldown;
}

void USkill::UpdateSkillData_Implementation(const FSkillData& SkillData)
{
	bUnlocked = SkillData.bUnlocked;
	SkillLevel = SkillData.SkillLevel;
	CastTime = SkillData.CastTime;
	Duration = SkillData.Duration;
	Cooldown = SkillData.Cooldown;
}

USkillComponent* USkill::GetOwningComponent() const
{
	return Cast<USkillComponent>(GetOuter());
}

bool USkill::HasAuthority() const
{
	const USkillComponent* OwningComponent = GetOwningComponent();
	return OwningComponent ? OwningComponent->HasAuthority() : false;
}

void USkill::RequestOwnerToExecute()
{
	if (USkillComponent* Owner = GetOwningComponent())
	{
		Owner->ExecuteSkill(this);
	}
}

void USkill::TryCastSkill_Implementation()
{
	FString ValidationLog;
	const bool bValidated = ValidateSkillPreCast(ValidationLog);
	OnReceivePreCastValidation(bValidated, ValidationLog);
	
	if (bValidated)
	{
		StartCastingSkill();
	}
}

void USkill::TryActivateSkill_Implementation()
{
	FString ValidationLog;
	const bool bValidated = ValidateSkillPreActivation(ValidationLog);
	OnReceivePreActivationValidation(bValidated, ValidationLog);

	if (bValidated)
	{
		DurationTimer = Duration;
	}
}

void USkill::DeactivateSkill_Implementation()
{
	CooldownTimer = Cooldown;
}

void USkill::StartCastingSkill_Implementation()
{
	CastTimer = CastTime;
}

void USkill::StopCastingSkill_Implementation()
{
	CastTimer = 0;
}

void USkill::Tick_Implementation(const float DeltaSeconds)
{
	if (CastTimer > 0)
	{
		CastTimer -= DeltaSeconds;
		
		if (CastTimer > 0)
		{
			FString ValidationLog;
			if (!ValidateSkillMidCast(ValidationLog))
			{
				OnReceiveMidCastValidation(false, ValidationLog);
				StopCastingSkill();
			}
		}
		else // CastTimer <= 0
		{
			TryActivateSkill();
		}
	}
	else if (DurationTimer > 0)
	{
		DurationTimer -= DeltaSeconds;
		if (DurationTimer <= 0) DeactivateSkill();
	}
	else if (CooldownTimer > 0)
	{
		CooldownTimer -= DeltaSeconds;
	}
}

void USkill::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USkill, bUnlocked);
	DOREPLIFETIME(USkill, SkillLevel);
	DOREPLIFETIME(USkill, CastTime);
	DOREPLIFETIME(USkill, Duration);
	DOREPLIFETIME(USkill, Cooldown);
}

void USkill::PostInitProperties()
{
	Super::PostInitProperties();
	
	if (GetWorld() && GetWorld()->IsGameWorld())
	{
		BeginPlay();
	}
}

void USkill::BeginPlay_Implementation()
{
	if (!GetOwningComponent())
	{
		UE_LOG(LogSkill, Error, TEXT("%s has an invalid skill outer. Outer is expected to be its owning skill component."), *GetName())
	}
}

bool USkill::ValidateSkillPreCast_Implementation(FString& ValidationLog)
{
	const bool bSuccess = bUnlocked;
	if (!bUnlocked) ValidationLog.Append("LOCKED;");
	return bSuccess;
}

bool USkill::ValidateSkillMidCast_Implementation(FString& ValidationLog)
{
	return true;
}

bool USkill::ValidateSkillPreActivation_Implementation(FString& ValidationLog)
{
	const bool bSuccess = bUnlocked;
	if (!bUnlocked) ValidationLog.Append("LOCKED;");
	return bSuccess;
}
