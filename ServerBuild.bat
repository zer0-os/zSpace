
set UE4_PATH=C:\UnrealEngine\

set PROJECT_NAME=zSpace

set PROJECT_PATH=%CD%\%PROJECT_NAME%.uproject

set CLIENTCONFIG=Development

set ARCHIVEDIRECTORY=C:\zSpace_Server

set SERVER_PATH=%ARCHIVEDIRECTORY%\WindowsNoEditor

set FINAL_SERVER_PATH=%ARCHIVEDIRECTORY%\zSpaceDedicatedServer

IF NOT EXIST %ARCHIVEDIRECTORY% mkdir "%ARCHIVEDIRECTORY%"

 
call "%UE4_PATH%\Engine\Build\BatchFiles\Build.bat" "%PROJECT_NAME%Editor" win64 %CLIENTCONFIG% "%PROJECT_PATH%" -WaitMutex

call "%UE4_PATH%\Engine\Build\BatchFiles\RunUAT.bat" -ScriptsForProject="%PROJECT_PATH%" BuildCookRun -nocompileeditor -installed -nop4 -project="%PROJECT_PATH%" -cook -allmaps -stage -archive -archivedirectory=%ARCHIVEDIRECTORY% -package -ue4exe="%UE4_PATH%\Engine\Binaries\Win64\UE4Editor-Cmd.exe"  -ddc=InstalledDerivedDataBackendGraph -pak -prereqs -nodebuginfo -targetplatform=Win64 -build -target=%PROJECT_NAME% -clientconfig=%CLIENTCONFIG% -utf8output

call "%UE4_PATH%\Engine\Build\BatchFiles\RunUAT.bat" -ScriptsForProject="%PROJECT_PATH%" BuildCookRun -nocompileeditor -nop4 -project="%PROJECT_PATH%" -cook -allmaps -stage -archive -archivedirectory=%ARCHIVEDIRECTORY% -package -ue4exe="%UE4_PATH%\Engine\Binaries\Win64\UE4Editor-Cmd.exe" -ddc=DerivedDataBackendGraph -pak -prereqs -nodebuginfo -targetplatform=Win64 -build -target=%PROJECT_NAME%Server -serverconfig=%CLIENTCONFIG% -utf8output -compile

set SERVER_EXE_PATH=%ARCHIVEDIRECTORY%\WindowsServer\%PROJECT_NAME%\Binaries\Win64\%PROJECT_NAME%Server.exe

set MOVE_SERVER_EXE_PATH=%ARCHIVEDIRECTORY%\WindowsNoEditor\%PROJECT_NAME%\Binaries\Win64\

if exist ( "%SERVER_EXE_PATH%" )(
	echo "Z Space: copying exe"
	copy %SERVER_EXE_PATH% %MOVE_SERVER_EXE_PATH%
	move %SERVER_PATH% %FINAL_SERVER_PATH%
	move %ARCHIVEDIRECTORY%\WindowsServer %ARCHIVEDIRECTORY%\WindowsServer_NoExecut
	xcopy /E /I "%CD%\OWSInstanceLauncher" "%ARCHIVEDIRECTORY%\OWSInstanceLauncher"
	copy "%CD%\appsettings.json" "%ARCHIVEDIRECTORY%\OWSInstanceLauncher\appsettings.json"
	
)




