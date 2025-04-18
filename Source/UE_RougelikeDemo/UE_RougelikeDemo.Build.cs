// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE_RougelikeDemo : ModuleRules
{
	public UE_RougelikeDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"HeadMountedDisplay", 
			"EnhancedInput",
            "UMG",
            "Niagara",
			"GameplayAbilities"
        });
		PrivateDependencyModuleNames.AddRange(new string[] { "GameplayTags", "GameplayTasks" });
	}
}
