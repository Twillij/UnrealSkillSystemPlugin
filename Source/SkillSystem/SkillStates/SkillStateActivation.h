#pragma once

#include "CoreMinimal.h"
#include "SkillState.h"
#include "SkillStateActivation.generated.h"

UCLASS()
class SKILLSYSTEM_API USkillStateActivation : public USkillState
{
	GENERATED_BODY()

protected:
	virtual void OnStateEntered_Implementation() override;
};
