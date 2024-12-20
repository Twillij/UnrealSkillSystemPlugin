#include "SkillEffect.h"

void USkillEffect::NativeTick(const float DeltaSeconds)
{
	if (bIsActive) ActiveTime += DeltaSeconds;
	BlueprintTick(DeltaSeconds);
}
