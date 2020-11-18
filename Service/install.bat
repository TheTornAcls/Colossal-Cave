@ECHO OFF
ECHO %WINDIR%\Microsoft.NET\Framework\v1.1.4322\InstallUtil.exe bin\Debug\WindowsService.exe
InstallUtil.exe bin\Debug\WindowsService.exe

IF NOT ERRORLEVEL = 0 GOTO :ERROR
ECHO.
ECHO.
ECHO To uninstall, run the following command
ECHO %WINDIR%\Microsoft.NET\Framework\v1.1.4322\InstallUtil.exe -u bin\Debug\WindowsService.exe
ECHO.

:ERROR


