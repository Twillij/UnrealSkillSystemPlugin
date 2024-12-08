#include "Skill.h"
#include "SkillGlobals.h"
#include "Net/UnrealNetwork.h"

void USkill::UpdateSkillData_Implementation(const FSkillData& SkillData)
{
	bUnlocked = SkillData.bUnlocked;
	SkillLevel = SkillData.SkillLevel;
}

void USkill::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USkill, bUnlocked);
	DOREPLIFETIME(USkill, SkillLevel);
}
