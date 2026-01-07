#include "Abilities/AegisCarmAbility.h"
#include "NexusTrialsCharacter.h"
#include "TimerManager.h"
#include "Engine/World.h"

UAegisCarmAbility::UAegisCarmAbility()
{
    AbilityName = TEXT("Aegis Charm");
    CooldownDuration = 0.0f;
    InvincibilityDuration = 10.0f;
}

bool UAegisCarmAbility::OnActivate_Implementation(APawn* Instigator, AActor* Target)
{
    ANexusTrialsCharacter* Character = Cast<ANexusTrialsCharacter>(Instigator);
    if (!Character)
    {
        return false;
    }

    InstigatorRef = Instigator;

    // Set invincibility flag (for legacy damage blocking code)
    Character->SetInvincible(true);

    UE_LOG(LogTemp, Warning, TEXT("✨ Aegis Charm Activated! Duration: %.1f seconds"), InvincibilityDuration);

    // Schedule the end of invincibility
    if (UWorld* World = Character->GetWorld())
    {
        World->GetTimerManager().SetTimer(
            InvincibilityTimerHandle,
            this,
            &UAegisCarmAbility::OnInvincibilityExpired,
            InvincibilityDuration,
            false
        );
    }

    return true;
}

void UAegisCarmAbility::OnDeactivate_Implementation(APawn* Instigator)
{
    ANexusTrialsCharacter* Character = Cast<ANexusTrialsCharacter>(Instigator);
    if (Character)
    {
        Character->SetInvincible(false);
        UE_LOG(LogTemp, Warning, TEXT("✨ Aegis Charm Deactivated!"));
    }

    // Clear the timer if still active
    if (APawn* Instigator_Ref = InstigatorRef.Get())
    {
        if (UWorld* World = Instigator_Ref->GetWorld())
        {
            World->GetTimerManager().ClearTimer(InvincibilityTimerHandle);
        }
    }
}

void UAegisCarmAbility::OnInvincibilityExpired()
{
    // Duration expired - end the ability
    if (APawn* Instigator = InstigatorRef.Get())
    {
        EndInvincibility(Instigator);
    }
}

void UAegisCarmAbility::EndInvincibility(APawn* Instigator)
{
    if (!Instigator)
    {
        return;
    }

    ANexusTrialsCharacter* Character = Cast<ANexusTrialsCharacter>(Instigator);
    if (Character)
    {
        Character->EndStarInvincibility();
    }

    OnDeactivate(Instigator);
}
