#include "OnlineSkill.h"
#include "OnlineSkillComponent.h"

void UOnlineSkill::RequestOwnerToExecute()
{
	if (UOnlineSkillComponent* Owner = GetOwningOnlineComponent())
	{
		Owner->ServerExecuteSkill(this);
	}
}

UOnlineSkillComponent* UOnlineSkill::GetOwningOnlineComponent() const
{
	return Cast<UOnlineSkillComponent>(GetOwningComponent());
}

bool UOnlineSkill::IsServer() const
{
	const UOnlineSkillComponent* Owner = GetOwningOnlineComponent();
	return Owner ? Owner->IsServer() : false;
}

bool UOnlineSkill::IsClient() const
{
	const UOnlineSkillComponent* Owner = GetOwningOnlineComponent();
	return Owner ? Owner->IsClient() : false;
}

void UOnlineSkill::Tick_Implementation(const float DeltaSeconds)
{
	
}
