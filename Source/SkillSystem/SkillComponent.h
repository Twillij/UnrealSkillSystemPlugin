﻿#pragma once

#include "CoreMinimal.h"
#include "SkillGlobals.h"
#include "Components/ActorComponent.h"
#include "SkillComponent.generated.h"

class UInputAction;
class USkill;

UCLASS(Blueprintable)
class SKILLSYSTEM_API USkillComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Intended for modifying on the client only, e.g. to load skills from the client's save file.
    // Will be sent to and processed by the server whenever ClientUploadSkillData() is called.
    // Afterwards, a copy of the data will be stored on the server for caching purposes only.
    UPROPERTY(BlueprintReadWrite, Category = "Skill|Network")
    TArray<FSkillData> ClientSkillData;
    
protected:
    // Skill data used to create the initial array of skills
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FSkillData> PresetSkills;
    
    // A replicated array of unique skills
    UPROPERTY(Replicated)
    TArray<USkill*> Skills;

public:
    USkillComponent();
    
    UFUNCTION(BlueprintPure, Category = "Component")
    APawn* GetOwningPawn() const { return Cast<APawn>(GetOwner()); }

    UFUNCTION(BlueprintPure, Category = "Component")
    AController* GetOwningController() const;

    // Returns true if the owner has network authority.
    UFUNCTION(BlueprintPure, Category = "Network")
    bool HasAuthority() const { return GetOwnerRole() == ROLE_Authority; }

    UFUNCTION(BlueprintPure, Category = "Network")
    bool IsLocallyControlled() const;

    // Returns all the registered skills.
    UFUNCTION(BlueprintPure, Category = "Skill")
    TArray<USkill*> GetAllSkills() { return Skills; }

    // Returns a registered skill of the specified class if found, null otherwise.
    UFUNCTION(BlueprintPure, Category = "Skill")
    USkill* GetSkillOfClass(TSubclassOf<USkill> SkillClass);

    // Registers a unique skill instance. Returns true if successful.
    UFUNCTION(BlueprintCallable, Category = "Skill")
    bool RegisterSkill(USkill* Skill);

    // De-registers a skill instance. Returns true if successful.
    UFUNCTION(BlueprintCallable, Category = "Skill")
    bool DeregisterSkill(USkill* Skill);

    // Processes a skill data and either updates an existing skill or creates a new one.
    UFUNCTION(BlueprintCallable, Category = "Skill")
    void ProcessSkillData(const FSkillData& InData);
    
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
};
