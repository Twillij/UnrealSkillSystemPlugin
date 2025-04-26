#include "CastingSkill.h"
#include "SkillComponent.h"
#include "Net/UnrealNetwork.h"

UCastingSkill::UCastingSkill()
{
	bPassive = false;
}

void UCastingSkill::OnSkillInputReceived_Implementation()
{
	ServerCastSkill();
}

void UCastingSkill::SetCastTimer(const float NewTimer)
{
	CastTimer = NewTimer;
	if (CastTimer >= 0 && IsServer())
	{
		ServerActivateSkill();
	}
}

void UCastingSkill::ServerCastSkill_Implementation()
{
	FString ErrorLog;
	if (CanSkillBeCast(ErrorLog))
	{
		MulticastCastSkill();
	}
	else
	{
		// TODO: Handle error
	}
}

inline void UCastingSkill::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCastingSkill, CastTime);
}

void UCastingSkill::Tick_Implementation(const float DeltaSeconds)
{
	if (CastTimer > 0)
	{
		//RequestOwnerToMaintainCast();
		CastTimer -= DeltaSeconds;
		
		if (CastTimer <= 0)
		{
			//RequestOwnerToActivate();
		}
	}

	Super::Tick_Implementation(DeltaSeconds);
}

inline void UCastingSkill::MulticastCastSkill_Implementation()
{
	OnSkillActivation();
	//GetOwningComponent()->OnSkillCast.Broadcast(this);
}
