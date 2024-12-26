#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Skill.generated.h"

class USkill;
class USkillComponent;
class USkillEffect;

USTRUCT(BlueprintType)
struct FSkillData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<USkill> SkillClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUnlocked = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	int32 SkillLevel = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	float CastTime = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	float Cooldown = 0;

	FSkillData() {}
	FSkillData(const USkill* InSkill);
};

UCLASS(Blueprintable)
class SKILLSYSTEM_API USkill : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SkillName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SkillDescription;

	// Whether this skill has been unlocked or not.
	// A skill cannot be cast unless it is unlocked.
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	bool bUnlocked = false;

	// The level of this skill
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0))
	int32 SkillLevel = 0;

	// Whether this skill is an active or passive type
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bPassive = false;

	// How long it takes for a skill to be cast
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", EditCondition = "!bPassive"))
	float CastTime = 0;
	
	// How long a skill lasts or stays active.
	// If set to 0, it will be instant. If set to a negative number, it will be permanent.
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!bPassive"))
	float Duration = 0;

	// How long before a skill can be recast
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", EditCondition = "!bPassive"))
	float Cooldown = 0;

	// An array of effects that will be applied when this skill is activated
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<USkillEffect>> Effects;

protected:
	UPROPERTY(BlueprintReadOnly)
	float CastTimer = 0;

	UPROPERTY(BlueprintReadOnly)
	float DurationTimer = 0;

	UPROPERTY(BlueprintReadOnly)
	float CooldownTimer = 0;

public:
	virtual void operator=(const FSkillData& SkillData) { UpdateSkillData(SkillData); }

	float GetCastTimer() const { return CastTimer; }
	float GetDurationTimer() const { return DurationTimer; }
	float GetCooldownTimer() const { return CooldownTimer; }
	
	UFUNCTION(BlueprintNativeEvent, Category = "Skill")
	void UpdateSkillData(const FSkillData& SkillData);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Skill")
	void CastSkill();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Skill")
	bool ValidateSkillCast();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Skill")
	void ActivateSkill();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Skill")
	void DeactivateSkill();

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Tick")
	void BlueprintTick(const float DeltaSeconds);
	virtual void NativeTick(const float DeltaSeconds);
	
	UFUNCTION(BlueprintPure, Category = "Debug")
	FString GetClassName() const { return GetClass()->GetName(); }
	
	UFUNCTION(BlueprintPure, Category = "Debug")
	FString GetLockedStatusAsString() const { return bUnlocked ? "Unlocked" : "Locked"; }

protected:
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitProperties() override;
};

