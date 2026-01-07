#pragma once

#include "CoreMinimal.h"
#include "Abilities/NexusAbility.h"
#include "MushroomeAbility.generated.h"

/**
 * MushroomeAbility - Grants temporary growth and increased health
 * 
 * Effects:
 * - Scales character up (mesh scale)
 * - Increases max health by 50
 * - Duration: until character takes damage or ability ends
 * 
 * Strategy: Ability pattern makes it trivial to add new power-ups
 * Just create new ability class, no core character code changes needed
 */
UCLASS()
class NEXUSTRIALS_API UMushroomeAbility : public UNexusAbility
{
    GENERATED_BODY()

public:
    UMushroomeAbility();

    /** Scale multiplier when ability is active */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mushroom")
    float ScaleMultiplier = 1.5f;

    /** Extra health granted */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mushroom")
    float HealthBonus = 50.0f;

protected:
    virtual bool OnActivate_Implementation(APawn* Instigator, AActor* Target) override;
    virtual void OnDeactivate_Implementation(APawn* Instigator) override;

private:
    /** Store the original scale for restoration */
    FVector OriginalScale = FVector::One();

    /** Store the original max health for restoration */
    float OriginalMaxHealth = 100.0f;
};
