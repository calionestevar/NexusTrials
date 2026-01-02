// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NexusTrialsGameMode.generated.h"

/**
 *  Simple GameMode for a third person game
 */
UCLASS(abstract)
class ANexusTrialsGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	/** Constructor */
	ANexusTrialsGameMode();

	/** Called when the game begins */
	virtual void BeginPlay() override;
	
	/** Tick to update live dashboard */
	virtual void Tick(float DeltaTime) override;
};



