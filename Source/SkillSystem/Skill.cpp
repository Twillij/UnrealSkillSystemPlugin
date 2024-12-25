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
	CooldownTime = InSkill->CooldownTime;
}

void USkill::NativeTick(const float DeltaSeconds)
{
	CooldownTimer -= DeltaSeconds;
	BlueprintTick(DeltaSeconds);
}

void USkill::UpdateSkillData_Implementation(const FSkillData& SkillData)
{
	bUnlocked = SkillData.bUnlocked;
	SkillLevel = SkillData.SkillLevel;
}

void USkill::ActivateSkill_Implementation()
{
	
	for (TSubclassOf<USkillEffect> Effect : Effects)
	{
		
	}
}

void USkill::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USkill, bUnlocked);
	DOREPLIFETIME(USkill, SkillLevel);
	DOREPLIFETIME(USkill, CastTime);
	DOREPLIFETIME(USkill, CooldownTime);
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
