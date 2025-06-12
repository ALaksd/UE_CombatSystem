// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class UE_RougelikeDemoEditorTarget : TargetRules
{
	public UE_RougelikeDemoEditorTarget(TargetInfo Target) : base(Target)
	{
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
        ExtraModuleNames.Add("UE_RougelikeDemo");
	}
}
