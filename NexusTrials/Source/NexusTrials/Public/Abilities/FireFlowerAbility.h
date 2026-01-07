#pragma once

#include "CoreMinimal.h"
#include "Abilities/NexusAbility.h"
#include "FireFlowerAbility.generated.h"

/**
 * FireFlowerAbility - Grants ranged attack capability
 * 
 * Effects:
 * - Enables ranged fire projectile attacks
 * - Increased damage output
 * - Can be used multiple times (ability stays active)
 * 
 * Future Enhancement: Could spawn projectiles on fire input
 */
UCLASS()
class NEXUSTRIALS_API UFireFlowerAbility : public UNexusAbility
{
    GENERATED_BODY()

public:
    UFireFlowerAbility();

    /** Damage multiplier for ranged attacks */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireFlower")
    float DamageMultiplier = 2.0f;

    /** Projectile spawn offset from character */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireFlower")
    FVector ProjectileOffset = FVector(100.0f, 0.0f, 50.0f);

protected:
    virtual bool OnActivate_Implementation(APawn* Instigator, AActor* Target) override;
    virtual void OnDeactivate_Implementation(APawn* Instigator) override;

private:
    /** Store original damage for restoration */
    float OriginalDamage = 0.0f;
};
