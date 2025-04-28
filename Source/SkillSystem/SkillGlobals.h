#pragma once

#include "SkillGlobals.generated.h"

class USkill;

UENUM(BlueprintType)
enum class ESkillTerminationType : uint8
{
	Expired,
	Cancelled,
	Interrupted,
	Invalidated,
	Misc
};

USTRUCT(BlueprintType)
struct FSkillInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<USkill> SkillClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUnlocked = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	int32 SkillLevel = 0;

	FSkillInfo() {}
	FSkillInfo(const USkill* InSkill);
};

USTRUCT(BlueprintType)
struct FSkillUsageLog
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString OwnerName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSkillInfo SkillInfo;
};
