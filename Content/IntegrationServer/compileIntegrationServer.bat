cd /d %1
call mvnw clean package -DskipTests=true
IF %ERRORLEVEL% NEQ 0 (
  pause
  EXIT /B 1
)