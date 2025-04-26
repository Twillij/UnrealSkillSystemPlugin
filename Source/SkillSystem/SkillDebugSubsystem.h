#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SkillDebugSubsystem.generated.h"

class USkillComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillComponentRegistered, USkillComponent*, Component);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillComponentUnregistered, USkillComponent*, Component);

UCLASS()
class SKILLSYSTEM_API USkillDebugSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnSkillComponentRegistered OnSkillComponentRegistered;

	UPROPERTY(BlueprintAssignable)
	FOnSkillComponentUnregistered OnSkillComponentUnregistered;

protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<USkillComponent*> ActiveSkillComponents;

public:
	static USkillDebugSubsystem* Get(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable)
	void RegisterComponent(USkillComponent* NewComponent);

	UFUNCTION(BlueprintCallable)
	void UnregisterComponent(USkillComponent* RegisteredComponent);

protected:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
};
