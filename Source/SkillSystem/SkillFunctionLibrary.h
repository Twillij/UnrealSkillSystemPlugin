#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SkillFunctionLibrary.generated.h"

enum class ETriggerEvent : uint8;
struct FSkillInfo;
class UInputAction;
class USkill;
class USkillComponent;

UCLASS()
class SKILLSYSTEM_API USkillFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Skill")
	static USkill* CreateSkill(USkillComponent* OwningComponent, const FSkillInfo& SkillInfo);

	UFUNCTION(BlueprintCallable, Category = "Skill")
	static FSkillInfo CreateSkillInfo(USkill* Skill);
};
