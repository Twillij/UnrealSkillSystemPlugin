#include "SkillState.h"

void USkillState::TryEnterState()
{
	if (CanEnterState())
	{
		OnStateEntered();
	}
}

void USkillState::TryExitState()
{
	if (CanExitState())
	{
		OnStateExited();
	}
}
