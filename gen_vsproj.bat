@echo off
setlocal

cd /d "%~dp0"
echo Generating Visual Studio project files...

set "SCONS_CMD="
where scons >nul 2>nul
if %ERRORLEVEL%==0 (
    set "SCONS_CMD=scons"
) else (
    py -m scons --version >nul 2>nul
    if %ERRORLEVEL%==0 (
        set "SCONS_CMD=py -m scons"
    ) else (
        echo ERROR: scons not found. Install via: py -m pip install scons
        exit /b 1
    )
)

call %SCONS_CMD% platform=windows target=editor arch=x86_64 dev_build=yes vsproj=yes vsproj_gen_only=yes %*
if errorlevel 1 (
    echo ERROR: VS project generation failed.
    exit /b 1
)

if exist "godot.sln" (
    echo OK: generated at %cd%\godot.sln
) else (
    echo NOTE: godot.sln not found; check SCons output.
)

endlocal
exit /b 0

