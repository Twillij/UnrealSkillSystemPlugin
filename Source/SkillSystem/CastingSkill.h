#pragma once

#include "Skill.h"
#include "CastingSkill.generated.h"

UCLASS()
class SKILLSYSTEM_API UCastingSkill : public USkill
{
	GENERATED_BODY()

public:
	// How long it takes for a skill to be cast.
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", EditCondition = "!bPassive"))
	float CastTime = 0;

protected:
	// The timer used for counting down casting
	UPROPERTY(BlueprintReadOnly)
	float CastTimer = 0;

public:
	UCastingSkill();
	
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void SetCastTimer(const float NewTimer);
	float GetCastTimer() const { return CastTimer; }

	// Attempts to cast the skill after doing a validation check on the server.
	// Note: Only call this when overriding OnSkillInputReceived().
	UFUNCTION(Server, Reliable, Category = "Skill")
	void ServerCastSkill();

	// Implements custom code for validating a skill before it can be cast.
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Skill")
	bool CanSkillBeCast(FString& ErrorLog) const;
	bool CanSkillBeCast_Implementation(FString& ErrorLog) const { return true; }

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick_Implementation(const float DeltaSeconds) override;

private:
	UFUNCTION(NetMulticast, Reliable, Category = "Skill")
	void MulticastCastSkill();
};
