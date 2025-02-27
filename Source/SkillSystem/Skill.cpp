#include "Skill.h"
#include "SkillComponent.h"
#include "SkillEffect.h"
#include "SkillSystem.h"
#include "Net/UnrealNetwork.h"

FSkillData::FSkillData(const USkill* InSkill)
{
	if (!InSkill) return;
	SkillClass = InSkill->GetClass();
	bUnlocked = InSkill->bUnlocked;
	SkillLevel = InSkill->SkillLevel;
	CastTime = InSkill->CastTime;
	Duration = InSkill->Duration;
	Cooldown = InSkill->Cooldown;
}

void USkill::UpdateSkillData_Implementation(const FSkillData& SkillData)
{
	bUnlocked = SkillData.bUnlocked;
	SkillLevel = SkillData.SkillLevel;
	CastTime = SkillData.CastTime;
	Duration = SkillData.Duration;
	Cooldown = SkillData.Cooldown;
}

USkillComponent* USkill::GetOwningComponent() const
{
	return Cast<USkillComponent>(GetOuter());
}

bool USkill::HasAuthority() const
{
	const USkillComponent* OwningComponent = GetOwningComponent();
	return OwningComponent ? OwningComponent->HasAuthority() : false;
}

bool USkill::IsServer() const
{
	const USkillComponent* Owner = GetOwningComponent();
	return Owner ? Owner->IsServer() : false;
}

bool USkill::IsClient() const
{
	const USkillComponent* Owner = GetOwningComponent();
	return Owner ? Owner->IsClient() : false;
}

void USkill::NativeTick(const float DeltaSeconds)
{
	if (CastTimer > 0)
	{
		RequestOwnerToMaintainCast();
		CastTimer -= DeltaSeconds;
		
		if (CastTimer <= 0)
		{
			RequestOwnerToActivate();
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
	BlueprintTick(DeltaSeconds);
}

void USkill::RequestOwnerToExecute()
{
	if (USkillComponent* Owner = GetOwningComponent())
	{
		Owner->ExecuteSkill(this);
	}
}

void USkill::RequestOwnerToMaintainCast()
{
	if (USkillComponent* Owner = GetOwningComponent())
	{
		if (Owner->IsServer())
		{
			Owner->TryMaintainSkillCast(this);
		}
	}
}

void USkill::RequestOwnerToActivate()
{
	if (USkillComponent* Owner = GetOwningComponent())
	{
		if (Owner->IsServer())
		{
			Owner->TryActivateSkill(this);
		}
	}
}

bool USkill::CanSkillBeCast_Implementation(FString& ErrorLog) const
{
	const bool bSuccess = bUnlocked;
	if (!bUnlocked) ErrorLog.Append("LOCKED;");
	return bSuccess;
}

bool USkill::CanSkillBeActivated_Implementation(FString& ErrorLog) const
{
	const bool bSuccess = bUnlocked;
	if (!bUnlocked) ErrorLog.Append("LOCKED;");
	return bSuccess;
}

void USkill::CastSkill_Implementation()
{
	GetOwningComponent()->CastSkill(this);
	CastTimer = CastTime;
}

void USkill::CancelSkillCast_Implementation()
{
	GetOwningComponent()->CancelSkillCast(this);
	CastTimer = 0;
}

void USkill::ActivateSkill_Implementation()
{
	GetOwningComponent()->ActivateSkill(this);
	DurationTimer = Duration;
}

void USkill::DeactivateSkill_Implementation()
{
	CooldownTimer = Cooldown;
}

void USkill::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USkill, bUnlocked);
	DOREPLIFETIME(USkill, SkillLevel);
	DOREPLIFETIME(USkill, CastTime);
	DOREPLIFETIME(USkill, Duration);
	DOREPLIFETIME(USkill, Cooldown);
}

void USkill::PostInitProperties()
{
	Super::PostInitProperties();
	
	if (GetWorld() && GetWorld()->IsGameWorld())
	{
		if (!GetOwningComponent())
		{
			UE_LOG(LogSkill, Error, TEXT("%s has an invalid skill outer. Outer is expected to be its owning skill component."), *GetName())
		}
		NativeBeginPlay();
	}
}
