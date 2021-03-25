// Universal Cross-Platform Voice Chat MeoPlay Copyright (C) 2021 MeoPlay <contact@meoplay.com> All Rights Reserved.


using System;
using System.IO;
using UnrealBuildTool;
using System.Collections.Generic;

public class UniversalVoiceChatPro : ModuleRules
{
	public UniversalVoiceChatPro(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {

            }
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "AudioMixer",
                    "AudioCaptureCore",
                    "AudioCapture"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                   "AudioMixer",
                    "Voice",
                    "AudioCapture",
                     "AudioCaptureCore"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

        if (Target.Platform.IsInGroup(UnrealPlatformGroup.Windows))
        {
            AddEngineThirdPartyPrivateStaticDependencies(Target, "DirectSound");
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            PublicFrameworks.AddRange(new string[] { "CoreAudio", "AudioUnit", "AudioToolbox" });
        }
        else if (Target.IsInPlatformGroup(UnrealPlatformGroup.Linux))
        {
            AddEngineThirdPartyPrivateStaticDependencies(Target, "SDL2");
        }

        if (Target.Platform.IsInGroup(UnrealPlatformGroup.Windows) ||
               Target.Platform == UnrealTargetPlatform.Mac)
        {
            PrivateDependencyModuleNames.Add("AudioCaptureRtAudio");
        }
        else if (Target.Platform == UnrealTargetPlatform.PS4)
        {
            PrivateDependencyModuleNames.Add("AudioCaptureSony");
        }
        else if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            PrivateDependencyModuleNames.Add("AudioCaptureAudioUnit");
            PrivateDependencyModuleNames.Add("Core");
            PrivateDependencyModuleNames.Add("AudioCaptureCore");
            PublicFrameworks.AddRange(new string[] { "CoreAudio", "AVFoundation", "AudioToolbox" });
        }
        else if (Target.Platform == UnrealTargetPlatform.Android && Target.Platform != UnrealTargetPlatform.Lumin)
        {
            PrivateDependencyModuleNames.Add("AudioCaptureAndroid");
        }
        else if (Target.Platform == UnrealTargetPlatform.Switch)
        {
            PrivateDependencyModuleNames.Add("AudioCaptureSwitch");
        }

        AddEngineThirdPartyPrivateStaticDependencies(Target, "libOpus");

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PublicDependencyModuleNames.AddRange(new string[] { "Launch", "ApplicationCore" });
            string BuildPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
            AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(BuildPath, "MeoPlayVoiceChat_AndroidAPL.xml"));
        }

        if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            PrivateDependencyModuleNames.Add("IOSRuntimeSettings");

            PublicFrameworks.Add("AVFoundation");

            string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
            AdditionalPropertiesForReceipt.Add("IOSPlugin", Path.Combine(PluginPath, "MeoPlayVoiceChat_IOSAPL.xml"));
        }
    }
}
