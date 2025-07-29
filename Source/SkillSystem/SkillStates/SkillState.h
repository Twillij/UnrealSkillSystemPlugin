#pragma once

#include "CoreMinimal.h"
#include "Skill.h"
#include "SkillState.generated.h"

class USkill;

UCLASS()
class SKILLSYSTEM_API USkillState : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	float StateDuration = 0;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	USkill* OwningSkill = nullptr;

	UPROPERTY(BlueprintReadOnly)
	float StateTimer = 0;

public:
	USkillState() {}
	explicit USkillState(USkill* InOwningSkill) : OwningSkill(InOwningSkill) {}
	
	UFUNCTION(BlueprintNativeEvent)
	void Tick(const float DeltaSeconds);

	UFUNCTION(BlueprintCallable)
	bool TryEnterState();
	
	UFUNCTION(BlueprintNativeEvent)
	bool CanEnterState() const;

	UFUNCTION(BlueprintNativeEvent)
	void OnStateEntered();

	UFUNCTION(BlueprintCallable)
	bool TryExitState();
	
	UFUNCTION(BlueprintNativeEvent)
	bool CanExitState() const;

	UFUNCTION(BlueprintNativeEvent)
	void OnStateExited();

protected:
	virtual bool IsSupportedForNetworking() const override { return true; }
	
	virtual void Tick_Implementation(const float DeltaSeconds);
	
	virtual bool CanEnterState_Implementation() const { return IsValid(OwningSkill); }
	virtual void OnStateEntered_Implementation() { StateTimer = StateDuration; }
	
	virtual bool CanExitState_Implementation() const { return IsValid(OwningSkill); }
	virtual void OnStateExited_Implementation() {}
};
