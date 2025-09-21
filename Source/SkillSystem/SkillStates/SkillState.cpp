#include "SkillState.h"
#include "Skill.h"

void USkillState::Tick_Implementation(const float DeltaSeconds)
{
	// Check whether the state uses a timer-based duration before ticking the timer
	if (StateDuration >= 0)
	{
		StateTimer -= DeltaSeconds;
		if (StateTimer <= 0)
		{
			OwningSkill->ServerChangeState(GetNextStateId(ESkillStateExitReason::Expired), ESkillStateExitReason::Expired);
		}
	}
}

FName USkillState::GetNextStateId_Implementation(const ESkillStateExitReason ExitReason) const
{
	return OwningSkill ? OwningSkill->DefaultStateId : FName();
}

bool USkillState::TryEnterState()
{
	if (CanEnterState())
	{
		OnStateEntered();
		OnStateEnteredDelegate.Broadcast();
		return true;
	}
	return false;
}

bool USkillState::TryExitState(const ESkillStateExitReason ExitReason)
{
	if (CanExitState(ExitReason))
	{
		OnStateExited(ExitReason);
		OnStateExitedDelegate.Broadcast();
		return true;
	}
	return false;
}

void USkillState::HandleSkillInput(const UInputAction* InputAction, const ETriggerEvent TriggerEvent) const
{
	OnEnhancedInputReceived.Broadcast(InputAction, TriggerEvent);
}
