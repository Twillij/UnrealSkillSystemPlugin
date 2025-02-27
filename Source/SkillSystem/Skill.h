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
	// The skill name to be displayed to the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SkillName;

	// The skill description to be displayed to the player
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

	// How long it takes for a skill to be cast.
	// If set to 0 or lower, casting will be skipped completely.
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", EditCondition = "!bPassive"))
	float CastTime = 0;

	// The timer used for counting down casting
	UPROPERTY(BlueprintReadWrite)
	float CastTimer = 0;
	
	// How long a skill lasts or stays active.
	// If set to 0, it will be instant. If set to a negative number, it will be permanent.
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!bPassive"))
	float Duration = 0;

	// The timer used for counting down activation duration
	UPROPERTY(BlueprintReadWrite)
	float DurationTimer = 0;
	
	// How long before a skill can be recast
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", EditCondition = "!bPassive"))
	float Cooldown = 0;
	
	// The timer used for counting down cooldown
	UPROPERTY(BlueprintReadWrite)
	float CooldownTimer = 0;

	// The max possible distance between the skill user and the target of the skill.
	// If the target goes out of range, the skill cast or activation will be interrupted.
	// If set to 0 or lower, then the range is treated as infinite.
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	float Range = 0;
	
	// An array of effects that will be applied when this skill is activated
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<USkillEffect>> Effects;

public:
	UFUNCTION(BlueprintPure)
	USkillComponent* GetOwningComponent() const;

	UFUNCTION(BlueprintPure, Category = "Online")
	bool HasAuthority() const;
	
	UFUNCTION(BlueprintPure, Category = "Online")
	bool IsServer() const;

	UFUNCTION(BlueprintPure, Category = "Online")
	bool IsClient() const;
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Tick")
	void BlueprintTick(const float DeltaSeconds);
	virtual void NativeTick(const float DeltaSeconds);

	// Updates the skill with the new data
	UFUNCTION(BlueprintNativeEvent, Category = "Skill")
	void UpdateSkillData(const FSkillData& SkillData);

	// Requests the owning component to execute this skill
	UFUNCTION(BlueprintCallable, Category = "Skill|Execution")
	virtual void RequestOwnerToExecute();

	// Requests the owning component to maintain the casting of this skill
	UFUNCTION(BlueprintCallable, Category = "Skill|Execution")
	virtual void RequestOwnerToMaintainCast();

	// Requests the owning component to activate this skill
	UFUNCTION(BlueprintCallable, Category = "Skill|Execution")
	virtual void RequestOwnerToActivate();

	// A custom implementation for validating a skill before it can be cast
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Skill|Execution")
	bool CanSkillBeCast(FString& ErrorLog) const;

	// A custom implementation for validating a skill before it can be activated
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Skill|Execution")
	bool CanSkillBeActivated(FString& ErrorLog) const;

	// Handles the casting of this skill, called after the skill is successfully validated.
	// This will be called on both the server and all connected clients when using the online component.
	// Intended for custom implementation, with a default implementation that handles the cast timer.
	UFUNCTION(BlueprintNativeEvent, Category = "Skill|Execution")
	void CastSkill();

	// Handles the cancellation of this skill's casting, called after the skill fails its mid-cast validation.
	// This will be called on both the server and all connected clients when using the online component.
	// Intended for custom implementation, with a default implementation that handles the cast timer.
	UFUNCTION(BlueprintNativeEvent, Category = "Skill|Execution")
	void CancelSkillCast();

	// Handles the activation of this skill, called after the skill is successfully validated.
	// This will be called on both the server and all connected clients when using the online component.
	// Intended for custom implementation, with a default implementation that handles the duration timer.
	UFUNCTION(BlueprintNativeEvent, Category = "Skill|Execution")
	void ActivateSkill();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Skill|Execution")
	void DeactivateSkill();
	
	UFUNCTION(BlueprintPure, Category = "Debug")
	FString GetClassName() const { return GetClass()->GetName(); }
	
	UFUNCTION(BlueprintPure, Category = "Debug")
	FString GetLockedStatusAsString() const { return bUnlocked ? "Unlocked" : "Locked"; }

protected:
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitProperties() override;
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "BeginPlay")
	void BlueprintBeginPlay();
	void NativeBeginPlay() { BlueprintBeginPlay(); }
};
