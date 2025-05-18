@echo off
Set CurDir=%~dp0
Set Editor=%CurDir%..\Engine\Binaries\Win64\UnrealEditor-Win64-Debug
Set ProjFile=%CurDir%\ProjectM.uproject
start "" %Editor% %ProjFile% /Game/Maps/TestMap.TestMap -server -log -port 7777