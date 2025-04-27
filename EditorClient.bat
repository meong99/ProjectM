@echo off
Set CurDir=%~dp0
Set Editor=%CurDir%..\Engine\Binaries\Win64\UnrealEditor
Set ProjFile=%CurDir%\ProjectM.uproject
start "" %Editor% %ProjFile% -Game -Client 127.0.0.1:7777

:: Set Editor=%CurDir%..\Engine\Binaries\Win64\UnrealEditor.exe -Game -Client 127.0.0.1:7777