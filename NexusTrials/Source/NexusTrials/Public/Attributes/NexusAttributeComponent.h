#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Attributes/NexusAttributeSet.h"
#include "NexusAttributeComponent.generated.h"

/**
 * Delegate fired when attribute changes
 * Parameters: NewValue, OldValue, DeltaValue
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChanged, 
    float, NewValue, float, OldValue, float, DeltaValue);

/**
 * Delegate fired when health reaches zero
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterDeath);

/**
 * UNexusAttributeComponent - Manages attributes for a character
 * 
 * Responsibility:
 * - Store and manage attribute set
 * - Broadcast changes so other systems can react
 * - Handle special cases (death, health thresholds)
 * 
 * Pattern: Component is simpler than having all logic in Character class
 * Benefits:
 * - Character class stays lean
 * - Multiple characters can share attribute logic
 * - Testable independently
 */
UCLASS(ClassGroup = (Custom), Meta = (BlueprintSpawnableComponent))
class NEXUSTRIALS_API UNexusAttributeComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UNexusAttributeComponent();

    /** Initialize with default attributes */
    virtual void BeginPlay() override;

    //================== Attribute Access ==================

    /** Get the attribute set */
    UFUNCTION(BlueprintCallable, Category = "Attributes")
    UNexusAttributeSet* GetAttributeSet() const { return AttributeSet; }

    /** Get current health */
    UFUNCTION(BlueprintCallable, Category = "Attributes|Health")
    float GetHealth() const;

    /** Get max health */
    UFUNCTION(BlueprintCallable, Category = "Attributes|Health")
    float GetMaxHealth() const;

    /** Get health as percentage (0.0 - 1.0) */
    UFUNCTION(BlueprintCallable, Category = "Attributes|Health")
    float GetHealthPercentage() const;

    /** Check if character is dead */
    UFUNCTION(BlueprintCallable, Category = "Attributes|Health")
    bool IsDead() const;

    /** Check if character is alive */
    UFUNCTION(BlueprintCallable, Category = "Attributes|Health")
    bool IsAlive() const { return !IsDead(); }

    //================== Damage/Healing ==================

    /**
     * Apply damage to this character
     * Broadcasts OnHealthChanged and OnCharacterDeath if health <= 0
     * 
     * @param DamageAmount Amount of damage to apply
     * @return Actual damage applied
     */
    UFUNCTION(BlueprintCallable, Category = "Attributes|Health")
    float TakeDamage(float DamageAmount);

    /**
     * Heal this character
     * Broadcasts OnHealthChanged
     * 
     * @param HealAmount Amount to heal
     * @return Actual amount healed
     */
    UFUNCTION(BlueprintCallable, Category = "Attributes|Health")
    float Heal(float HealAmount);

    //================== Events ==================

    /** Fired whenever health changes */
    UPROPERTY(BlueprintAssignable, Category = "Attributes|Events")
    FOnAttributeChanged OnHealthChanged;

    /** Fired when health reaches zero */
    UPROPERTY(BlueprintAssignable, Category = "Attributes|Events")
    FOnCharacterDeath OnCharacterDeath;

    //================== Initialization ==================

    /**
     * Set max health and reinitialize
     * @param NewMaxHealth The new maximum health value
     */
    UFUNCTION(BlueprintCallable, Category = "Attributes|Health")
    void SetMaxHealth(float NewMaxHealth);

protected:
    /** The attribute set for this character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
    UNexusAttributeSet* AttributeSet;

    /** Track previous health for delta calculation */
    float PreviousHealth = 0.0f;
};
