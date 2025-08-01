﻿#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SkillGlobals.h"
#include "Skill.generated.h"

class USkillState;
class USkill;
class USkillComponent;
class USkillEffect;

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
	
	// How long a skill lasts or stays active.
	// If set to 0, it will be instant. If set to a negative number, it will be permanent.
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!bPassive"))
	float Duration = 0;
	
	// How long before a skill can be recast.
	// If set to 0 or lower, it will not have a cooldown.
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", EditCondition = "!bPassive"))
	float Cooldown = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableTick = false;
	
	// The max possible distance between the skill user and the target of the skill.
	// If set to 0 or lower, then the range is treated as infinite.
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	float Range = 0;
	
	// An array of effects that will be applied when this skill is activated
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<USkillEffect>> Effects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<USkillState>> States;

protected:
	UPROPERTY(Replicated, BlueprintReadOnly)
	USkillState* CurrentState = nullptr;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentStateIndex = -1;
	
public:
	USkill();
	
	UFUNCTION(BlueprintPure)
	USkillComponent* GetOwningComponent() const;

	UFUNCTION(BlueprintPure)
	virtual UWorld* GetWorld() const override;
	
	UFUNCTION(BlueprintPure)
	APawn* GetOwningPawn() const;
	
	UFUNCTION(BlueprintPure, Category = "Online")
	bool HasAuthority() const;
	
	UFUNCTION(BlueprintPure, Category = "Online")
	bool IsServer() const;

	UFUNCTION(BlueprintPure, Category = "Online")
	bool IsClient() const;

	// Updates the skill with the new data
	UFUNCTION(BlueprintNativeEvent, Category = "Skill")
	void UpdateSkillInfo(const FSkillInfo& SkillInfo);

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void TryStartSkill();

	// UFUNCTION(Server, Reliable)
	// void ServerChangeState();

	UFUNCTION(Server, Reliable)
	void ServerChangeStateByIndex(int32 NewStateIndex);
	
	UFUNCTION(BlueprintPure, Category = "Debug")
	FString GetClassName() const { return GetClass()->GetName(); }
	
	UFUNCTION(BlueprintPure, Category = "Debug")
	FString GetLockedStatusAsString() const { return bUnlocked ? "Unlocked" : "Locked"; }

	UFUNCTION(BlueprintNativeEvent)
	void Tick(const float DeltaSeconds);
	
protected:
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitProperties() override;

private:
	//UFUNCTION(NetMulticast, Reliable, Category = "Skill")
	//void MulticastChangeState();
};
