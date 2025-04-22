#include "SkillDebugSubsystem.h"
#include "Kismet/GameplayStatics.h"

USkillDebugSubsystem* USkillDebugSubsystem::Get(const UObject* WorldContextObject)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	return GameInstance ? GameInstance->GetSubsystem<USkillDebugSubsystem>() : nullptr;
}

void USkillDebugSubsystem::RegisterComponent(USkillComponent* NewComponent)
{
	ActiveSkillComponents.Add(NewComponent);
	OnSkillComponentRegistered.Broadcast(NewComponent);
}

void USkillDebugSubsystem::UnregisterComponent(USkillComponent* RegisteredComponent)
{
	ActiveSkillComponents.Remove(RegisteredComponent);
	OnSkillComponentUnregistered.Broadcast(RegisteredComponent);
}
