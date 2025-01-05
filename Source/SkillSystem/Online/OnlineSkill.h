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
	virtual void TryCastSkill_Implementation() override;
	virtual void StartActivation_Implementation() override;

	UFUNCTION(BlueprintPure)
	UOnlineSkillComponent* GetOwningOnlineComponent() const;
	
	UFUNCTION(BlueprintPure)
	bool IsServer() const;

	UFUNCTION(BlueprintPure)
	bool IsClient() const;
	
	

protected:
	virtual void Tick_Implementation(const float DeltaSeconds) override;
	
	UFUNCTION(Client, Reliable, Category = "Skill|Network")
	void ClientReceivePreCastValidation(const bool bSuccess, const FString& ErrorLog);

	UFUNCTION(Client, Reliable, Category = "Skill|Network")
	void ClientReceiveMidCastValidation(const bool bSuccess, const FString& ErrorLog);
	
	UFUNCTION(Client, Reliable, Category = "Skill|Network")
	void ClientReceivePreActivationValidation(const bool bSuccess, const FString& ErrorLog);
};
