// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NexusTrials : ModuleRules
{
        public NexusTrials(ReadOnlyTargetRules Target) : base(Target)
        {
                PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

                PublicDependencyModuleNames.AddRange(new string[] {
                        "Core",
                        "CoreUObject",
                        "Engine",
                        "InputCore",
                        "EnhancedInput",
                        "AIModule",
                        "StateTreeModule",
                        "GameplayStateTreeModule",
                        "UMG",
                        "Slate",
                        "Nexus",
                        "ArgusLens",
                        "FringeNetwork",
                        "Protego",
                        "StargateStress",
                        "Utilities"
                });

                PrivateDependencyModuleNames.AddRange(new string[] { });

                PublicIncludePaths.AddRange(new string[] {
                        "NexusTrials",
                        "NexusTrials/Variant_Platforming",
                        "NexusTrials/Variant_Platforming/Animation",
                        "NexusTrials/Variant_Combat",
                        "NexusTrials/Variant_Combat/AI",
                        "NexusTrials/Variant_Combat/Animation",
                        "NexusTrials/Variant_Combat/Gameplay",
                        "NexusTrials/Variant_Combat/Interfaces",
                        "NexusTrials/Variant_Combat/UI",
                        "NexusTrials/Variant_SideScrolling",
                        "NexusTrials/Variant_SideScrolling/AI",
                        "NexusTrials/Variant_SideScrolling/Gameplay",
                        "NexusTrials/Variant_SideScrolling/Interfaces",
                        "NexusTrials/Variant_SideScrolling/UI"
                });

                // Uncomment if you are using Slate UI
                // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

                // Uncomment if you are using online features
                // PrivateDependencyModuleNames.Add("OnlineSubsystem");

                // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
        }
}
