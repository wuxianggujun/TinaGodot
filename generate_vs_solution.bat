@echo off
echo ========================================
echo Generating Visual Studio Solution
echo ========================================
echo.

echo Generating VS project files...
echo.

scons platform=windows vsproj=yes ^
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
    module_vhacd_enabled=no

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo Solution Generated Successfully!
    echo ========================================
    echo Solution File: godot.sln
    echo.
    echo Usage:
    echo 1. Double-click godot.sln to open Visual Studio
    echo 2. Select build configuration (Debug/Release)
    echo 3. Press F5 to debug or Ctrl+Shift+B to build
    echo ========================================
) else (
    echo.
    echo ========================================
    echo Solution Generation Failed!
    echo ========================================
)

echo.
pause
