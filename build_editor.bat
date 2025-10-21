@echo off
echo Building Godot Editor...
echo.

scons -j8 p=windows target=editor %*

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Build Successful!
    echo Binary: bin\godot.windows.editor.x86_64.exe
) else (
    echo.
    echo Build Failed!
)

pause
