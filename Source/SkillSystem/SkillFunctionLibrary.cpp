#include "SkillFunctionLibrary.h"
#include "Skill.h"
#include "SkillComponent.h"
#include "EnhancedInputComponent.h"

USkill* USkillFunctionLibrary::CreateSkill(USkillComponent* OwningComponent, const FSkillInfo& SkillInfo)
{
	if (USkill* NewSkill = NewObject<USkill>(OwningComponent, SkillInfo.SkillClass))
	{
		NewSkill->UpdateSkillInfo(SkillInfo);
		return NewSkill;
	}
	return nullptr;
}

FSkillInfo USkillFunctionLibrary::CreateSkillInfo(USkill* Skill)
{
	return FSkillInfo(Skill);
}
