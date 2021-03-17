// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class zSpaceTarget : TargetRules
{
	public zSpaceTarget(TargetInfo Target) : base (Target)
	{
		Type = TargetType.Game;
        ExtraModuleNames.AddRange(new string[] { "zSpace" });
        DefaultBuildSettings = BuildSettingsVersion.V2;
    }
}
