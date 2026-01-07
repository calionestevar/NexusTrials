#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NexusAttributeSet.generated.h"

/**
 * FNexusAttribute - Encapsulates an attribute value with modifiers
 * 
 * Attributes can have:
 * - Base Value: The core value (e.g., 100 health)
 * - Modifiers: Temporary changes (e.g., +20 from power-up, -50 from damage)
 * 
 * This pattern allows stacking and managing multiple effects on a single attribute.
 */
USTRUCT(BlueprintType)
struct NEXUSTRIALS_API FNexusAttribute
{
    GENERATED_BODY()

    /** Current value = base + all active modifiers */
    UPROPERTY(BlueprintReadOnly, Category = "Attributes")
    float CurrentValue = 0.0f;

    /** Base value before any modifiers */
    UPROPERTY(BlueprintReadOnly, Category = "Attributes")
    float BaseValue = 100.0f;

    /**
     * Get the effective value (base + modifiers)
     * @return Current effective value
     */
    float GetValue() const { return CurrentValue; }

    /**
     * Set base value and recalculate current value
     * @param NewBaseValue The new base value
     */
    void SetBaseValue(float NewBaseValue)
    {
        BaseValue = NewBaseValue;
        RecalculateCurrentValue();
    }

    /**
     * Apply a modifier to this attribute
     * @param ModifierAmount Amount to add/subtract
     */
    void ApplyModifier(float ModifierAmount)
    {
        CurrentValue += ModifierAmount;
    }

    /**
     * Recalculate current value from base
     * In a more complex system, this would iterate through all active modifiers
     */
    void RecalculateCurrentValue()
    {
        CurrentValue = BaseValue;
    }

    /**
     * Clamp value to a min/max range
     * @param MinValue Minimum allowed value
     * @param MaxValue Maximum allowed value
     */
    void Clamp(float MinValue, float MaxValue)
    {
        CurrentValue = FMath::Clamp(CurrentValue, MinValue, MaxValue);
    }
};

/**
 * UNexusAttributeSet - Defines all attributes a character can have
 * 
 * This is the single source of truth for character stats (Health, Damage, Speed, etc.)
 * Enables:
 * - Data-driven gameplay (can tune without recompiling)
 * - Clean separation of concerns (attributes separate from character class)
 * - Easy extensibility (add new attributes by extending this class)
 * - Testability (can test attribute changes independently)
 */
UCLASS(Blueprintable, BlueprintType)
class NEXUSTRIALS_API UNexusAttributeSet : public UObject
{
    GENERATED_BODY()

public:
    /** Health attribute - determines character life */
    UPROPERTY(BlueprintReadWrite, Category = "Attributes|Health")
    FNexusAttribute Health;

    /** Maximum health - hard cap for healing */
    UPROPERTY(BlueprintReadWrite, Category = "Attributes|Health")
    float MaxHealth = 100.0f;

    /** Damage attribute - outgoing damage multiplier */
    UPROPERTY(BlueprintReadWrite, Category = "Attributes|Combat")
    FNexusAttribute Damage;

    /** Movement speed multiplier */
    UPROPERTY(BlueprintReadWrite, Category = "Attributes|Movement")
    FNexusAttribute MovementSpeed;

    /** Initialize attributes with default values */
    virtual void Initialize();

    /**
     * Apply damage to health attribute
     * @param DamageAmount Amount of damage to apply
     * @return Actual damage applied (clamped to [0, CurrentHealth])
     */
    float TakeDamage(float DamageAmount);

    /**
     * Heal the character
     * @param HealAmount Amount to heal
     * @return Actual amount healed (clamped to MaxHealth)
     */
    float Heal(float HealAmount);

    /** Check if character is dead (health <= 0) */
    UFUNCTION(BlueprintCallable, Category = "Attributes")
    bool IsDead() const;

    /** Check if character is alive (health > 0) */
    UFUNCTION(BlueprintCallable, Category = "Attributes")
    bool IsAlive() const { return !IsDead(); }

    /** Get current health as percentage (0.0 - 1.0) */
    UFUNCTION(BlueprintCallable, Category = "Attributes")
    float GetHealthPercentage() const;
};
