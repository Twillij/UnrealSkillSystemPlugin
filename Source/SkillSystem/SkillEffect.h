#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SkillEffect.generated.h"

class USkillComponent;

UENUM(BlueprintType)
enum class ESkillEffectApplicationType : uint8
{
	Immediate,
	Delay,
	Trigger
};

UENUM(BlueprintType)
enum class ESkillEffectDurationType : uint8
{
	Instant,
	Timed,
	Permanent
};

UENUM(BlueprintType)
enum class ESkillEffectTargetingType : uint8
{
	Self,
	Target,
	Location
};

USTRUCT(BlueprintType)
struct FSkillEffectHandle
{
	GENERATED_BODY()
	
};

UCLASS(Blueprintable)
class SKILLSYSTEM_API USkillEffect : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillEffectApplicationType ApplicationType = ESkillEffectApplicationType::Immediate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillEffectDurationType DurationType = ESkillEffectDurationType::Instant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillEffectTargetingType TargetingType = ESkillEffectTargetingType::Self;
	
	UPROPERTY(BlueprintReadWrite, Category = "Targeting")
	USkillComponent* TargetComponent;

	UPROPERTY(BlueprintReadWrite, Category = "Targeting")
	FVector TargetLocation;

	UPROPERTY(BlueprintReadWrite)
	bool bIsActive = false;

protected:
	UPROPERTY(BlueprintReadOnly)
	USkillComponent* SourceComponent = nullptr;
	
private:
	float ActiveTime;
	
public:
	UFUNCTION(BlueprintPure)
	float GetActiveTime() const { return ActiveTime; }

	// Tick function managed by the target skill component once applied
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Tick")
	void BlueprintTick(const float DeltaSeconds);
	void NativeTick(const float DeltaSeconds);
};
