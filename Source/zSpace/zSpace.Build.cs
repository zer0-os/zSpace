// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class zSpace : ModuleRules
{
	public zSpace(ReadOnlyTargetRules ROTargetRules) : base(ROTargetRules)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core"
            , "CoreUObject"
            , "Engine"
            , "InputCore"
            , "HeadMountedDisplay"
            , "GameplayTags"
            , "OWSPlugin"
            , "AIModule"
            , "Json"
            , "JsonUtilities"
			, "GameplayAbilities"
			, "GameplayTasks"
			, "UniversalVoiceChatPro"
			, "PhysicsCore"
            , "MediaAssets"
            , "Etherlinker"
            , "OVRPlugin"
            , "HTTP"
            , "HeadMountedDisplay"
            , "OculusHMD"
			});

		PrivateDependencyModuleNames.AddRange(new string[] { "OculusHMD" });

        PublicIncludePathModuleNames.AddRange(new string[] { "OWSPlugin" });

        // Uncomment if you are using Slate UI
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
        //Definitions.Add("AES_KEY=FF82B5451E21C090AE76622AAF24BCB0");
    }
}
