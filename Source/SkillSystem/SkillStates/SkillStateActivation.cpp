#include "SkillStateActivation.h"

void USkillStateActivation::OnStateEntered_Implementation()
{
	StateDuration = OwningSkill ? OwningSkill->Duration : StateDuration;
	StateTimer = StateDuration;
}
