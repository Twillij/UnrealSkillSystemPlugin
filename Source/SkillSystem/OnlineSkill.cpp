#include "OnlineSkill.h"
#include "OnlineSkillComponent.h"

void UOnlineSkill::RequestOwnerToExecute()
{
	if (UOnlineSkillComponent* Owner = Cast<UOnlineSkillComponent>(GetOwningComponent()))
	{
		Owner->ServerExecuteSkill(this);
	}
} 

void UOnlineSkill::TryCastSkill_Implementation()
{
	FString ValidationLog;
	const bool bValidated = ValidateSkillPreCast(ValidationLog);
	ClientReceivePreCastValidation(bValidated, ValidationLog);
	
	if (bValidated)
	{
		StartCastingSkill();
	}
}

void UOnlineSkill::TryActivateSkill_Implementation()
{
	FString ValidationLog;
	const bool bValidated = ValidateSkillPreActivation(ValidationLog);
	ClientReceivePreActivationValidation(bValidated, ValidationLog);

	if (bValidated)
	{
		DurationTimer = Duration;
	}
}

void UOnlineSkill::Tick_Implementation(const float DeltaSeconds)
{
	if (CastTimer > 0)
	{
		CastTimer -= DeltaSeconds;

		if (HasAuthority())
		{
			if (CastTimer > 0)
			{
				FString ValidationLog;
				if (!ValidateSkillMidCast(ValidationLog))
				{
					ClientReceiveMidCastValidation(false, ValidationLog);
					StopCastingSkill();
				}
			}
			else // CastTimer <= 0
			{
				TryActivateSkill();
			}
		}
	}
	else if (DurationTimer > 0)
	{
		DurationTimer -= DeltaSeconds;
		if (DurationTimer <= 0) DeactivateSkill();
	}
	else if (CooldownTimer > 0)
	{
		CooldownTimer -= DeltaSeconds;
	}
}

void UOnlineSkill::ClientReceivePreCastValidation_Implementation(const bool bSuccess, const FString& ValidationLog)
{
	OnReceivePreCastValidation(bSuccess, ValidationLog);
}

void UOnlineSkill::ClientReceiveMidCastValidation_Implementation(const bool bSuccess, const FString& ValidationLog)
{
	OnReceiveMidCastValidation(bSuccess, ValidationLog);
}

void UOnlineSkill::ClientReceivePreActivationValidation_Implementation(const bool bSuccess, const FString& ValidationLog)
{
	OnReceivePreActivationValidation(bSuccess, ValidationLog);
}
