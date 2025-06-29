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
