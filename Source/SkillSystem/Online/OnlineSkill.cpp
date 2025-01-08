#include "OnlineSkill.h"
#include "OnlineSkillComponent.h"

void UOnlineSkill::RequestOwnerToExecute()
{
	if (UOnlineSkillComponent* Owner = GetOwningOnlineComponent())
	{
		Owner->ServerExecuteSkill(this);
	}
}

void UOnlineSkill::RequestOwnerToMaintainCast()
{
	if (UOnlineSkillComponent* Owner = GetOwningOnlineComponent())
	{
		if (Owner->IsServer())
		{
			Owner->TryMaintainSkillCast(this);
		}
	}
}

void UOnlineSkill::RequestOwnerToActivate()
{
	if (UOnlineSkillComponent* Owner = GetOwningOnlineComponent())
	{
		if (Owner->IsServer())
		{
			Owner->TryActivateSkill(this);
		}
	}
}

UOnlineSkillComponent* UOnlineSkill::GetOwningOnlineComponent() const
{
	return Cast<UOnlineSkillComponent>(GetOwningComponent());
}

bool UOnlineSkill::HasAuthority() const
{
	const USkillComponent* OwningComponent = GetOwningComponent();
	return OwningComponent ? OwningComponent->HasAuthority() : false;
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
