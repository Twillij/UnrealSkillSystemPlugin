#include "SkillDebugSubsystem.h"

USkillDebugSubsystem* USkillDebugSubsystem::Get(const UObject* WorldContextObject)
{
	const UWorld* World = WorldContextObject ? WorldContextObject->GetWorld() : nullptr;
	return World ? World->GetSubsystem<USkillDebugSubsystem>() : nullptr;
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

bool USkillDebugSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	const bool bShouldCreateSubsystem = Super::ShouldCreateSubsystem(Outer);
	UE_LOG(LogTemp, Warning, TEXT("ShouldCreateSubsystem: %s"), bShouldCreateSubsystem ? TEXT("TRUE") : TEXT("FALSE"));
	return bShouldCreateSubsystem;
}
