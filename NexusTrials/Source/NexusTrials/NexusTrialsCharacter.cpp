// Copyright Epic Games, Inc. All Rights Reserved.
#include "NexusTrialsCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "NexusTrials.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

// Include ability classes for power-ups
#include "Abilities/VigorSeedAbility.h"
#include "Abilities/InfernoShardAbility.h"
#include "Abilities/AegisCarmAbility.h"

// Include the pure calculator helper
#include "Public/FFallDamageCalculator.h"


ANexusTrialsCharacter::ANexusTrialsCharacter()
{
    // Default health
    MaxHealth = 100.0f;

    // init fall damage timer
    FallDamageTimer = 0.0f;

    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // Don't rotate when the controller rotates. Let that just affect the camera.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 500.f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 500.f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
    GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

    // Create attribute component for health management
    AttributeComponent = CreateDefaultSubobject<UNexusAttributeComponent>(TEXT("AttributeComponent"));
    AttributeComponent->SetMaxHealth(MaxHealth);

    // Create ability component for ability management
    AbilityComponent = CreateDefaultSubobject<UNexusAbilityComponent>(TEXT("AbilityComponent"));

    // Create a camera boom
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.0f;
    CameraBoom->bUsePawnControlRotation = true;

    // Create a follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;
}

void ANexusTrialsCharacter::BeginPlay()
{
    Super::BeginPlay();

    // Debug camera setup
    if (CameraBoom)
    {
        UE_LOG(LogTemp, Warning, TEXT("Camera Debug - TargetArmLength: %.2f, DoCollisionTest: %d"), 
            CameraBoom->TargetArmLength, CameraBoom->bDoCollisionTest);
    }
}

void ANexusTrialsCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Use the pure, testable fall-damage calculator
    float Damage = FFallDamageCalculator::AccumulateAndComputeDamage(GetVelocity().Z, DeltaTime, FallDamageTimer);
    if (Damage > 0.0f)
    {
        TakeDamage(Damage);
    }
}

void ANexusTrialsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UE_LOG(LogTemp, Warning, TEXT("SetupPlayerInputComponent called — InputComponent: %s"), *GetNameSafe(PlayerInputComponent));

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        UE_LOG(LogTemp, Warning, TEXT("EnhancedInputComponent cast succeeded — binding actions"));

        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ANexusTrialsCharacter::Move);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ANexusTrialsCharacter::Look);

        UE_LOG(LogTemp, Warning, TEXT("All bindings complete"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("EnhancedInputComponent cast FAILED — using legacy input"));
    }
}

void ANexusTrialsCharacter::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();
    
    // DIAGNOSTIC LOGGING - Remove after fixing input issues
    UE_LOG(LogTemp, Warning, TEXT("Move() called - Input Vector: X=%.2f, Y=%.2f"), MovementVector.X, MovementVector.Y);

    if (Controller != nullptr)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        UE_LOG(LogTemp, Warning, TEXT("  Forward Dir: %s, Right Dir: %s"), *ForwardDirection.ToString(), *RightDirection.ToString());
        UE_LOG(LogTemp, Warning, TEXT("  Adding Forward: %.2f, Right: %.2f"), MovementVector.Y, MovementVector.X);

        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void ANexusTrialsCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

float ANexusTrialsCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    // Star power blocks ALL damage (legacy system - will be moved to ability in next phase)
    if (bHasStar && DamageAmount > 0.f)
    {
        UE_LOG(LogTemp, Log, TEXT("Star invincibility blocked %.1f damage!"), DamageAmount);
        return 0.f;
    }

    // Delegate to AttributeComponent for actual damage handling
    if (AttributeComponent)
    {
        float ActualDamage = AttributeComponent->TakeDamage(DamageAmount);
        
        // Log if character died
        if (AttributeComponent->IsDead())
        {
            UE_LOG(LogNexusTrials, Warning, TEXT("Player died from %.1f damage!"), ActualDamage);
        }
        
        return ActualDamage;
    }

    return 0.f;
}

