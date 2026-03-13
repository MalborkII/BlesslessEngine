@echo off
setlocal
set ROOT=%~dp0
set BUILD=%ROOT%Build

if not exist "%BUILD%" mkdir "%BUILD%"
cd /d "%BUILD%"

rem Источник — родитель Build; без trailing backslash, иначе ломается разбор -G
cmake ".." -G "Visual Studio 17 2022" -A x64
if errorlevel 1 goto :err
cmake --build . --config Release
if errorlevel 1 goto :err

echo.
echo Build OK. Exe: Binaries\Win64\BEVersionSelector.exe, Binaries\Win64\BlesslessEngine.exe

rem Ассоциация .bless с BEVersionSelector (двойной клик по project.bless)
set "EXEPATH=%ROOT%Binaries\Win64\BEVersionSelector.exe"
reg add "HKCU\Software\Classes\.bless" /ve /d "BlesslessEngine.Project" /f >nul
reg add "HKCU\Software\Classes\BlesslessEngine.Project" /ve /d "Blessless Engine Project" /f >nul
reg add "HKCU\Software\Classes\BlesslessEngine.Project\shell\open\command" /ve /d "\"%EXEPATH%\" \"%%1\"" /f >nul
echo .bless associated with BEVersionSelector.

exit /b 0
:err
echo Build failed.
exit /b 1
