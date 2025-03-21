﻿#include "Skill.h"
#include "SkillComponent.h"
#include "SkillEffect.h"
#include "SkillSystem.h"
#include "Net/UnrealNetwork.h"

USkillComponent* USkill::GetOwningComponent() const
{
	return Cast<USkillComponent>(GetOuter());
}

UWorld* USkill::GetWorld() const
{
	if (IsTemplate() || !GetOuter()) return nullptr;
	return GetOuter()->GetWorld();
}

APawn* USkill::GetOwningPawn() const
{
	const USkillComponent* Owner = GetOwningComponent();
	return Owner ? Owner->GetOwningPawn() : nullptr;
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

void USkill::UpdateSkillData_Implementation(const FSkillData& SkillData)
{
	bUnlocked = SkillData.bUnlocked;
	SkillLevel = SkillData.SkillLevel;
}

void USkill::ExecuteSkill_Implementation()
{
	ServerActivateSkill();
}

void USkill::ServerActivateSkill_Implementation()
{
	FString ErrorLog;
	if (CanSkillBeActivated(ErrorLog))
	{
		MulticastActivateSkill();
	}
	else
	{
		// TODO: Handle error
	}
}

void USkill::ServerTerminateSkill_Implementation(const ESkillTerminationType TerminationType)
{
	FString ErrorLog;
	if (CanSkillBeTerminated(TerminationType, ErrorLog))
	{
		MulticastTerminateSkill(TerminationType);
	}
	else
	{
		// TODO: Handle error
	}
}

void USkill::OnSkillActivation_Implementation()
{
	DurationTimer = Duration;
}

void USkill::OnSkillTermination_Implementation(const ESkillTerminationType TerminationType)
{
	CooldownTimer = Cooldown;
}

void USkill::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USkill, bUnlocked);
	DOREPLIFETIME(USkill, SkillLevel);
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
		BeginPlay();
	}
}

void USkill::Tick_Implementation(const float DeltaSeconds)
{
	if (DurationTimer > 0)
	{
		DurationTimer -= DeltaSeconds;
		if (DurationTimer <= 0 && IsServer()) ServerTerminateSkill(ESkillTerminationType::Expired);
	}
	else if (CooldownTimer > 0)
	{
		CooldownTimer -= DeltaSeconds;
	}
}

void USkill::MulticastActivateSkill_Implementation()
{
	OnSkillActivation();
	GetOwningComponent()->OnSkillActivated.Broadcast(this);
}

void USkill::MulticastTerminateSkill_Implementation(const ESkillTerminationType TerminationType)
{
	OnSkillTermination(TerminationType);
	GetOwningComponent()->OnSkillTerminated.Broadcast(this, TerminationType);
}
