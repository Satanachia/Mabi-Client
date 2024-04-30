// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Mabinogi : ModuleRules
{
	public Mabinogi(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
        { 
            "Core", "CoreUObject", "Engine", "InputCore", 
            "Sockets", "Networking", "EnhancedInput" , "UMG", "Slate", "SlateCore","GameplayCameras"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { "ProtobufCore", "Slate", "SlateCore" });

        PrivateIncludePaths.AddRange(new string[]
        {
            "Mabinogi/",
            "Mabinogi/Network/",
        });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
