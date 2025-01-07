#include "OnlineSkillComponent.h"
#include "OnlineSkill.h"

void UOnlineSkillComponent::TryCastSkill(USkill* Skill)
{
	UOnlineSkill* OnlineSkill = Cast<UOnlineSkill>(Skill);
	bool bValidated;
	FString ErrorLog;
	ValidateSkillPreCast(OnlineSkill, bValidated, ErrorLog);

	if (bValidated)
		MulticastCastSkill(OnlineSkill);
	else
		ClientReceiveSkillPreCastValidationError(OnlineSkill, ErrorLog);
}

void UOnlineSkillComponent::TryMaintainSkillCast(USkill* Skill)
{
	UOnlineSkill* OnlineSkill = Cast<UOnlineSkill>(Skill);
	bool bValidated;
	FString ErrorLog;
	ValidateSkillMidCast(OnlineSkill, bValidated, ErrorLog);
	
	if (!bValidated)
	{
		ClientReceiveSkillMidCastValidationError(OnlineSkill, ErrorLog);
		MulticastCancelSkillCast(OnlineSkill);
	}
}

void UOnlineSkillComponent::TryActivateSkill(USkill* Skill)
{
	UOnlineSkill* OnlineSkill = Cast<UOnlineSkill>(Skill);
	bool bValidated;
	FString ErrorLog;
	ValidateSkillPreActivation(OnlineSkill, bValidated, ErrorLog);

	if (bValidated)
		MulticastActivateSkill(OnlineSkill);
	else
		ClientReceiveSkillPreActivationValidationError(OnlineSkill, ErrorLog);
}

void UOnlineSkillComponent::ServerExecuteSkill_Implementation(UOnlineSkill* Skill)
{
	ExecuteSkill(Skill);
}

void UOnlineSkillComponent::MulticastCastSkill_Implementation(UOnlineSkill* Skill)
{
	CastSkill(Skill);
	Skill->CastSkill();
}

void UOnlineSkillComponent::MulticastCancelSkillCast_Implementation(UOnlineSkill* Skill)
{
	CancelSkillCast(Skill);
	Skill->CancelSkillCast();
}

void UOnlineSkillComponent::MulticastActivateSkill_Implementation(UOnlineSkill* Skill)
{
	ActivateSkill(Skill);
	Skill->ActivateSkill();
}

void UOnlineSkillComponent::ClientReceiveSkillPreCastValidationError_Implementation(UOnlineSkill* Skill, const FString& ErrorLog)
{
	OnSkillPreCastValidationError.Broadcast(Skill, ErrorLog);
}

void UOnlineSkillComponent::ClientReceiveSkillMidCastValidationError_Implementation(UOnlineSkill* Skill, const FString& ErrorLog)
{
	OnSkillMidCastValidationError.Broadcast(Skill, ErrorLog);
}

void UOnlineSkillComponent::ClientReceiveSkillPreActivationValidationError_Implementation(UOnlineSkill* Skill, const FString& ErrorLog)
{
	OnSkillPreActivationValidationError.Broadcast(Skill, ErrorLog);
}
