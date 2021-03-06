
set UE4_PATH=C:\UnrealEngine\

set PROJECT_NAME=zSpace

set PROJECT_PATH=%CD%/%PROJECT_NAME%.uproject

set CLIENTCONFIG=Development

set ARCHIVEDIRECTORY=C:/%PROJECT_NAME%_Client/


call "%UE4_PATH%\Engine\Build\BatchFiles\Build.bat" "%PROJECT_NAME%Editor" win64 %CLIENTCONFIG% "%PROJECT_PATH%" -WaitMutex

call "%UE4_PATH%\Engine\Build\BatchFiles\RunUAT.bat" -ScriptsForProject="%PROJECT_PATH%" BuildCookRun -nocompileeditor -installed -nop4 -project="%PROJECT_PATH%" -cook -allmaps -stage -archive -archivedirectory=%ARCHIVEDIRECTORY% -package -ue4exe="%UE4_PATH%\Engine\Binaries\Win64\UE4Editor-Cmd.exe"  -ddc=InstalledDerivedDataBackendGraph -pak -prereqs -nodebuginfo -targetplatform=Win64 -build -target=%PROJECT_NAME% -clientconfig=%CLIENTCONFIG% -utf8output