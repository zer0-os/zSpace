// Copyright 2019 Ruslan Nazirov. All Rights Reserved.

using UnrealBuildTool;

public class Etherlinker : ModuleRules
{
	public Etherlinker(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        if (Target.bBuildEditor == true)
        {
            PrivateDependencyModuleNames.AddRange(
                new string[] {
                    "UnrealEd"
                }
            );
        }

        PublicIncludePaths.AddRange(
			new string[] {
				//"Etherlinker/Public"                
                //"EtherlinkerEditor/Public"
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				//"Etherlinker/Private",
				// ... add other private include paths required here ...
			}
            );

        PrivateIncludePathModuleNames.AddRange(
           new string[] {
                "Settings"
           }
       );

        PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core"
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
                "HeadMountedDisplay",
                "UMG",
                "Http",
                "Json",
                "JsonUtilities"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
