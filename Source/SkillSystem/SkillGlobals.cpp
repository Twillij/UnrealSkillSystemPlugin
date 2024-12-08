#include "SkillGlobals.h"
#include "Skill.h"

FSkillData::FSkillData(const USkill* InSkill)
{
	if (!InSkill)
		return;
	
	SkillClass = InSkill->GetClass();
	bUnlocked = InSkill->bUnlocked;
	SkillLevel = InSkill->SkillLevel;
}
