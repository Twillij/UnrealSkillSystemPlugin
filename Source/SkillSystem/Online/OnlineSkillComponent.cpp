#include "OnlineSkillComponent.h"
#include "OnlineSkill.h"

bool UOnlineSkillComponent::TryActivateSkill(USkill* Skill)
{
	UOnlineSkill* OnlineSkill = Cast<UOnlineSkill>(Skill);
	bool bValidated;
	FString Results;
	ValidateSkillPreActivation(OnlineSkill, bValidated, Results);

	if (bValidated)
		MulticastStartSkillActivation(OnlineSkill);
	else
		ClientReceiveSkillPreActivationValidationError(OnlineSkill, Results);
	
	return bValidated;
}

void UOnlineSkillComponent::ServerExecuteSkill_Implementation(UOnlineSkill* Skill)
{
	ExecuteSkill(Skill);
}

void UOnlineSkillComponent::ClientReceiveSkillPreActivationValidationError_Implementation(UOnlineSkill* Skill, const FString& ErrorLog)
{
	OnSkillPreActivationValidationError.Broadcast(Skill, ErrorLog);
}

void UOnlineSkillComponent::MulticastStartSkillActivation_Implementation(UOnlineSkill* Skill)
{
	// Execute server logic
	if (IsServer())
	{
		
	}

	// Execute client logic
	if (IsClient())
	{
		
	}
}
