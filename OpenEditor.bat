@echo off
Set CurDir=%~dp0
Set Editor=%CurDir%..\Engine\Binaries\Win64\UnrealEditor
Set ProjFile=%CurDir%\ProjectM.uproject
start "" %Editor% %ProjFile% Experience=BP_ShooterCore_DefaultExperience

:: Set Editor=%CurDir%..\Engine\Binaries\Win64\UnrealEditor.exe