#include "Skill.h"
#include "SkillComponent.h"
#include "SkillSystem.h"
#include "Net/UnrealNetwork.h"
#include "SkillStates/SkillState.h"
#include "SkillStates/SkillStateActivation.h"

USkill::USkill()
{
	States.Add(USkillStateActivation::StaticClass());
}

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

void USkill::UpdateSkillInfo_Implementation(const FSkillInfo& SkillInfo)
{
	bUnlocked = SkillInfo.bUnlocked;
	SkillLevel = SkillInfo.SkillLevel;
}

void USkill::TryStartSkill()
{
	ServerChangeStateByIndex(0);
}

// void USkill::ServerChangeState_Implementation(const TSubclassOf<USkillState> NewState)
// {
// 	if (CurrentState->TryEnterState())
// 	{
// 		CurrentState = NewObject<USkillState>(this, NewState);
// 	}
// }

void USkill::ServerChangeStateByIndex_Implementation(const int32 NewStateIndex)
{
	if (States.IsValidIndex(NewStateIndex))
	{
		//ServerChangeState(States[NewStateIndex]);
	}
}

void USkill::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USkill, bUnlocked);
	DOREPLIFETIME(USkill, SkillLevel);
	DOREPLIFETIME(USkill, Duration);
	DOREPLIFETIME(USkill, Cooldown);
	DOREPLIFETIME(USkill, CurrentState);
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
	}
}

void USkill::Tick_Implementation(const float DeltaSeconds)
{
	if (bEnableTick)
	{
		CurrentState->Tick(DeltaSeconds);
	}
}
