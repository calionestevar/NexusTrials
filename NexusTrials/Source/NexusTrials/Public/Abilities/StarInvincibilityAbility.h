#pragma once

#include "CoreMinimal.h"
#include "Abilities/NexusAbility.h"
#include "StarInvincibilityAbility.generated.h"

class ANexusTrialsCharacter;

/**
 * StarInvincibilityAbility - Grants temporary invincibility
 * 
 * Effects:
 * - Character becomes invincible (blocks all damage)
 * - Timed duration (default 10 seconds)
 * - Auto-deactivates after duration or on manual end
 * 
 * Design: Shows how abilities can be time-limited
 * Future: Could add visual effects (glow), audio cues, particle effects
 */
UCLASS()
class NEXUSTRIALS_API UStarInvincibilityAbility : public UNexusAbility
{
    GENERATED_BODY()

public:
    UStarInvincibilityAbility();

    /** Duration of invincibility in seconds */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StarInvincibility", meta = (ClampMin = 1, ClampMax = 60))
    float InvincibilityDuration = 10.0f;

    /**
     * End the invincibility effect early
     * Called when duration expires or character manually ends it
     */
    void EndInvincibility(APawn* Instigator);

protected:
    virtual bool OnActivate_Implementation(APawn* Instigator, AActor* Target) override;
    virtual void OnDeactivate_Implementation(APawn* Instigator) override;

private:
    /** Timer handle for tracking invincibility duration */
    FTimerHandle InvincibilityTimerHandle;

    /** Weak reference to prevent invalid pointer access */
    TWeakObjectPtr<APawn> InstigatorRef;

    /**
     * Internal callback when duration expires
     */
    void OnInvincibilityExpired();
};
