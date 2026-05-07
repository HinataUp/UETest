// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UETest : ModuleRules
{
	public UETest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "NetCore",
			// GAS 三件套
			"GameplayAbilities", "GameplayTags", "GameplayTasks",
			// 渲染模块所需 ：Global Shader / Compute Shader / RDG / SceneCapture
			"RenderCore", "RHI", "Renderer", "Projects"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
