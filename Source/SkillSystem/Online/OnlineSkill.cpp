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
	StartCastingSkill();
}

void UOnlineSkill::StartActivation_Implementation()
{
	DurationTimer = Duration;
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
	if (CastTimer > 0)
	{
		CastTimer -= DeltaSeconds;

		if (HasAuthority())
		{
			if (CastTimer > 0)
			{
				FString ErrorLog;
				if (!ValidateSkillMidCast(ErrorLog))
				{
					ClientReceiveMidCastValidation(false, ErrorLog);
					StopCastingSkill();
				}
			}
			else // CastTimer <= 0
			{
				StartActivation();
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

void UOnlineSkill::ClientReceivePreCastValidation_Implementation(const bool bSuccess, const FString& ErrorLog)
{
	OnReceivePreCastValidation(bSuccess, ErrorLog);
}

void UOnlineSkill::ClientReceiveMidCastValidation_Implementation(const bool bSuccess, const FString& ErrorLog)
{
	OnReceiveMidCastValidation(bSuccess, ErrorLog);
}

void UOnlineSkill::ClientReceivePreActivationValidation_Implementation(const bool bSuccess, const FString& ErrorLog)
{
	OnReceivePreActivationValidation(bSuccess, ErrorLog);
}
