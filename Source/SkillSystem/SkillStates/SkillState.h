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
	UPROPERTY(BlueprintReadOnly)
	USkill* OwningSkill;

	explicit USkillState(USkill* InOwningSkill) : OwningSkill(InOwningSkill) {}
	
	UFUNCTION(BlueprintNativeEvent)
	void Tick(const float DeltaSeconds);

	UFUNCTION(BlueprintCallable)
	void TryEnterState();
	
	UFUNCTION(BlueprintNativeEvent)
	bool CanEnterState() const;

	UFUNCTION(BlueprintNativeEvent)
	void OnStateEntered();

	UFUNCTION(BlueprintCallable)
	void TryExitState();
	
	UFUNCTION(BlueprintNativeEvent)
	bool CanExitState() const;

	UFUNCTION(BlueprintNativeEvent)
	void OnStateExited();

protected:
	virtual void Tick_Implementation(const float DeltaSeconds) {}
	
	virtual bool CanEnterState_Implementation() const { return OwningSkill; }
	virtual void OnStateEntered_Implementation() {}
	
	virtual bool CanExitState_Implementation() const { return OwningSkill; }
	virtual void OnStateExited_Implementation() {}
};
