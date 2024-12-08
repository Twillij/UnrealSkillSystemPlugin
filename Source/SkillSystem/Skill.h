#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Skill.generated.h"

struct FSkillData;
class USkillComponent;

UCLASS(Blueprintable)
class SKILLSYSTEM_API USkill : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SkillName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SkillDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool bUnlocked = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, meta = (ClampMin = 0))
	int32 SkillLevel = 0;

protected:
	UPROPERTY(BlueprintReadOnly)
	USkillComponent* OwningComponent = nullptr;

public:
	USkillComponent* GetOwningComponent() const { return OwningComponent; }
	void SetOwningComponent(USkillComponent* InSkillComponent) { OwningComponent = InSkillComponent; }

	UFUNCTION(BlueprintNativeEvent, Category = "Skill")
	void UpdateSkillData(const FSkillData& SkillData);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Skill")
	void ActivateSkill();
	virtual void ActivateSkill_Implementation() {}

	UFUNCTION(BlueprintPure, Category = "Debug")
	FString GetClassName() const { return GetClass()->GetName(); }
	
	UFUNCTION(BlueprintPure, Category = "Debug")
	FString GetLockedStatusAsString() const { return bUnlocked ? "Unlocked" : "Locked"; }

protected:
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
