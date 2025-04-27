#pragma once

#include "Components/ActorComponent.h"
#include "EnhancedInputComponent.h"
#include "Skill.h"
#include "SkillGlobals.h"
#include "SkillComponent.generated.h"

class USkillEffect;
class USkill;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillActivated, USkill*, Skill);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillTerminated, USkill*, Skill, ESkillTerminationType, TerminationType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillValidationError, USkill*, Skill, const FString&, ErrorLog);

UCLASS(Blueprintable)
class SKILLSYSTEM_API USkillComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnSkillActivated OnSkillActivated;

    UPROPERTY(BlueprintAssignable)
    FOnSkillTerminated OnSkillTerminated;

    UPROPERTY(BlueprintAssignable)
    FOnSkillValidationError OnSkillExecutionFailed;
    
    UPROPERTY(BlueprintAssignable)
    FOnSkillValidationError OnSkillCastFailed;
    
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
    UFUNCTION(BlueprintPure, Category = "Online")
    bool HasAuthority() const { return GetOwnerRole() == ROLE_Authority; }

    UFUNCTION(BlueprintPure, Category = "Online")
    bool IsServer() const { return GetNetMode() <= NM_ListenServer; }

    UFUNCTION(BlueprintPure, Category = "Online")
    bool IsClient() const { return GetNetMode() >= NM_ListenServer; }
    
    // Returns true if the owning pawn is locally controlled.
    UFUNCTION(BlueprintPure, Category = "Online")
    bool IsOwnerLocallyControlled() const;
    
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

    UFUNCTION(BlueprintPure, Category = "Debug")
    FString GetOwnerControlTypeAsString() const;

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void OnRegister() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void TickComponent(const float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(Client, Reliable, Category = "Skill|Execution")
    void ClientOnSkillExecutionFailed(USkill* Skill, const FString& ErrorLog);
    
    UFUNCTION(Client, Reliable, Category = "Skill|Execution")
    void ClientOnSkillCastFailed(USkill* Skill, const FString& ErrorLog);
};
