
set UE4_PATH=C:\UnrealEngine\

set PROJECT_NAME=zSpace


If EXIST  "C:\zSpace_Server"  rmdir C:\zSpace_Server /s /q
IF EXIST  "C:\zSpace_Client"  rmdir C:\zSpace_Client /s /q
IF EXIST  "C:\zSpaceArchive"  rmdir C:\zSpaceArchive /s /q

set PROJECT_PATH=%CD%\%PROJECT_NAME%.uproject

set CLIENTCONFIG=Development

set ARCHIVEDIRECTORY=C:\zSpace_Server
set ARCHIVEDIRECTORY_CLIENT=C:\%PROJECT_NAME%_Client\WindowsNoEditor

set SERVER_PATH=%ARCHIVEDIRECTORY%\WindowsNoEditor

set FINAL_SERVER_PATH=%ARCHIVEDIRECTORY%\zSpaceDedicatedServer

IF NOT EXIST %ARCHIVEDIRECTORY% mkdir "%ARCHIVEDIRECTORY%"

IF NOT EXIST "C:\%PROJECT_NAME%_Client" mkdir "C:\%PROJECT_NAME%_Client"


call "%UE4_PATH%\Engine\Build\BatchFiles\Build.bat" "%PROJECT_NAME%Editor" win64 %CLIENTCONFIG% "%PROJECT_PATH%" -WaitMutex 
if %errorlevel% NEQ 0 exit /b %errorlevel%

::call "%CD%\BuildLighting.bat" 
if %errorlevel% NEQ 0 exit /b %errorlevel%

call "%UE4_PATH%\Engine\Build\BatchFiles\RunUAT.bat" -ScriptsForProject="%PROJECT_PATH%" BuildCookRun -nocompileeditor -installed -nop4 -project="%PROJECT_PATH%" -cook -allmaps -stage -archive -archivedirectory=%ARCHIVEDIRECTORY% -package -ue4exe="%UE4_PATH%\Engine\Binaries\Win64\UE4Editor-Cmd.exe"  -ddc=InstalledDerivedDataBackendGraph -nopak -prereqs -nodebuginfo -targetplatform=Win64 -build -target=%PROJECT_NAME% -clientconfig=%CLIENTCONFIG% -utf8output -UNATTENDED
if %errorlevel% NEQ 0 exit /b %errorlevel%


call "%UE4_PATH%\Engine\Build\BatchFiles\RunUAT.bat" -ScriptsForProject="%PROJECT_PATH%" BuildCookRun -nocompileeditor -nop4 -project="%PROJECT_PATH%" -cook -allmaps -stage -archive -archivedirectory=%ARCHIVEDIRECTORY% -package -ue4exe="%UE4_PATH%\Engine\Binaries\Win64\UE4Editor-Cmd.exe" -ddc=DerivedDataBackendGraph -nopak -prereqs -nodebuginfo -targetplatform=Win64 -build -target=%PROJECT_NAME%Server -serverconfig=%CLIENTCONFIG% -utf8output -compile -UNATTENDED
if %errorlevel% NEQ 0 exit /b %errorlevel%

set SERVER_EXE_PATH=%ARCHIVEDIRECTORY%\WindowsServer\%PROJECT_NAME%\Binaries\Win64\%PROJECT_NAME%Server.exe

if %errorlevel% NEQ 0 exit /b %errorlevel%
set MOVE_SERVER_EXE_PATH=%ARCHIVEDIRECTORY%\WindowsNoEditor\%PROJECT_NAME%\Binaries\Win64\

if exist ( "%SERVER_EXE_PATH%" ) (
	echo "Z Space: copying exe" 
	echo "Info xcopy %SERVER_PATH% %ARCHIVEDIRECTORY_CLIENT%"
	xcopy /Y /E /I "%SERVER_PATH%" "%ARCHIVEDIRECTORY_CLIENT%"
	if %errorlevel% NEQ 0 exit /b %errorlevel%
	xcopy /Y /E /I  %SERVER_EXE_PATH% %MOVE_SERVER_EXE_PATH%
	if %errorlevel% NEQ 0 exit /b %errorlevel%
	move %SERVER_PATH% %FINAL_SERVER_PATH%
	if %errorlevel% NEQ 0 exit /b %errorlevel%
	move %ARCHIVEDIRECTORY%\WindowsServer %ARCHIVEDIRECTORY%\WindowsServer_NoExecut
	if %errorlevel% NEQ 0 exit /b %errorlevel%
	xcopy /Y /E /I "%CD%\OWSInstanceLauncher" "%ARCHIVEDIRECTORY%\OWSInstanceLauncher"
	if %errorlevel% NEQ 0 exit /b %errorlevel%
	copy "%CD%\appsettings.json" "%ARCHIVEDIRECTORY%\OWSInstanceLauncher\appsettings.json"
	if %errorlevel% NEQ 0 exit /b %errorlevel%
	copy "%CD%\RunOWS.bat" "%ARCHIVEDIRECTORY%\OWSInstanceLauncher\RunOWS.bat"
	if %errorlevel% NEQ 0 exit /b %errorlevel%
	
)




