// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MyTutorTest : ModuleRules
{
	public MyTutorTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        OptimizeCode = CodeOptimization.Never;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "SlateCore", "Slate", "GameplayAbilities", "UnrealEd" });
	}
}
