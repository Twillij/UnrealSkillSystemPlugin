#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SkillFunctionLibrary.generated.h"

enum class ETriggerEvent : uint8;
struct FSkillData;
class UInputAction;
class USkill;
class USkillComponent;

UCLASS()
class SKILLSYSTEM_API USkillFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Skill")
	static USkill* CreateSkill(USkillComponent* OwningComponent, const FSkillData& SkillData);

	UFUNCTION(BlueprintCallable, Category = "Skill")
	static FSkillData CreateSkillData(USkill* Skill);
};
