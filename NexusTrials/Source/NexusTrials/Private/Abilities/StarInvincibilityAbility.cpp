#include "Abilities/StarInvincibilityAbility.h"
#include "NexusTrialsCharacter.h"
#include "TimerManager.h"
#include "Engine/World.h"

UStarInvincibilityAbility::UStarInvincibilityAbility()
{
    AbilityName = TEXT("Star Invincibility");
    CooldownDuration = 0.0f;
    InvincibilityDuration = 10.0f;
}

bool UStarInvincibilityAbility::OnActivate_Implementation(APawn* Instigator, AActor* Target)
{
    ANexusTrialsCharacter* Character = Cast<ANexusTrialsCharacter>(Instigator);
    if (!Character)
    {
        return false;
    }

    InstigatorRef = Instigator;

    // Set star flag (for legacy damage blocking code)
    Character->bHasStar = true;

    UE_LOG(LogTemp, Warning, TEXT("⭐ Star Invincibility Activated! Duration: %.1f seconds"), InvincibilityDuration);

    // Schedule the end of invincibility
    if (UWorld* World = Character->GetWorld())
    {
        World->GetTimerManager().SetTimer(
            InvincibilityTimerHandle,
            this,
            &UStarInvincibilityAbility::OnInvincibilityExpired,
            InvincibilityDuration,
            false
        );
    }

    return true;
}

void UStarInvincibilityAbility::OnDeactivate_Implementation(APawn* Instigator)
{
    ANexusTrialsCharacter* Character = Cast<ANexusTrialsCharacter>(Instigator);
    if (Character)
    {
        Character->bHasStar = false;
        UE_LOG(LogTemp, Warning, TEXT("⭐ Star Invincibility Deactivated!"));
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

void UStarInvincibilityAbility::OnInvincibilityExpired()
{
    // Duration expired - end the ability
    if (APawn* Instigator = InstigatorRef.Get())
    {
        EndInvincibility(Instigator);
    }
}

void UStarInvincibilityAbility::EndInvincibility(APawn* Instigator)
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
