#include "Skill.h"

#include "SkillComponent.h"
#include "SkillEffect.h"
#include "SkillSystem.h"
#include "Net/UnrealNetwork.h"

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
