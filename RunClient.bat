@echo off
Set CurDir=%~dp0
Set Editor=%CurDir%..\Engine\Binaries\Win64\UnrealEditor.exe
Set ProjFile=%CurDir%\ProjectM.uproject
start "" %Editor% %ProjFile% -Client -Game 127.0.0.1:7777