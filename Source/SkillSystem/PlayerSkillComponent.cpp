#include "PlayerSkillComponent.h"
#include "Skill.h"
#include "EnhancedInputComponent.h"

void UPlayerSkillComponent::BindSkillToInput(USkill* Skill, const UInputAction* InputAction) const
{
	if (!Skills.Contains(Skill) || !InputAction)
		return;

	const AController* OwningController = GetOwningController();
	if (!OwningController || !OwningController->IsLocalPlayerController())
		return;

	UEnhancedInputComponent* InputComponent = Cast<UEnhancedInputComponent>(OwningController->InputComponent);
	if (!InputComponent)
		return;

	InputComponent->BindAction(InputAction, ETriggerEvent::Triggered, Skill, "ActivateSkill");
}
