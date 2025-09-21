#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "SkillGlobals.h"
#include "SkillState.generated.h"

class USkill;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillStateEntered);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillStateExited);

UCLASS(Blueprintable)
class SKILLSYSTEM_API USkillState : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName StateId = "Inactive";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StateDuration = 0;
	
	UPROPERTY(BlueprintAssignable)
	FOnSkillStateEntered OnStateEnteredDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FOnSkillStateExited OnStateExitedDelegate;

	UPROPERTY(BlueprintAssignable)
	FEnhancedInputMulticastDelegate OnEnhancedInputReceived;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	USkill* OwningSkill = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	float StateTimer = 0;

public:
	explicit USkillState(USkill* InOwningSkill) : OwningSkill(InOwningSkill) {}
	
	UFUNCTION(BlueprintNativeEvent)
	void Tick(const float DeltaSeconds);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	FName GetNextStateId(const ESkillStateExitReason ExitReason) const;
	
	UFUNCTION(BlueprintCallable)
	bool TryEnterState();
	
	UFUNCTION(BlueprintNativeEvent)
	bool CanEnterState() const;

	UFUNCTION(BlueprintNativeEvent)
	void OnStateEntered();

	UFUNCTION(BlueprintCallable)
	bool TryExitState(const ESkillStateExitReason ExitReason);
	
	UFUNCTION(BlueprintNativeEvent)
	bool CanExitState(const ESkillStateExitReason ExitReason) const;

	UFUNCTION(BlueprintNativeEvent)
	void OnStateExited(const ESkillStateExitReason ExitReason);
	
	void HandleSkillInput(const UInputAction* InputAction, const ETriggerEvent TriggerEvent) const;
	
protected:
	USkillState() {}
	
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual void Tick_Implementation(const float DeltaSeconds);
	
	virtual FName GetNextStateId_Implementation(const ESkillStateExitReason ExitReason) const;
	
	virtual bool CanEnterState_Implementation() const { return true; }
	virtual void OnStateEntered_Implementation() { StateTimer = StateDuration; }
	
	virtual bool CanExitState_Implementation(const ESkillStateExitReason ExitReason) const { return true; }
	virtual void OnStateExited_Implementation(const ESkillStateExitReason ExitReason) {}
};
