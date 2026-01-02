// Copyright Epic Games, Inc. All Rights Reserved.

#include "NexusTrialsGameMode.h"
#include "FringeNetwork/Public/FringeNetwork.h"
#include "FringeNetwork/Public/ObserverNetworkDashboard.h"
#include "Modules/ModuleManager.h"

ANexusTrialsGameMode::ANexusTrialsGameMode()
{
	// stub
}

void ANexusTrialsGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Check if tests are running (Nexus framework sets this during test execution)
	// Skip full observer network in test mode to avoid Palantir HTTP issues
	bool bIsTestMode = FParse::Param(FCommandLine::Get(), TEXT("TestMode")) || 
	                   FModuleManager::Get().IsModuleLoaded("Nexus");
	
	if (!bIsTestMode)
	{
		// Activate observer network monitoring for real-time network testing
		// and safety event tracking
		UFringeNetwork::ActivateObserverNetwork();
		UE_LOG(LogTemp, Warning, TEXT("🔍 OBSERVER NETWORK ACTIVATED — Safety monitoring is now online"));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("⚠️ Test mode detected — Skipping observer network HTTP tracing"));
	}
	
	// Initialize observer dashboard for live event tracking
	UObserverNetworkDashboard::Initialize();
}

void ANexusTrialsGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Update live dashboard every frame (ImGui rendering)
	UObserverNetworkDashboard::UpdateLiveDashboard();
}
