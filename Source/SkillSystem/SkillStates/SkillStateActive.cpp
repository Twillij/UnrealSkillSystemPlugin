#include "SkillStateActive.h"
#include "Skill.h"

USkillStateActive::USkillStateActive()
{
	StateId = "Active";
}

FName USkillStateActive::GetNextStateId_Implementation(const ESkillStateExitReason ExitReason) const
{
	switch (ExitReason)
	{
	case ESkillStateExitReason::Expired:
		return "Cooldown";
	default:
		return OwningSkill ? OwningSkill->GetDefaultState()->StateId : EName::None;
	}
}

void USkillStateActive::OnStateEntered_Implementation()
{
	StateDuration = OwningSkill ? OwningSkill->Duration : StateDuration;
	StateTimer = StateDuration;
}
