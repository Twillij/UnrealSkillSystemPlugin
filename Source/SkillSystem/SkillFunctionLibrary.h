#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SkillFunctionLibrary.generated.h"

enum class ETriggerEvent : uint8;
class UInputAction;
class USkill;

UCLASS()
class SKILLSYSTEM_API USkillFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static bool BindSkillToInput(const APlayerController* PlayerController, const FName InputActionName,
		const EInputEvent InputEvent, USkill* Skill);

	static bool BindSkillToEnhancedInput(const APlayerController* PlayerController, const UInputAction* InputAction,
		const ETriggerEvent TriggerEvent, USkill* Skill);
};
