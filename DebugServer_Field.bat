@echo off
Set CurDir=%~dp0
Set Editor=%CurDir%..\Engine\Binaries\Win64\UnrealEditor-Win64-Debug
Set ProjFile=%CurDir%\ProjectM.uproject
start "" %Editor% %ProjFile% /Game/Maps/Level_P_Field.Level_P_Field -server -log -port=7778