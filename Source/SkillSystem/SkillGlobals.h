#pragma once

#include "SkillGlobals.generated.h"

class USkillComponent;
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
struct FSkillUsage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int32 Handle = -1;
	
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<USkill> SkillClass;
	
	// The time that the client sends a request to the server to process an inputted command to use a skill
	UPROPERTY(BlueprintReadWrite)
	FDateTime ClientSendInputTime;

	// The time that the server receives an inputted command from the client to use a skill
	UPROPERTY(BlueprintReadWrite)
	FDateTime ServerReceiveInputTime;

	// The time that the server sends a response to the client on whether to use a skill
	UPROPERTY(BlueprintReadWrite)
	FDateTime ServerSendResponseTime;

	// The time that the client receives a response from the server on whether to use a skill
	UPROPERTY(BlueprintReadWrite)
	FDateTime ClientReceiveResponseTime;
};
