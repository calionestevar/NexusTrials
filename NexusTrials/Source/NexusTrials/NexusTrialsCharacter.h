// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Attributes/NexusAttributeComponent.h"
#include "Abilities/NexusAbilityComponent.h"
#include "NexusTrialsCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

// Power-up state enum
UENUM(BlueprintType)
enum class EPowerUpState : uint8
{
	Small = 0 UMETA(DisplayName = "Small"),
	VigorSeed = 1 UMETA(DisplayName = "Vigor Seed"),
	InfernoShard = 2 UMETA(DisplayName = "Inferno Shard"),
	AegisCharm = 3 UMETA(DisplayName = "Aegis Charm")
};

/**
 *  A player-controllable third person character with power-ups and health systems.
 *  Implements a controllable orbiting camera and game mechanics similar to Mario.
 *  Designed to showcase the NexusQA testing framework capabilities.
 */
UCLASS(abstract)
class NEXUSTRIALS_API ANexusTrialsCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** Attribute component manages health and stats */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UNexusAttributeComponent* AttributeComponent;

	/** Ability component manages all character abilities */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UNexusAbilityComponent* AbilityComponent;
	
protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

	//=== Health and Stats ===
	
	/** Maximum health the player can have (managed by AttributeComponent) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health", meta = (ClampMin = 1, ClampMax = 200))
	float MaxHealth = 100.0f;

	/** Fall damage timer for accumulating damage */
	float FallDamageTimer = 0.0f;

	//=== Power-ups ===
	
	/** Current power-up state */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PowerUps")
	EPowerUpState CurrentPowerUpState = EPowerUpState::Small;

	/** Whether player has mushroom power-up */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PowerUps")
	bool bHasMushroom = false;

	/** Whether player has fire flower power-up */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PowerUps")
	bool bHasFireFlower = false;

	/** Whether player has star invincibility */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PowerUps")
	bool bHasStar = false;

	/** Duration of star invincibility in seconds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PowerUps", meta = (ClampMin = 1, ClampMax = 60))
	float StarInvincibilityDuration = 10.0f;

	/** Timer handle for star invincibility */
	FTimerHandle StarTimerHandle;

	//=== Collectibles ===
	
	/** Number of collectibles collected this level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Collectibles")
	int32 CollectionCount = 0;

	/** Whether player got 1-UP bonus this level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Collectibles")
	bool bGotOneUpThisLevel = false;

public:

	/** Constructor */
	ANexusTrialsCharacter();

	/** Tick for physics and damage calculation */
	virtual void Tick(float DeltaTime) override;

	/** Set invincibility state (for abilities) */
	FORCEINLINE void SetInvincible(bool bNewInvincible) { bHasStar = bNewInvincible; }

	/** Get invincibility state (for abilities) */
	FORCEINLINE bool IsInvincible() const { return bHasStar; }

protected:

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** BeginPlay initialization */
	virtual void BeginPlay() override;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Blueprint event for movement - called when move input is received */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Input")
	void DoMove(float Forward, float Right);

	/** Blueprint event for look - called when look input is received */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Input")
	void DoLook(float Pitch, float Yaw);

	/** Blueprint event for jump start */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Input")
	void DoJumpStart();

	/** Blueprint event for jump end */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Input")
	void DoJumpEnd();

public:

	//=== Health Management ===

	/** Overridden TakeDamage with star invincibility support */
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	/** Simple take damage helper */
	UFUNCTION(BlueprintCallable, Category="Health")
	void TakeDamage(float Damage);

	/** Gets the current health */
	UFUNCTION(BlueprintCallable, Category="Health")
	FORCEINLINE float GetCurrentHealth() const 
	{ 
		return AttributeComponent ? AttributeComponent->GetHealth() : 0.0f;
	}

	/** Gets the maximum health */
	UFUNCTION(BlueprintCallable, Category="Health")
	FORCEINLINE float GetMaxHealth() const 
	{ 
		return AttributeComponent ? AttributeComponent->GetMaxHealth() : 0.0f;
	}

	//=== Death and Respawn ===

	/** Handles character death */
	UFUNCTION(BlueprintCallable, Category="Health")
	void Die();

	//=== Power-ups ===

	/** Apply a power-up to the character */
	UFUNCTION(BlueprintCallable, Category="PowerUps")
	void ApplyPowerUp(EPowerUpState NewState);

	/** End star invincibility effect */
	UFUNCTION(BlueprintCallable, Category="PowerUps")
	void EndStarInvincibility();

	//=== Collectibles ===

	/** Add collectible items */
	UFUNCTION(BlueprintCallable, Category="Collectibles")
	void AddCollection(int32 Amount);

	/** Grant an extra life */
	UFUNCTION(BlueprintCallable, Category="Collectibles")
	void GrantExtraLife();

	/** Reset collectibles for a new level */
	UFUNCTION(BlueprintCallable, Category="Collectibles")
	void ResetCollections();

	/** Gets the current power-up state */
	UFUNCTION(BlueprintCallable, Category="PowerUps")
	FORCEINLINE EPowerUpState GetPowerUpState() const { return CurrentPowerUpState; }

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** Returns AttributeComponent for health/stat management **/
	FORCEINLINE UNexusAttributeComponent* GetAttributeComponent() const { return AttributeComponent; }

	/** Returns AbilityComponent for ability management **/
	FORCEINLINE UNexusAbilityComponent* GetAbilityComponent() const { return AbilityComponent; }
};



