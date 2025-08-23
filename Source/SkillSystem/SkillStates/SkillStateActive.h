#pragma once

#include "CoreMinimal.h"
#include "SkillState.h"
#include "SkillStateActive.generated.h"

UCLASS()
class SKILLSYSTEM_API USkillStateActive : public USkillState
{
	GENERATED_BODY()

public:
	USkillStateActive();
	
protected:
	virtual void OnStateEntered_Implementation() override;
};
