#pragma once

#include "CoreMinimal.h"
#include "Abilities/NexusAbility.h"
#include "InfernoShardAbility.generated.h"

/**
 * InfernoShardAbility - Grants ranged attack capability with increased damage
 * 
 * Effects:
 * - Enables ranged fire projectile attacks
 * - Increased damage output (2x multiplier)
 * - Can be used multiple times (ability stays active)
 * 
 * Future Enhancement: Could spawn projectiles on fire input
 */
UCLASS()
class NEXUSTRIALS_API UInfernoShardAbility : public UNexusAbility
{
    GENERATED_BODY()

public:
    UInfernoShardAbility();

    /** Damage multiplier for ranged attacks */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InfernoShard")
    float DamageMultiplier = 2.0f;

    /** Projectile spawn offset from character */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InfernoShard")
    FVector ProjectileOffset = FVector(100.0f, 0.0f, 50.0f);

protected:
    virtual bool OnActivate_Implementation(APawn* Instigator, AActor* Target) override;
    virtual void OnDeactivate_Implementation(APawn* Instigator) override;

private:
    /** Store original damage for restoration */
    float OriginalDamage = 0.0f;
};
