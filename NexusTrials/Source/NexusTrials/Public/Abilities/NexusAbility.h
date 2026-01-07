#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NexusAbility.generated.h"

class UNexusAbilityComponent;
class AActor;
class APawn;

/**
 * Enum for ability state
 */
UENUM(BlueprintType)
enum class EAbilityState : uint8
{
    Idle       UMETA(DisplayName = "Not Active"),
    Active     UMETA(DisplayName = "Active"),
    OnCooldown UMETA(DisplayName = "On Cooldown"),
    Blocked    UMETA(DisplayName = "Blocked")
};

/**
 * UNexusAbility - Base class for all character abilities
 * 
 * Design Pattern: Strategy pattern for abilities
 * Each ability encapsulates its own logic, allowing easy addition/removal
 * 
 * Features:
 * - Cooldown tracking
 * - Can be blocked by character state (stunned, dead, etc.)
 * - Can do damage, apply effects, etc.
 * - Fully testable independently
 * 
 * Usage:
 * 1. Extend this class (e.g., FSlashAbility, DashAbility)
 * 2. Override OnActivate/OnDeactivate
 * 3. Add to character's AbilityComponent
 * 4. Component handles execution and cooldown
 */
UCLASS(Blueprintable, Abstract)
class NEXUSTRIALS_API UNexusAbility : public UObject
{
    GENERATED_BODY()

public:
    UNexusAbility();

    //================== Configuration ==================

    /** Display name for this ability */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    FString AbilityName;

    /** Cooldown in seconds before ability can be used again */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    float CooldownDuration = 0.5f;

    /** Cost to activate (health, mana, etc.) - 0 = free */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    float ActivationCost = 0.0f;

    /** Damage this ability deals (0 = no damage) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    float DamageAmount = 0.0f;

    /** Whether this ability requires a target */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    bool bRequiresTarget = false;

    //================== State ==================

    /** Get current state of this ability */
    UFUNCTION(BlueprintCallable, Category = "Ability")
    EAbilityState GetState() const { return CurrentState; }

    /** Check if ability can be activated right now */
    UFUNCTION(BlueprintCallable, Category = "Ability")
    bool CanActivate() const;

    /** Get remaining cooldown time (0 = ready) */
    UFUNCTION(BlueprintCallable, Category = "Ability")
    float GetRemainingCooldown() const;

    //================== Activation ==================

    /**
     * Activate this ability
     * Should be called by AbilityComponent
     * 
     * @param Instigator The pawn activating this ability
     * @param Target Optional target for this ability
     * @return true if activation succeeded
     */
    UFUNCTION(BlueprintCallable, Category = "Ability")
    bool Activate(APawn* Instigator, AActor* Target = nullptr);

    /**
     * Called when ability is activated
     * Override in subclasses to implement ability logic
     * 
     * @param Instigator The pawn using the ability
     * @param Target Optional target (if required)
     * @return true if activation successful
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ability")
    bool OnActivate(APawn* Instigator, AActor* Target);

    /**
     * Called when ability effect ends (after animation, after cooldown, etc.)
     * 
     * @param Instigator The pawn that used the ability
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ability")
    void OnDeactivate(APawn* Instigator);

    /** Reset cooldown (usually called externally for power-ups, debug) */
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void ResetCooldown();

    //================== Internal State ==================

protected:
    /** Current state of this ability */
    EAbilityState CurrentState = EAbilityState::Idle;

    /** Time when cooldown will end (0 = no cooldown active) */
    float CooldownEndTime = 0.0f;

    /**
     * Start cooldown
     * Called automatically after activation
     */
    void StartCooldown();

    /**
     * Update cooldown (called by component)
     */
    void UpdateCooldown(float DeltaTime);

    friend class UNexusAbilityComponent;
};
