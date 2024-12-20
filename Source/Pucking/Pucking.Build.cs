// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Pucking : ModuleRules
{
	public Pucking(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicIncludePaths.AddRange(new string[] { "Pucking" });
		
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG" });
	}
}
