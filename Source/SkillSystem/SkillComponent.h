#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillComponent.generated.h"

enum class ETriggerEvent : uint8;
struct FSkillData;
class USkillEffect;
class UInputAction;
class USkill;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillStateChanged, USkill*, Skill);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillValidationError, USkill*, Skill, const FString&, ErrorLog);

UCLASS(Blueprintable)
class SKILLSYSTEM_API USkillComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    FOnSkillStateChanged OnSkillActivated;

    UPROPERTY(BlueprintAssignable)
    FOnSkillValidationError OnSkillPreCastValidationError;
    
    UPROPERTY(BlueprintAssignable)
    FOnSkillValidationError OnSkillMidCastValidationError;
    
    UPROPERTY(BlueprintAssignable)
    FOnSkillValidationError OnSkillPreActivationValidationError;
    
    // Intended to be modified on the client only, e.g. to load skills from the client's save file.
    // Will be sent to and processed by the server whenever ClientUploadSkillData() is called.
    // Afterwards, a copy of the data will be stored on the server for caching purposes.
    UPROPERTY(BlueprintReadWrite, Category = "Skill|Network")
    TArray<FSkillData> ClientSkillData;
    
protected:
    // Skill data used to create the initial array of skills
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FSkillData> PresetSkills;
    
    // A replicated array of currently owned skills
    UPROPERTY(Replicated)
    TArray<USkill*> OwnedSkills;

    // A replicated array of currently applied effects
    UPROPERTY(Replicated)
    TArray<USkillEffect*> AppliedEffects;

