set UE4_PATH=C:\UnrealEngine\


set PROJECT_NAME=zSpace

set TEST_MAP_NAME=FourZoneMap

set PROJECT_PATH=%CD%\%PROJECT_NAME%.uproject

set CLIENTCONFIG=Development

call "%UE4_PATH%\Engine\Build\BatchFiles\Build.bat" "%PROJECT_NAME%Editor" win64 %CLIENTCONFIG% "%PROJECT_PATH%" -WaitMutex

call %UE4_PATH%\Engine\Binaries\Win64\UE4Editor-Cmd.exe ^
 "%PROJECT_PATH%" ^
 -run=CompileAllBlueprints
 