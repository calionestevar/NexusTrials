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

	// Initialize observer dashboard with Slate backend for live test monitoring
	// Slate is the native Unreal UI framework and always available (no plugin required)
	UObserverNetworkDashboard::Initialize(EDashboardBackend::Slate);
	UE_LOG(LogTemp, Display, TEXT("✅ Observer Network Dashboard initialized (Slate backend)"));

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
}

void ANexusTrialsGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Update live Slate-based dashboard every frame during test execution
	// Displays real-time test progress, pass/fail counts, and performance metrics
	UObserverNetworkDashboard::UpdateLiveDashboard();
}
