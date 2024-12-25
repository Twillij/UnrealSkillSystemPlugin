#include "SkillFunctionLibrary.h"
#include "Skill.h"
#include "SkillSystem.h"
#include "SkillComponent.h"
#include "EnhancedInputComponent.h"

USkill* USkillFunctionLibrary::CreateSkill(USkillComponent* OwningComponent, const FSkillData& SkillData)
{
	if (USkill* NewSkill = NewObject<USkill>(OwningComponent, SkillData.SkillClass))
	{
		NewSkill->UpdateSkillData(SkillData);
		return NewSkill;
	}
	return nullptr;
}

bool USkillFunctionLibrary::BindSkillToInput(const APlayerController* PlayerController, const FName InputActionName,
	const EInputEvent InputEvent, USkill* Skill)
{
	if (!Skill || !PlayerController || !PlayerController->InputComponent || !PlayerController->IsLocalPlayerController())
		return false;

	FInputActionBinding& Binding = PlayerController->InputComponent->BindAction(InputActionName, InputEvent, Skill, &USkill::ActivateSkill);
	return true;
}

bool USkillFunctionLibrary::BindSkillToEnhancedInput(const APlayerController* PlayerController, const UInputAction* InputAction,
	const ETriggerEvent TriggerEvent, USkill* Skill)
{
	if (!InputAction || !Skill || !PlayerController || !PlayerController->InputComponent || !PlayerController->IsLocalPlayerController())
		return false;

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
	{
		FEnhancedInputActionEventBinding& Binding = EnhancedInputComponent->BindAction(InputAction, TriggerEvent, Skill, &USkill::ActivateSkill);
		return true;
	}
	return false;
}
