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

void USkill::CastSkill_Implementation()
{
	CastTimer = CastTime;
}

bool USkill::ValidateSkillCast_Implementation()
{
	return bUnlocked;
}

void USkill::ActivateSkill_Implementation()
{
	DurationTimer = Duration;
	
	for (TSubclassOf<USkillEffect> Effect : Effects)
	{
		
	}
}

void USkill::DeactivateSkill_Implementation()
{
	CooldownTimer = Cooldown;
}

void USkill::NativeTick(const float DeltaSeconds)
{
	if (CastTimer > 0)
	{
		CastTimer -= DeltaSeconds;
		
		if (CastTimer > 0)
		{
			ValidateSkillCast();
		}
		else // CastTimer <= 0
		{
			ActivateSkill();
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
	BlueprintTick(DeltaSeconds);
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
		if (!GetOuter() || !GetOuter()->IsA(USkillComponent::StaticClass()))
		{
			UE_LOG(LogSkill, Error, TEXT("Invalid skill outer. Outer is expected to be its owning skill component."))
		}
	}
}
