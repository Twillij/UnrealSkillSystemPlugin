#pragma once

#include "Skill.h"
#include "OnlineSkill.generated.h"

UCLASS()
class SKILLSYSTEM_API UOnlineSkill : public USkill
{
	GENERATED_BODY()

public:
	virtual void TryCastSkill_Implementation() override;
	virtual void TryActivateSkill_Implementation() override;

protected:
	virtual void Tick_Implementation(const float DeltaSeconds) override;
	
	UFUNCTION(Client, Reliable, Category = "Skill|Network")
	void ClientReceivePreCastValidation(const bool bSuccess, const FString& ValidationLog);

	UFUNCTION(Client, Reliable, Category = "Skill|Network")
	void ClientReceiveMidCastValidation(const bool bSuccess, const FString& ValidationLog);
	
	UFUNCTION(Client, Reliable, Category = "Skill|Network")
	void ClientReceivePreActivationValidation(const bool bSuccess, const FString& ValidationLog);
};
