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
	virtual void RequestOwnerToMaintainCast() override;
	virtual void RequestOwnerToActivate() override;

	UFUNCTION(BlueprintPure)
	UOnlineSkillComponent* GetOwningOnlineComponent() const;

	UFUNCTION(BlueprintPure)
	bool HasAuthority() const;
	
	UFUNCTION(BlueprintPure)
	bool IsServer() const;

	UFUNCTION(BlueprintPure)
	bool IsClient() const;
};
