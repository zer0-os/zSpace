
taskkill /im  OWSInstanceLauncher.exe /F /T /FI "IMAGENAME  eq OWSInstanceLauncher.exe"

cd C:\zSpace_Server

Start /B %CD%\OWSInstanceLauncher\OWSInstanceLauncher.exe