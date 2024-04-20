// Copyright Stephen Swires. All Rights Reserved.

using UnrealBuildTool;

public class OnDeck : ModuleRules
{
	public bool IsPlatformSupported => Target.Platform == UnrealTargetPlatform.Linux || Target.Platform == UnrealTargetPlatform.Win64;
	public string IsPlatformSupportedString => IsPlatformSupported ? "1" : "0";
	
	public OnDeck(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
		});
		
		PrivateDependencyModuleNames.AddRange(new string[] 
		{
			"CoreUObject",
			"Engine",
			"DeveloperSettings",
		});
		
		PublicDefinitions.Add($"ONDECK_STEAM_API_AVAILABLE={IsPlatformSupportedString}");

		if (IsPlatformSupported)
		{
			PrivateDependencyModuleNames.AddRange(new string[]
			{
                "Slate",
				"SteamShared",
			});
			
			AddEngineThirdPartyPrivateStaticDependencies(Target, "Steamworks");
		}
	}
}
