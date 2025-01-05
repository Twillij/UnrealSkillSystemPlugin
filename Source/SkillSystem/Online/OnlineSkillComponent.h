#pragma once

#include "SkillComponent.h"
#include "OnlineSkillComponent.generated.h"

class UOnlineSkill;

UCLASS()
class SKILLSYSTEM_API UOnlineSkillComponent : public USkillComponent
{
	GENERATED_BODY()

public:
	virtual void CastSkill(USkill* Skill) override;
	virtual void KeepCastingSkill(USkill* Skill) override;
	virtual void ActivateSkill(USkill* Skill) override;

	UFUNCTION(BlueprintPure)
	bool IsServer() const { return GetNetMode() <= NM_ListenServer; }
	
	UFUNCTION(BlueprintPure)
	bool IsClient() const { return GetNetMode() >= NM_ListenServer; }
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerExecuteSkill(UOnlineSkill* Skill);

protected:
	UFUNCTION(NetMulticast, Reliable, Category = "Skill|Network")
	void MulticastCastSkill(UOnlineSkill* Skill);

	UFUNCTION(NetMulticast, Reliable, Category = "Skill|Network")
	void MulticastInterruptSkillCast(UOnlineSkill* Skill);
	
	UFUNCTION(NetMulticast, Reliable, Category = "Skill|Network")
	void MulticastActivateSkill(UOnlineSkill* Skill);
	
	UFUNCTION(Client, Reliable, Category = "Skill|Network")
	void ClientReceiveSkillPreCastValidationError(UOnlineSkill* Skill, const FString& ErrorLog);

	UFUNCTION(Client, Reliable, Category = "Skill|Network")
	void ClientReceiveSkillMidCastValidationError(UOnlineSkill* Skill, const FString& ErrorLog);
	
	UFUNCTION(Client, Reliable, Category = "Skill|Network")
	void ClientReceiveSkillPreActivationValidationError(UOnlineSkill* Skill, const FString& ErrorLog);
};

