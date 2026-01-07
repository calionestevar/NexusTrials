#include "Abilities/NexusAbilityComponent.h"
#include "GameFramework/Pawn.h"

UNexusAbilityComponent::UNexusAbilityComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickInterval = 0.016f; // ~60 FPS
}

void UNexusAbilityComponent::BeginPlay()
{
    Super::BeginPlay();
    OwnerPawn = Cast<APawn>(GetOwner());
}

void UNexusAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Update all ability cooldowns
    for (UNexusAbility* Ability : Abilities)
    {
        if (Ability)
        {
            Ability->UpdateCooldown(DeltaTime);
        }
    }
}

UNexusAbility* UNexusAbilityComponent::AddAbility(TSubclassOf<UNexusAbility> AbilityClass)
{
    if (!AbilityClass || !OwnerPawn)
    {
        return nullptr;
    }

    // Don't add duplicate abilities
    if (GetAbility(AbilityClass))
    {
        return nullptr;
    }

    // Create the ability instance
    UNexusAbility* NewAbility = NewObject<UNexusAbility>(this, AbilityClass);
    if (NewAbility)
    {
        Abilities.Add(NewAbility);
        return NewAbility;
    }

    return nullptr;
}

UNexusAbility* UNexusAbilityComponent::GetAbility(TSubclassOf<UNexusAbility> AbilityClass) const
{
    if (!AbilityClass)
    {
        return nullptr;
    }

    for (UNexusAbility* Ability : Abilities)
    {
        if (Ability && Ability->IsA(AbilityClass))
        {
            return Ability;
        }
    }

    return nullptr;
}

UNexusAbility* UNexusAbilityComponent::GetAbilityByIndex(int32 Index) const
{
    if (Abilities.IsValidIndex(Index))
    {
        return Abilities[Index];
    }
    return nullptr;
}

bool UNexusAbilityComponent::ActivateAbility(TSubclassOf<UNexusAbility> AbilityClass, AActor* Target)
{
    UNexusAbility* Ability = GetAbility(AbilityClass);
    if (!Ability || !bAbilitiesEnabled)
    {
        return false;
    }

    return Ability->Activate(OwnerPawn, Target);
}

bool UNexusAbilityComponent::ActivateAbilityByIndex(int32 AbilityIndex, AActor* Target)
{
    UNexusAbility* Ability = GetAbilityByIndex(AbilityIndex);
    if (!Ability || !bAbilitiesEnabled)
    {
        return false;
    }

    return Ability->Activate(OwnerPawn, Target);
}

void UNexusAbilityComponent::SetAbilitiesEnabled(bool bEnabled)
{
    bAbilitiesEnabled = bEnabled;
}