void ANexusTrialsCharacter::TakeDamage(float Damage)
{
    FDamageEvent DamageEvent;
    TakeDamage(Damage, DamageEvent, nullptr, nullptr);
}
void ANexusTrialsCharacter::Die()
{
    // Simple death: respawn or game over
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        DisableInput(PC);
    }

    // Or just destroy for testing
    Destroy();
}

void ANexusTrialsCharacter::AddCollection(int32 Amount)
{
    CollectionCount += Amount;
    UE_LOG(LogTemp, Warning, TEXT("Collection! Now %d"), CollectionCount);

    if (CollectionCount >= 3 && !bGotOneUpThisLevel)
    {
        GrantExtraLife();
        bGotOneUpThisLevel = true;
    }
}

void ANexusTrialsCharacter::GrantExtraLife()
{
    UE_LOG(LogTemp, Warning, TEXT("★★★ 1-UP! Extra life granted! ★★★"));
    // In a real game: Lives++, play sound, VFX, HUD flash, etc.
}

void ANexusTrialsCharacter::EndStarInvincibility()
{
    if (bHasStar)
    {
        bHasStar = false;
        CurrentPowerUpState = EPowerUpState::Small; // or save previous state if you want
        UE_LOG(LogTemp, Warning, TEXT("Star power ended — vulnerable again!"));
    }
}

void ANexusTrialsCharacter::ResetCollections()
{
    CollectionCount = 0;
    bGotOneUpThisLevel = false;
}

void ANexusTrialsCharacter::ApplyPowerUp(EPowerUpState NewState)
{
    if (!AbilityComponent)
    {
        return;
    }

    CurrentPowerUpState = NewState;

    // Deactivate current power-ups before applying new one
    bHasMushroom = false;
    bHasFireFlower = false;
    bHasStar = false;

    switch (NewState)
    {
        case EPowerUpState::VigorSeed:
        {
            // Add and activate Vigor Seed ability
            if (!AbilityComponent->GetAbility(UVigorSeedAbility::StaticClass()))
            {
                AbilityComponent->AddAbility(UVigorSeedAbility::StaticClass());
            }
            AbilityComponent->ActivateAbility(UVigorSeedAbility::StaticClass());
            bHasMushroom = true;
            break;
        }
        case EPowerUpState::InfernoShard:
        {
            // Add and activate Inferno Shard ability
            if (!AbilityComponent->GetAbility(UInfernoShardAbility::StaticClass()))
            {
                AbilityComponent->AddAbility(UInfernoShardAbility::StaticClass());
            }
            AbilityComponent->ActivateAbility(UInfernoShardAbility::StaticClass());
            bHasFireFlower = true;
            break;
        }
        case EPowerUpState::AegisCharm:
        {
            // Add and activate Aegis Charm ability
            if (!AbilityComponent->GetAbility(UAegisCarmAbility::StaticClass()))
            {
                AbilityComponent->AddAbility(UAegisCarmAbility::StaticClass());
            }
            AbilityComponent->ActivateAbility(UAegisCarmAbility::StaticClass());
            bHasStar = true;
            break;
        }
        case EPowerUpState::Small:
        default:
        {
            // Return to normal state - no special ability
            break;
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Power-up applied: %s"), *UEnum::GetValueAsString(NewState));
}

void ANexusTrialsCharacter::DoMove_Implementation(float Forward, float Right)
{
    // Default implementation - can be overridden in Blueprint
    if (Controller != nullptr)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(ForwardDirection, Forward);
        AddMovementInput(RightDirection, Right);
    }
}

void ANexusTrialsCharacter::DoLook_Implementation(float Pitch, float Yaw)
{
    // Default implementation - can be overridden in Blueprint
    if (Controller != nullptr)
    {
        AddControllerYawInput(Yaw);
        AddControllerPitchInput(Pitch);
    }
}

void ANexusTrialsCharacter::DoJumpStart_Implementation()
{
    // Default implementation - can be overridden in Blueprint
    Jump();
}

void ANexusTrialsCharacter::DoJumpEnd_Implementation()
{
    // Default implementation - can be overridden in Blueprint
    StopJumping();
}
