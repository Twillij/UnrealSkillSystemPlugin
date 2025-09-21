#include "Skill.h"
#include "SkillComponent.h"
#include "SkillSystem.h"
#include "Net/UnrealNetwork.h"
#include "SkillStates/SkillState.h"
#include "SkillStates/SkillStateActive.h"

USkill::USkill()
{
	StateClasses.Add(USkillState::StaticClass());
	StateClasses.Add(USkillStateActive::StaticClass());
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

USkillState* USkill::GetDefaultState() const
{
	if (DefaultStateId.IsNone())
	{
		return !States.IsEmpty() ? States[0] : nullptr;
	}
	return GetStateById(DefaultStateId);
}

void USkill::UpdateSkillInfo_Implementation(const FSkillInfo& SkillInfo)
{
	bUnlocked = SkillInfo.bUnlocked;
	SkillLevel = SkillInfo.SkillLevel;
}

USkillState* USkill::GetStateById(const FName StateId) const
{
	for (USkillState* State : States)
	{
		if (State && State->StateId == StateId)
		{
			return State;
		}
	}
	return nullptr;
}

int32 USkill::GetStateIndexById(const FName StateId) const
{
	for (int i = 0; i < States.Num(); ++i)
	{
		if (States[i] && States[i]->StateId == StateId)
		{
			return i;
		}
	}
	return -1;
}

void USkill::BindToStateEntry(const FName StateName, const FSkillStateDelegate& Delegate) const
{
	if (USkillState* State = GetStateById(StateName))
	{
		State->OnStateEnteredDelegate.Add(Delegate);
	}
}

void USkill::BindToStateExit(const FName StateName, const FSkillStateDelegate& Delegate) const
{
	if (USkillState* State = GetStateById(StateName))
	{
		State->OnStateExitedDelegate.Add(Delegate);
	}
}

void USkill::BindInputToState(const FName StateName, const FEnhancedInputDelegate& Delegate)
{
	if (USkillState* State = GetStateById(StateName))
	{
		State->OnEnhancedInputReceived.Add(Delegate);
	}
}

void USkill::HandleSkillInput(const UInputAction* InputAction, const ETriggerEvent TriggerEvent)
{
	if (CurrentState)
	{
		CurrentState->HandleSkillInput(InputAction, TriggerEvent);
	}
}

void USkill::ServerChangeState_Implementation(const FName NewStateId, const ESkillStateExitReason ChangeReason)
{
	if (CurrentState && !CurrentState->TryExitState(ChangeReason))
	{
		return;
	}

	USkillState* NewState = GetStateById(NewStateId);
	
	if (NewState->TryEnterState())
	{
		CurrentState = NewState;
	}
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
	}
}

void USkill::BeginPlay()
{
	if (HasAuthority())
	{
		for (const UClass* StateClass : StateClasses)
		{
			States.Add(NewObject<USkillState>(this, StateClass));
		}
		
		if (States.IsEmpty())
		{
			return;
		}
		
		if (DefaultStateId.IsNone())
		{
			DefaultStateId = States[0]->StateId;
		}
		
		ServerChangeState(DefaultStateId);
	}
}

void USkill::NativeTick(const float DeltaSeconds)
{
	if (bEnableTick)
	{
		if (CurrentState)
		{
			CurrentState->Tick(DeltaSeconds);
		}
	}
}
