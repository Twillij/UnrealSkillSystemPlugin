#include "OnlineSkillComponent.h"
#include "OnlineSkill.h"

void UOnlineSkillComponent::ServerExecuteSkill_Implementation(UOnlineSkill* Skill)
{
	ExecuteSkill(Skill);
}

void UOnlineSkillComponent::ServerExecuteSkillOfClass_Implementation(TSubclassOf<UOnlineSkill> SkillClass)
{
	ExecuteSkillOfClass(SkillClass);
}
