@echo off
echo ========================================
echo Building TinaGodot Editor - Debug
echo ========================================
echo.

scons platform=windows target=editor arch=x86_64 ^
    dev_build=yes ^
    debug_symbols=yes ^
    optimize=none ^
    module_gdscript_enabled=no ^
    module_mono_enabled=no ^
    module_webrtc_enabled=no ^
    module_multiplayer_enabled=no ^
    module_enet_enabled=no ^
    module_navigation_3d_enabled=no ^
    module_openxr_enabled=no ^
    module_mobile_vr_enabled=no ^
    module_webxr_enabled=no ^
    module_gridmap_enabled=no ^
    module_lightmapper_rd_enabled=no ^
    module_raycast_enabled=no ^
    module_gltf_enabled=no ^
    module_csg_enabled=no ^
    module_objectdb_profiler_enabled=no ^
    module_xatlas_unwrap_enabled=no ^
    module_vhacd_enabled=no ^
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
