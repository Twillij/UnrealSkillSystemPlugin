#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "SkillSystemSettings.generated.h"

UCLASS(Config="SkillSystem", DefaultConfig, meta = (DisplayName="Skill System"))
class SKILLSYSTEM_API USkillSystemSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly)
	bool bEnableDebugger = false;
	
	virtual FName GetCategoryName() const override { return "Plugins"; }
};
