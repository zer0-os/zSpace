cd /d %1
call mvnw org.web3j:web3j-maven-plugin:4.5.11:generate-sources
IF %ERRORLEVEL% NEQ 0 (
  pause
  EXIT /B 1
)