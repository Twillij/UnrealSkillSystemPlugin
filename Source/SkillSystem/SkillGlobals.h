#pragma once

#include "SkillGlobals.generated.h"

class USkillComponent;
class USkill;

DECLARE_DYNAMIC_DELEGATE(FSkillStateDelegate);

UENUM(BlueprintType)
enum class ESkillStateExitReason : uint8
{
	None,
	Expired,
	Cancelled,
	Invalidated,
	Interrupted,
	Other
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
struct FSkillPingInfo
{
	GENERATED_BODY()

	// A unique identifier for the ping
	UPROPERTY(BlueprintReadWrite)
	int32 Handle = -1;

	// The class of the skill associated with the ping
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<USkill> SkillClass;
	
	// The time that the client sends a request to the server
	UPROPERTY(BlueprintReadWrite)
	FDateTime ClientSendRequestTime;

	// The time that the server receives a request from the client
	UPROPERTY(BlueprintReadWrite)
	FDateTime ServerReceiveRequestTime;

	// The time that the server sends a response to the client
	UPROPERTY(BlueprintReadWrite)
	FDateTime ServerSendResponseTime;

	// The time that the client receives a response from the server
	UPROPERTY(BlueprintReadWrite)
	FDateTime ClientReceiveResponseTime;

	// Log string for any additional information about the ping
	UPROPERTY(BlueprintReadWrite)
	FString LogString;
};
