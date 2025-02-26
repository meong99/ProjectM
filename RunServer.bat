@echo off
Set CurDir=%~dp0
Set Editor=%CurDir%..\Engine\Binaries\Win64\UnrealEditor-Win64-Debug
Set ProjFile=%CurDir%\ProjectM.uproject
start "" %Editor% %ProjFile% -server -log -port 7777 Experience=BP_ShooterCore_DefaultExperience