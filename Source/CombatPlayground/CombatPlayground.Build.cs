// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CombatPlayground : ModuleRules
{
	public CombatPlayground(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "AbilityHelper",  "GameplayTags", "GameplayTasks", "ALS" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayAbilities"
		});
	}
}
