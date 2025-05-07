@echo off
Set CurDir=%~dp0
Set Editor=%CurDir%..\Engine\Binaries\Win64\UnrealEditor
Set ProjFile=%CurDir%\ProjectM.uproject
start "" %Editor% %ProjFile% /Game/Maps/Level_P_Square.Level_P_Square -server -log -port=7777

:: /Game/ThirdPerson/Maps/ThirdPersonMap.ThirdPersonMap -server -log -port=7777
:: /Game/Maps/Level_P_Square.Level_P_Square
:: /Game/Maps/Level_P_Field.Level_P_Field