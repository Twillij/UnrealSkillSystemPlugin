#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SkillEffect.generated.h"

UENUM(BlueprintType)
enum class ESkillEffectApplicationType : uint8
{
	Active,
	Passive,
	Trigger
};

UENUM(BlueprintType)
enum class ESkillEffectDurationType : uint8
{
	Instant,
	Timed,
	Permanent
};

UCLASS(Blueprintable)
class SKILLSYSTEM_API USkillEffect : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillEffectApplicationType ApplicationType = ESkillEffectApplicationType::Active;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillEffectDurationType DurationType = ESkillEffectDurationType::Instant;
};
