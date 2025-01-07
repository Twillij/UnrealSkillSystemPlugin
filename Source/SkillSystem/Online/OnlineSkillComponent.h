#pragma once

#include "SkillComponent.h"
#include "OnlineSkillComponent.generated.h"

class UOnlineSkill;

UCLASS()
class SKILLSYSTEM_API UOnlineSkillComponent : public USkillComponent
{
	GENERATED_BODY()

public:
	virtual void TryCastSkill(USkill* Skill) override;
	virtual void TryMaintainSkillCast(USkill* Skill) override;
	virtual void TryActivateSkill(USkill* Skill) override;

	UFUNCTION(BlueprintPure, Category = "Network")
	bool IsServer() const { return GetNetMode() <= NM_ListenServer; }
	
	UFUNCTION(BlueprintPure, Category = "Network")
	bool IsClient() const { return GetNetMode() >= NM_ListenServer; }
	
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Skill|Execution")
	void ServerExecuteSkill(UOnlineSkill* Skill);

protected:
	UFUNCTION(NetMulticast, Reliable, Category = "Skill|Execution")
	void MulticastCastSkill(UOnlineSkill* Skill);

	UFUNCTION(NetMulticast, Reliable, Category = "Skill|Execution")
	void MulticastCancelSkillCast(UOnlineSkill* Skill);
	
	UFUNCTION(NetMulticast, Reliable, Category = "Skill|Execution")
	void MulticastActivateSkill(UOnlineSkill* Skill);
	
	UFUNCTION(Client, Reliable, Category = "Skill|Execution")
	void ClientReceiveSkillPreCastValidationError(UOnlineSkill* Skill, const FString& ErrorLog);

	UFUNCTION(Client, Reliable, Category = "Skill|Execution")
	void ClientReceiveSkillMidCastValidationError(UOnlineSkill* Skill, const FString& ErrorLog);
	
	UFUNCTION(Client, Reliable, Category = "Skill|Execution")
	void ClientReceiveSkillPreActivationValidationError(UOnlineSkill* Skill, const FString& ErrorLog);
};
