// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Grumblin.generated.h"

class ANexusTrialsCharacter;

/**
 * AGrumblin: A basic ground-dwelling enemy inspired by the Goomba from Mario series.
 * Represents a small, attacking creature that can harm the player.
 * Killed by jumping on it or using power-ups.
 */
UCLASS()
class NEXUSTRIALS_API AGrumblin : public ACharacter
{
	GENERATED_BODY()

public:
	/** Constructor */
	AGrumblin();

	/** Enable per-frame ticking */
	virtual void Tick(float DeltaTime) override;

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Handle collision with player */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
