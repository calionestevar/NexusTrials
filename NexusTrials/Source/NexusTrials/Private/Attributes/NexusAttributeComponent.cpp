#include "Attributes/NexusAttributeComponent.h"

UNexusAttributeComponent::UNexusAttributeComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    
    // Create the attribute set
    AttributeSet = CreateDefaultSubobject<UNexusAttributeSet>(TEXT("AttributeSet"));
}

void UNexusAttributeComponent::BeginPlay()
{
    Super::BeginPlay();

    if (AttributeSet)
    {
        AttributeSet->Initialize();
        PreviousHealth = AttributeSet->Health.CurrentValue;
    }
}

float UNexusAttributeComponent::GetHealth() const
{
    return AttributeSet ? AttributeSet->Health.CurrentValue : 0.0f;
}

float UNexusAttributeComponent::GetMaxHealth() const
{
    return AttributeSet ? AttributeSet->MaxHealth : 0.0f;
}

float UNexusAttributeComponent::GetHealthPercentage() const
{
    return AttributeSet ? AttributeSet->GetHealthPercentage() : 0.0f;
}

bool UNexusAttributeComponent::IsDead() const
{
    return AttributeSet ? AttributeSet->IsDead() : false;
}

float UNexusAttributeComponent::TakeDamage(float DamageAmount)
{
    if (!AttributeSet || IsDead())
    {
        return 0.0f;
    }

    PreviousHealth = AttributeSet->Health.CurrentValue;
    float ActualDamage = AttributeSet->TakeDamage(DamageAmount);

    if (ActualDamage > 0.0f)
    {
        float Delta = AttributeSet->Health.CurrentValue - PreviousHealth;
        OnHealthChanged.Broadcast(AttributeSet->Health.CurrentValue, PreviousHealth, Delta);

        // Check if we died
        if (IsDead())
        {
            OnCharacterDeath.Broadcast();
        }
    }

    return ActualDamage;
}

float UNexusAttributeComponent::Heal(float HealAmount)
{
    if (!AttributeSet || IsDead())
    {
        return 0.0f;
    }

    PreviousHealth = AttributeSet->Health.CurrentValue;
    float ActualHeal = AttributeSet->Heal(HealAmount);

    if (ActualHeal > 0.0f)
    {
        float Delta = AttributeSet->Health.CurrentValue - PreviousHealth;
        OnHealthChanged.Broadcast(AttributeSet->Health.CurrentValue, PreviousHealth, Delta);
    }

    return ActualHeal;
}

void UNexusAttributeComponent::SetMaxHealth(float NewMaxHealth)
{
    if (AttributeSet && NewMaxHealth > 0.0f)
    {
        AttributeSet->MaxHealth = NewMaxHealth;
        AttributeSet->Initialize();
        PreviousHealth = AttributeSet->Health.CurrentValue;
    }
}
