@echo off
Set CurDir=%~dp0
Set Editor=%CurDir%..\Engine\Binaries\Win64\UnrealEditor.exe
Set ProjFile=%CurDir%\ProjectM.uproject
start "" %Editor% %ProjFile% -server -log Experience=BP_ShooterCore_DefaultExperience