#include "Attributes/NexusAttributeSet.h"

void UNexusAttributeSet::Initialize()
{
    Health.SetBaseValue(MaxHealth);
    Health.CurrentValue = MaxHealth;
    
    Damage.SetBaseValue(10.0f);
    Damage.CurrentValue = 10.0f;
    
    MovementSpeed.SetBaseValue(1.0f);
    MovementSpeed.CurrentValue = 1.0f;
}

float UNexusAttributeSet::TakeDamage(float DamageAmount)
{
    if (DamageAmount <= 0.0f)
    {
        return 0.0f;
    }

    // Calculate actual damage (before death)
    float ActualDamage = FMath::Min(DamageAmount, Health.CurrentValue);
    Health.CurrentValue -= ActualDamage;

    // Clamp to prevent negative health
    Health.Clamp(0.0f, MaxHealth);

    return ActualDamage;
}

float UNexusAttributeSet::Heal(float HealAmount)
{
    if (HealAmount <= 0.0f || IsDead())
    {
        return 0.0f;
    }

    // Calculate actual heal (before max health)
    float ActualHeal = FMath::Min(HealAmount, MaxHealth - Health.CurrentValue);
    Health.CurrentValue += ActualHeal;

    // Clamp to max health
    Health.Clamp(0.0f, MaxHealth);

    return ActualHeal;
}

bool UNexusAttributeSet::IsDead() const
{
    return Health.CurrentValue <= 0.0f;
}

float UNexusAttributeSet::GetHealthPercentage() const
{
    if (MaxHealth <= 0.0f)
    {
        return 0.0f;
    }
    return Health.CurrentValue / MaxHealth;
}
