#pragma once

#include "Skill.h"
#include "OnlineSkill.generated.h"

class UOnlineSkillComponent;

UCLASS()
class SKILLSYSTEM_API UOnlineSkill : public USkill
{
	GENERATED_BODY()

public:
	virtual void RequestOwnerToExecute() override;

	UFUNCTION(BlueprintPure)
	UOnlineSkillComponent* GetOwningOnlineComponent() const;
	
	UFUNCTION(BlueprintPure)
	bool IsServer() const;

	UFUNCTION(BlueprintPure)
	bool IsClient() const;
	
protected:
	virtual void Tick_Implementation(const float DeltaSeconds) override;
};
