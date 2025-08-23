#include "SkillStateActive.h"
#include "Skill.h"

USkillStateActive::USkillStateActive()
{
	StateId = "Active";
	NextStateOverrides.Add(ESkillStateExitReason::Expired, "Cooldown");
}

void USkillStateActive::OnStateEntered_Implementation()
{
	StateDuration = OwningSkill ? OwningSkill->Duration : StateDuration;
	StateTimer = StateDuration;
}
