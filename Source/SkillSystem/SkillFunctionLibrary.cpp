#include "SkillFunctionLibrary.h"
#include "Skill.h"
#include "SkillComponent.h"
#include "EnhancedInputComponent.h"

USkill* USkillFunctionLibrary::CreateSkill(USkillComponent* OwningComponent, const FSkillData& SkillData)
{
	if (USkill* NewSkill = NewObject<USkill>(OwningComponent, SkillData.SkillClass))
	{
		NewSkill->UpdateSkillData(SkillData);
		return NewSkill;
	}
	return nullptr;
}

FSkillData USkillFunctionLibrary::CreateSkillData(USkill* Skill)
{
	return FSkillData(Skill);
}
