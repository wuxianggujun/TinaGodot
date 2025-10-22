@echo off
echo ========================================
echo Building TinaGodot Editor - Debug
echo ========================================
echo.

scons platform=windows target=editor arch=x86_64 ^
    dev_build=yes ^
    debug_symbols=yes ^
    optimize=none ^
    module_minimp3_enabled=no ^
    module_vorbis_enabled=no ^
    module_interactive_music_enabled=no ^
    -j7

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo Build Successful!
    echo ========================================
    echo Binary: bin\godot.windows.editor.dev.x86_64.exe
    echo Debug Symbols: YES
    echo Optimization: NONE (for debugging)
    echo ========================================
) else (
    echo.
    echo ========================================
    echo Build Failed!
    echo ========================================
)

echo.
pause
