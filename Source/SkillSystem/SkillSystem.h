#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "SkillSystem.generated.h"

class USkill;

USTRUCT(BlueprintType)
struct FSkillData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<USkill> SkillClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUnlocked = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0))
	int32 SkillLevel = 0;

	FSkillData() {}
	FSkillData(const USkill* InSkill);
};

DECLARE_LOG_CATEGORY_EXTERN(LogSkill, Display, All)

class FSkillSystemModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
