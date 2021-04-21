
set UE4_PATH=C:\UnrealEngine\


set PROJECT_NAME=zSpace

set TEST_MAP_NAME=FourZoneMap

set PROJECT_PATH=%CD%\%PROJECT_NAME%.uproject


call %UE4_PATH%\Engine\Binaries\Win64\UE4Editor-Cmd.exe ^
 "%PROJECT_PATH%" ^
 -Run=ResavePackages ^
 -IgnoreChangeList ^
 -BuildLighting ^
 -Quality=Production ^
 -MapsOnly ^
 -ProjectOnly ^
 -AllowCommandletRendering 
 
 
 


