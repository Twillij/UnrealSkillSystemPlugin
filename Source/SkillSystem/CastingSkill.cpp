#include "CastingSkill.h"
#include "SkillComponent.h"
#include "Net/UnrealNetwork.h"

UCastingSkill::UCastingSkill()
{
	bPassive = false;
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
