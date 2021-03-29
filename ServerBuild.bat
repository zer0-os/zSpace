
set UE4_PATH=C:\UnrealEngine\

set PROJECT_NAME=zSpace

set PROJECT_PATH=%CD%/%PROJECT_NAME%.uproject

set CLIENTCONFIG=Development

set ARCHIVEDIRECTORY=C:\Users\ue4Ar\Desktop\Server

 
call "%UE4_PATH%\Engine\Build\BatchFiles\Build.bat" "%PROJECT_NAME%Editor" win64 Development "%PROJECT_PATH%" -WaitMutex


call "%UE4_PATH%\Engine\Build\BatchFiles\RunUAT.bat" -ScriptsForProject="%PROJECT_PATH%" BuildCookRun -project="%PROJECT_PATH%" -noP4 -clientconfig=%CLIENTCONFIG% -serverconfig=%CLIENTCONFIG% -nocompileeditor -ue4exe="%UE4_PATH%\Engine\Binaries\Win64\UE4Editor-Cmd.exe"  -utf8output -server -serverplatform=Win64 -noclient -build -cook -Maps=List+Of+Maps+To+Include -compressed -stage -package -stagingdirectory=%ARCHIVEDIRECTORY% -compile
