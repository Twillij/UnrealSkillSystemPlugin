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
struct FSkillData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<USkill> SkillClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUnlocked = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	int32 SkillLevel = 0;

	FSkillData() {}
	FSkillData(const USkill* InSkill);
};
