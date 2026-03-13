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

rem Собираем все модули в папке Modules, у которых есть свой CMakeLists.txt
for /D %%M in ("%ROOT%Modules\*") do (
  if exist "%%M\CMakeLists.txt" (
    echo.
    echo Building module in %%M
    set "MOD_BUILD=%%M\Build"
    if not exist "%%M\Build" mkdir "%%M\Build"
    pushd "%%M\Build"
    cmake ".." -G "Visual Studio 17 2022" -A x64
    if errorlevel 1 (
      echo Module configure failed in %%M
      popd
      goto :err
    )
    cmake --build . --config Release
    if errorlevel 1 (
      echo Module build failed in %%M
      popd
      goto :err
    )
    popd
  )
)

echo.
echo Build OK. Exe: Binaries\BlesslessEngine.exe

rem Ассоциация .bless с BlesslessEngine.exe (двойной клик по project.bless)
set "EXEPATH=%ROOT%Binaries\BlesslessEngine.exe"
reg add "HKCU\Software\Classes\.bless" /ve /d "BlesslessEngine.Project" /f >nul
reg add "HKCU\Software\Classes\BlesslessEngine.Project" /ve /d "Blessless Engine Project" /f >nul
reg add "HKCU\Software\Classes\BlesslessEngine.Project\shell\open\command" /ve /d "\"%EXEPATH%\" \"%%1\"" /f >nul
echo .bless associated with BlesslessEngine.exe.

exit /b 0
:err
echo Build failed.
exit /b 1
