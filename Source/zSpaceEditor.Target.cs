// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class zSpaceEditorTarget : TargetRules
{
	public zSpaceEditorTarget(TargetInfo Target) : base (Target)
	{
		Type = TargetType.Editor;
        ExtraModuleNames.AddRange(new string[] { "zSpace" });
        bCompileChaos = true;
        bUseChaos = true;
        DefaultBuildSettings = BuildSettingsVersion.V2;
    }
}
