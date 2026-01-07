#include "Abilities/NexusAbility.h"
#include "GameFramework/Pawn.h"

UNexusAbility::UNexusAbility()
{
    AbilityName = TEXT("Unknown Ability");
}

bool UNexusAbility::CanActivate() const
{
    return GetRemainingCooldown() <= 0.0f && CurrentState != EAbilityState::Blocked;
}

float UNexusAbility::GetRemainingCooldown() const
{
    float Remaining = CooldownEndTime - FPlatformTime::Seconds();
    return FMath::Max(0.0f, Remaining);
}

bool UNexusAbility::Activate(APawn* Instigator, AActor* Target)
{
    // Validate activation conditions
    if (!CanActivate())
    {
        return false;
    }

    if (bRequiresTarget && !Target)
    {
        return false;
    }

    // Call the virtual method to let subclasses implement logic
    CurrentState = EAbilityState::Active;
    bool bSuccess = OnActivate(Instigator, Target);

    if (bSuccess)
    {
        StartCooldown();
        OnDeactivate(Instigator);
    }

    CurrentState = EAbilityState::OnCooldown;
    return bSuccess;
}

bool UNexusAbility::OnActivate_Implementation(APawn* Instigator, AActor* Target)
{
    // Default implementation does nothing
    // Subclasses should override this method
    return true;
}

void UNexusAbility::OnDeactivate_Implementation(APawn* Instigator)
{
    // Default implementation does nothing
}

void UNexusAbility::StartCooldown()
{
    CooldownEndTime = FPlatformTime::Seconds() + CooldownDuration;
}

void UNexusAbility::ResetCooldown()
{
    CooldownEndTime = 0.0f;
    CurrentState = EAbilityState::Idle;
}

void UNexusAbility::UpdateCooldown(float DeltaTime)
{
    // Update state based on cooldown time
    if (GetRemainingCooldown() <= 0.0f && CurrentState == EAbilityState::OnCooldown)
    {
        CurrentState = EAbilityState::Idle;
    }
}
