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
	virtual FName GetNextStateId_Implementation(const ESkillStateExitReason ExitReason) const override;
	virtual void OnStateEntered_Implementation() override;
};
