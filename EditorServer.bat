@echo off
Set CurDir=%~dp0
Set Editor=%CurDir%..\Engine\Binaries\Win64\UnrealEditor
Set ProjFile=%CurDir%\ProjectM.uproject
start "" %Editor% %ProjFile% -server -log -port 7777

:: /Game/ThirdPerson/Maps/ThirdPersonMap.ThirdPersonMap -server -log -port 7777