public:
    USkillComponent();
    
    UFUNCTION(BlueprintPure, Category = "Component")
    APawn* GetOwningPawn() const { return Cast<APawn>(GetOwner()); }

    UFUNCTION(BlueprintPure, Category = "Component")
    AController* GetOwningController() const;

    // Returns true if the owning actor has network authority.
    UFUNCTION(BlueprintPure, Category = "Network")
    bool HasAuthority() const { return GetOwnerRole() == ROLE_Authority; }

    UFUNCTION(BlueprintPure, Category = "Network")
    bool IsServer() const { return GetNetMode() <= NM_ListenServer; }

    UFUNCTION(BlueprintPure, Category = "Network")
    bool IsClient() const { return GetNetMode() >= NM_ListenServer; }
    
    // Returns true if the owning pawn is locally controlled.
    UFUNCTION(BlueprintPure, Category = "Network")
    bool IsLocallyControlled() const;
    
    // Returns all currently owned skills.
    UFUNCTION(BlueprintPure, Category = "Skill")
    TArray<USkill*> GetOwnedSkills() { return OwnedSkills; }

    // Returns an owned skill of the specified class if found, null otherwise.
    UFUNCTION(BlueprintPure, Category = "Skill")
    USkill* GetSkillOfClass(TSubclassOf<USkill> SkillClass);

    UFUNCTION(BlueprintPure, Category = "Skill")
    bool HasSkill(const USkill* Skill) const;

    // Registers a unique skill instance. Returns true if successful.
    UFUNCTION(BlueprintCallable, Category = "Skill")
    bool RegisterSkill(USkill* Skill);

    // De-registers a skill instance. Returns true if successful.
    UFUNCTION(BlueprintCallable, Category = "Skill")
    bool DeregisterSkill(USkill* Skill);

    // Processes a skill data and either updates an existing skill or creates a new one.
    UFUNCTION(BlueprintCallable, Category = "Skill")
    void ProcessSkillData(const FSkillData& InData);

    // Starts the execution of a skill and tries to go through each of the implemented execution phases.
    // If the cast time for the skill is set to 0, then it will skip the casting phase and go immediately to activation.
    UFUNCTION(BlueprintCallable, Category = "Skill|Execution")
    void ExecuteSkill(USkill* Skill);

    // Attempts to validate a skill before casting it.
    // If the validation fails, it will broadcast the corresponding validation error delegate.
    UFUNCTION(BlueprintCallable, Category = "Skill|Execution")
    virtual void TryCastSkill(USkill* Skill);

    // Attempts to validate a skill that is mid-cast every frame.
    // If the validation fails, it will cancel the cast and broadcast the corresponding validation error delegate.
    UFUNCTION(BlueprintCallable, Category = "Skill|Execution")
    virtual void TryMaintainSkillCast(USkill* Skill);

    // Attempts to validate a skill before activating it.
    // If the validation fails, it will broadcast the corresponding validation error delegate.
    UFUNCTION(BlueprintCallable, Category = "Skill|Execution")
    virtual void TryActivateSkill(USkill* Skill);

    // Attempts to validate a skill before deactivating it.
    // If the validation fails, it will broadcast the corresponding validation error delegate.
    //UFUNCTION(BlueprintCallable, Category = "Skill|Execution")
    //virtual void TryDeactivateSkill(USkill* Skill);
    
    // A custom implementation for validating a skill before it can be cast
    UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Skill|Execution")
    bool CanSkillBeCast(USkill* Skill, FString& ErrorLog) const;
    bool CanSkillBeCast_Implementation(USkill* Skill, FString& ErrorLog) const { return true; }

    // A custom implementation for validating a skill before it can be activated
    UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Skill|Execution")
    bool CanSkillBeActivated(USkill* Skill, FString& ErrorLog) const;
    bool CanSkillBeActivated_Implementation(USkill* Skill, FString& ErrorLog) const { return true; }

    // A custom implementation for casting any skill, called after the skill is successfully validated.
    // This will be called on both the server and all connected clients when using the online component.
    UFUNCTION(BlueprintNativeEvent, Category = "Skill|Execution")
    void CastSkill(USkill* Skill);
    void CastSkill_Implementation(USkill* Skill) {}

    // A custom implementation for cancelling any skill, called after the skill fails its mid-cast validation.
    // This will be called on both the server and all connected clients when using the online component.
    UFUNCTION(BlueprintNativeEvent, Category = "Skill|Execution")
    void CancelSkillCast(USkill* Skill);
    void CancelSkillCast_Implementation(USkill* Skill) {}
    
    // A custom implementation for activating any skill, called after the skill is successfully validated.
    // This will be called on both the server and all connected clients when using the online component.
    UFUNCTION(BlueprintNativeEvent, Category = "Skill|Execution")
    void ActivateSkill(USkill* Skill);
    void ActivateSkill_Implementation(USkill* Skill) {}
    
    // A custom implementation for activating any skill, called after the skill is successfully deactivated.
    // This will be called on both the server and all connected clients when using the online component.
    UFUNCTION(BlueprintNativeEvent, Category = "Skill|Execution")
    void DeactivateSkill(USkill* Skill);
    void DeactivateSkill_Implementation(USkill* Skill) {}
    
    // Applies a skill effect to this component.
    UFUNCTION(BlueprintCallable, Category = "Skill|Effect")
    void ApplySkillEffect(USkillEffect* Effect);

    // Binds a skill execution to an input action.
    UFUNCTION(BlueprintCallable, Category = "Skill|Input")
    bool BindSkillToInput(const TSubclassOf<USkill> SkillClass, const FName InputActionName, const EInputEvent InputEvent);

    // Binds a skill execution to an input action.
    UFUNCTION(BlueprintCallable, Category = "Skill|Input")
    bool BindSkillToEnhancedInput(const TSubclassOf<USkill> SkillClass, const UInputAction* InputAction, const ETriggerEvent TriggerEvent);
    
    // Default implementation returns the ClientSkillData array.
    // Intended for custom implementation to be called on the client e.g. to load skills from the client's save file.
    // Called by ClientUploadSkillData() to be sent to the server to process.
    UFUNCTION(BlueprintNativeEvent, Category = "Skill|Network")
    TArray<FSkillData> GetClientSkillData();
    
    // Sends ClientSourcedSkillData to the server to process.
    UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Skill|Network")
    void ClientUploadSkillData();
    
    // Processes the provided skill data on the server and replicates any data changes back to the owning client.
    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Skill|Network")
    void ServerDownloadSkillData(const TArray<FSkillData>& InDataArray);
    
    UFUNCTION(BlueprintPure, Category = "Debug")
    FString GetClassName() const { return GetClass()->GetName(); }

    UFUNCTION(BlueprintPure, Category = "Debug")
    FString GetOwnerName() const;
    
    UFUNCTION(BlueprintPure, Category = "Debug")
    FString GetOwnerNetRoleAsString() const;

protected:
    virtual void OnRegister() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void TickComponent(const float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    
    void ValidateSkillPreCast(USkill* Skill, bool& bValidated, FString& ErrorLog) const;
    void ValidateSkillMidCast(USkill* Skill, bool& bValidated, FString& ErrorLog) const;
    void ValidateSkillPreActivation(USkill* Skill, bool& bValidated, FString& ErrorLog) const;

    UFUNCTION(NetMulticast, Reliable, Category = "Skill|Execution")
    void MulticastCastSkill(USkill* Skill);

    UFUNCTION(NetMulticast, Reliable, Category = "Skill|Execution")
    void MulticastCancelSkillCast(USkill* Skill);
	
    UFUNCTION(NetMulticast, Reliable, Category = "Skill|Execution")
    void MulticastActivateSkill(USkill* Skill);
	
    UFUNCTION(Client, Reliable, Category = "Skill|Execution")
    void ClientReceiveSkillPreCastValidationError(USkill* Skill, const FString& ErrorLog);

    UFUNCTION(Client, Reliable, Category = "Skill|Execution")
    void ClientReceiveSkillMidCastValidationError(USkill* Skill, const FString& ErrorLog);
	
    UFUNCTION(Client, Reliable, Category = "Skill|Execution")
    void ClientReceiveSkillPreActivationValidationError(USkill* Skill, const FString& ErrorLog);
};
