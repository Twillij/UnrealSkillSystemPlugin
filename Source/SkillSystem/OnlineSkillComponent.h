#pragma once

#include "SkillComponent.h"
#include "OnlineSkillComponent.generated.h"

class UOnlineSkill;

UCLASS()
class SKILLSYSTEM_API UOnlineSkillComponent : public USkillComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerExecuteSkill(UOnlineSkill* Skill);
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerExecuteSkillOfClass(TSubclassOf<UOnlineSkill> SkillClass);
};
