#include "Abilities/MushroomeAbility.h"
#include "NexusTrialsCharacter.h"
#include "Attributes/NexusAttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UMushroomeAbility::UMushroomeAbility()
{
    AbilityName = TEXT("Mushroom Power-Up");
    CooldownDuration = 0.0f;  // No cooldown for power-ups
    DamageAmount = 0.0f;       // Doesn't deal damage
}

bool UMushroomeAbility::OnActivate_Implementation(APawn* Instigator, AActor* Target)
{
    ANexusTrialsCharacter* Character = Cast<ANexusTrialsCharacter>(Instigator);
    if (!Character)
    {
        return false;
    }

    // Store original values for restoration
    OriginalScale = Character->GetActorScale3D();
    
    UNexusAttributeComponent* AttrComp = Character->GetAttributeComponent();
    if (AttrComp)
    {
        OriginalMaxHealth = AttrComp->GetMaxHealth();
        
        // Increase max health and heal to full
        float NewMaxHealth = OriginalMaxHealth + HealthBonus;
        AttrComp->SetMaxHealth(NewMaxHealth);
        AttrComp->Heal(HealthBonus);
    }

    // Scale up the character mesh
    FVector NewScale = OriginalScale * ScaleMultiplier;
    Character->SetActorScale3D(NewScale);

    UE_LOG(LogTemp, Warning, TEXT("🍄 Mushroom Power-Up Activated! Scale: %.1f, Health: +%.0f"), ScaleMultiplier, HealthBonus);

    return true;
}

void UMushroomeAbility::OnDeactivate_Implementation(APawn* Instigator)
{
    ANexusTrialsCharacter* Character = Cast<ANexusTrialsCharacter>(Instigator);
    if (!Character)
    {
        return;
    }

    // Restore original scale
    Character->SetActorScale3D(OriginalScale);

    // Restore original max health
    UNexusAttributeComponent* AttrComp = Character->GetAttributeComponent();
    if (AttrComp)
    {
        AttrComp->SetMaxHealth(OriginalMaxHealth);
    }

    UE_LOG(LogTemp, Warning, TEXT("🍄 Mushroom Power-Up Deactivated!"));
}
