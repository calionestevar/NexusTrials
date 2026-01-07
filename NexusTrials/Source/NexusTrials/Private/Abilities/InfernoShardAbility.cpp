#include "Abilities/InfernoShardAbility.h"
#include "NexusTrialsCharacter.h"
#include "Attributes/NexusAttributeComponent.h"

UInfernoShardAbility::UInfernoShardAbility()
{
    AbilityName = TEXT("Inferno Shard Power-Up");
    CooldownDuration = 0.0f;
    DamageAmount = 1.5f;  // Base damage for fire attacks
}

bool UInfernoShardAbility::OnActivate_Implementation(APawn* Instigator, AActor* Target)
{
    ANexusTrialsCharacter* Character = Cast<ANexusTrialsCharacter>(Instigator);
    if (!Character)
    {
        return false;
    }

    // Store original damage
    UNexusAttributeComponent* AttrComp = Character->GetAttributeComponent();
    if (AttrComp && AttrComp->GetAttributeSet())
    {
        OriginalDamage = AttrComp->GetAttributeSet()->Damage.GetValue();
        
        // Apply damage multiplier
        float NewDamage = OriginalDamage * DamageMultiplier;
        AttrComp->GetAttributeSet()->Damage.SetBaseValue(NewDamage);
    }

    UE_LOG(LogTemp, Warning, TEXT("🔥 Inferno Shard Power-Up Activated! Damage: x%.1f"), DamageMultiplier);

    return true;
}

void UInfernoShardAbility::OnDeactivate_Implementation(APawn* Instigator)
{
    ANexusTrialsCharacter* Character = Cast<ANexusTrialsCharacter>(Instigator);
    if (!Character)
    {
        return;
    }

    // Restore original damage
    UNexusAttributeComponent* AttrComp = Character->GetAttributeComponent();
    if (AttrComp && AttrComp->GetAttributeSet())
    {
        AttrComp->GetAttributeSet()->Damage.SetBaseValue(OriginalDamage);
    }

    UE_LOG(LogTemp, Warning, TEXT("🔥 Inferno Shard Power-Up Deactivated!"));
}
