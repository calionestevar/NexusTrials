#pragma once

#include "CoreMinimal.h"
#include "Abilities/NexusAbility.h"
#include "AegisCarmAbility.generated.h"

class ANexusTrialsCharacter;

/**
 * AegisCarmAbility - Grants temporary protection and invincibility
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
class NEXUSTRIALS_API UAegisCarmAbility : public UNexusAbility
{
    GENERATED_BODY()

public:
    UAegisCarmAbility();

    /** Duration of invincibility in seconds */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AegisCharm", meta = (ClampMin = 1, ClampMax = 60))
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

    /** Callback when invincibility duration expires */
    void OnInvincibilityExpired();

    /** Weak reference to instigator to prevent dangling pointers */
    TWeakObjectPtr<APawn> InstigatorRef;
};
