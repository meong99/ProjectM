using UnrealBuildTool;
 
public class ProjectMEditor : ModuleRules
{
    public ProjectMEditor(ReadOnlyTargetRules Target) : base(Target)
    {
		PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"Engine",
				"UnrealEd",
				"ProjectM",
			}
		);
		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"Slate",
				"SlateCore",
				"ToolMenus",
				"DeveloperToolSettings",
			}
		);
	}
}
 