#pragma once

#include "SkillComponent.h"
#include "PlayerSkillComponent.generated.h"

UCLASS()
class SKILLSYSTEM_API UPlayerSkillComponent : public USkillComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void BindSkillToInput(USkill* Skill, const UInputAction* InputAction) const;
};
