#pragma once

#include "SkillComponent.h"
#include "OnlineSkillComponent.generated.h"

class UOnlineSkill;

UCLASS()
class SKILLSYSTEM_API UOnlineSkillComponent : public USkillComponent
{
	GENERATED_BODY()

public:
	virtual bool TryActivateSkill(USkill* Skill) override;

	UFUNCTION(BlueprintPure)
	bool IsServer() const { return GetNetMode() <= NM_ListenServer; }
	
	UFUNCTION(BlueprintPure)
	bool IsClient() const { return GetNetMode() >= NM_ListenServer; }
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerExecuteSkill(UOnlineSkill* Skill);

protected:
	UFUNCTION(Client, Reliable, Category = "Skill|Network")
	void ClientReceiveSkillPreActivationValidationError(UOnlineSkill* Skill, const FString& ErrorLog);
	
	UFUNCTION(NetMulticast, Reliable, Category = "Skill|Network")
	void MulticastStartSkillActivation(UOnlineSkill* Skill);
};
