#include "SkillGlobals.h"
#include "Skill.h"

FSkillInfo::FSkillInfo(const USkill* InSkill)
{
	if (!InSkill) return;
	SkillClass = InSkill->GetClass();
	bUnlocked = InSkill->bUnlocked;
	SkillLevel = InSkill->SkillLevel;
}
