#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Abilities/NexusAbility.h"
#include "NexusAbilityComponent.generated.h"

/**
 * UNexusAbilityComponent - Manages all abilities for a character
 * 
 * Responsibility:
 * - Hold and manage ability instances
 * - Handle ability activation with validation
 * - Update cooldowns every frame
 * - Enable/disable abilities based on character state
 * 
 * Benefits of component:
 * - Character class stays lean
 * - Easy to add/remove abilities at runtime
 * - Abilities are independent and reusable
 * - Fully testable
 */
UCLASS(ClassGroup = (Custom), Meta = (BlueprintSpawnableComponent))
class NEXUSTRIALS_API UNexusAbilityComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UNexusAbilityComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    //================== Ability Management ==================

    /**
     * Add an ability to this character
     * @param AbilityClass The ability class to create and add
     * @return Pointer to created ability, or nullptr if failed
     */
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    UNexusAbility* AddAbility(TSubclassOf<UNexusAbility> AbilityClass);

    /**
     * Get an ability by class
     * @param AbilityClass The class to search for
     * @return Pointer to ability, or nullptr if not found
     */
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    UNexusAbility* GetAbility(TSubclassOf<UNexusAbility> AbilityClass) const;

    /**
     * Get ability by index
     * @param Index Index in the abilities array
     * @return Pointer to ability, or nullptr if out of range
     */
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    UNexusAbility* GetAbilityByIndex(int32 Index) const;

    /** Get number of abilities */
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    int32 GetAbilityCount() const { return Abilities.Num(); }

    /** Get all abilities */
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    const TArray<UNexusAbility*>& GetAbilities() const { return Abilities; }

    //================== Ability Activation ==================

    /**
     * Attempt to activate an ability by class
     * @param AbilityClass The ability to activate
     * @param Target Optional target for the ability
     * @return true if activation succeeded
     */
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    bool ActivateAbility(TSubclassOf<UNexusAbility> AbilityClass, AActor* Target = nullptr);

    /**
     * Attempt to activate ability by index
     * @param AbilityIndex Index of ability to activate
     * @param Target Optional target for the ability
     * @return true if activation succeeded
     */
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    bool ActivateAbilityByIndex(int32 AbilityIndex, AActor* Target = nullptr);

    /**
     * Enable or disable all abilities based on character state
     * (e.g., disable when dead, stunned, in cutscene)
     * @param bEnabled true to enable, false to disable
     */
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void SetAbilitiesEnabled(bool bEnabled);

    /** Check if abilities are currently enabled */
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    bool AreAbilitiesEnabled() const { return bAbilitiesEnabled; }

protected:
    /** List of abilities this character has */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
    TArray<UNexusAbility*> Abilities;

    /** Whether abilities are enabled (used to block activation during death, cutscenes, etc.) */
    bool bAbilitiesEnabled = true;

private:
    /** Owner pawn cache for quick access */
    APawn* OwnerPawn = nullptr;
};
