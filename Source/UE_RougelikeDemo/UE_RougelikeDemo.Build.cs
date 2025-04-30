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
            "Slate",
            "SlateCore",
            "Niagara",
			"GameplayAbilities",
            "AIModule",
        });
		PrivateDependencyModuleNames.AddRange(new string[] { "GameplayTags", "GameplayTasks" });
	}
}
