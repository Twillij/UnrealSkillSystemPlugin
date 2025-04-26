#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SkillGlobals.h"
#include "Skill.generated.h"

class USkill;
class USkillComponent;
class USkillEffect;

UCLASS(Blueprintable)
class SKILLSYSTEM_API USkill : public UObject
{
	GENERATED_BODY()

public:
	// The skill name to be displayed to the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SkillName;

	// The skill description to be displayed to the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SkillDescription;

	// Whether this skill has been unlocked or not.
	// A skill cannot be cast unless it is unlocked.
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	bool bUnlocked = false;

	// The level of this skill
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0))
	int32 SkillLevel = 0;

	// Whether this skill is an active or passive type
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bPassive = false;
	
	// How long a skill lasts or stays active.
	// If set to 0, it will be instant. If set to a negative number, it will be permanent.
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!bPassive"))
	float Duration = 0;

	// The timer used for counting down activation duration
	UPROPERTY(BlueprintReadWrite)
	float DurationTimer = 0;
	
	// How long before a skill can be recast
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", EditCondition = "!bPassive"))
	float Cooldown = 0;
	
	// The timer used for counting down cooldown
	UPROPERTY(BlueprintReadWrite)
	float CooldownTimer = 0;

	// The max possible distance between the skill user and the target of the skill.
	// If the target goes out of range, the skill cast or activation will be interrupted.
	// If set to 0 or lower, then the range is treated as infinite.
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	float Range = 0;
	
	// An array of effects that will be applied when this skill is activated
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<USkillEffect>> Effects;
	
public:
	UFUNCTION(BlueprintPure)
	USkillComponent* GetOwningComponent() const;

	UFUNCTION(BlueprintPure)
	virtual UWorld* GetWorld() const override;
	
	UFUNCTION(BlueprintPure)
	APawn* GetOwningPawn() const;
	
	UFUNCTION(BlueprintPure, Category = "Online")
	bool HasAuthority() const;
	
	UFUNCTION(BlueprintPure, Category = "Online")
	bool IsServer() const;

	UFUNCTION(BlueprintPure, Category = "Online")
	bool IsClient() const;

	// Updates the skill with the new data
	UFUNCTION(BlueprintNativeEvent, Category = "Skill")
	void UpdateSkillData(const FSkillData& SkillData);

	// Called when the input for the skill is received - overrideable for custom implementation.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Skill")
	void OnSkillInputReceived();

	// Attempts to activate the skill after doing a validation check on the server.
	// Note: Only call this when overriding OnSkillInputReceived().
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Skill")
	void ServerActivateSkill();

	// Attempts to terminate the skill after doing a validation check on the server.
	// Note: Only call this when overriding OnSkillInputReceived().
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Skill")
	void ServerTerminateSkill(const ESkillTerminationType TerminationType);
	
	// Implements custom code for validating a skill before it can be activated.
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Skill")
	bool CanSkillBeActivated(FString& ErrorLog) const;
	bool CanSkillBeActivated_Implementation(FString& ErrorLog) const { return true; }
	
	// Implements custom code for validating a skill before it can be terminated.
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Skill")
	bool CanSkillBeTerminated(ESkillTerminationType TerminationType, FString& ErrorLog) const;
	bool CanSkillBeTerminated_Implementation(ESkillTerminationType TerminationType, FString& ErrorLog) const { return true; }
	
	// Implements custom code for skill activation, called on both server and client after being validated.
	UFUNCTION(BlueprintNativeEvent, Category = "Skill")
	void OnSkillActivation();

	// Implements custom code for skill termination, called on both server and client after being validated.
	UFUNCTION(BlueprintNativeEvent, Category = "Skill")
	void OnSkillTermination(const ESkillTerminationType TerminationType);
	
	UFUNCTION(BlueprintPure, Category = "Debug")
	FString GetClassName() const { return GetClass()->GetName(); }
	
	UFUNCTION(BlueprintPure, Category = "Debug")
	FString GetLockedStatusAsString() const { return bUnlocked ? "Unlocked" : "Locked"; }

	UFUNCTION(BlueprintNativeEvent)
	void Tick(const float DeltaSeconds);
	
protected:
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitProperties() override;
	
	UFUNCTION(BlueprintNativeEvent)
	void BeginPlay();
	void BeginPlay_Implementation() {}

private:
	UFUNCTION(NetMulticast, Reliable, Category = "Skill")
	void MulticastActivateSkill();

	UFUNCTION(NetMulticast, Reliable, Category = "Skill")
	void MulticastTerminateSkill(const ESkillTerminationType TerminationType);
};
