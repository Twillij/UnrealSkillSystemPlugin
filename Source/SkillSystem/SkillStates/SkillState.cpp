#include "SkillState.h"

void USkillState::Tick_Implementation(const float DeltaSeconds)
{
	if (StateTimer > 0)
	{
		StateTimer -= DeltaSeconds;
		if (StateTimer < 0)
		{
			TryExitState();
		}
	}
}

bool USkillState::TryEnterState()
{
	if (CanEnterState())
	{
		OnStateEntered();
		return true;
	}
	return false;
}

bool USkillState::TryExitState()
{
	if (CanExitState())
	{
		OnStateExited();
		return true;
	}
	return false;
}
