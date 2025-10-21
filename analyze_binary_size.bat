@echo off
echo ===================================
echo Godot 二进制大小分析工具
echo ===================================
echo.

set BIN_DIR=bin
set EDITOR_EXE=%BIN_DIR%\godot.windows.editor.x86_64.exe

if not exist "%EDITOR_EXE%" (
    echo 错误: 找不到编辑器可执行文件!
    echo 路径: %EDITOR_EXE%
    pause
    exit /b 1
)

echo 当前编辑器文件:
dir "%EDITOR_EXE%" | findstr "godot"
echo.

echo 正在分析文件大小...
echo.

REM 使用 dumpbin 分析 PE 文件结构（如果有 Visual Studio）
where dumpbin >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo 使用 dumpbin 分析 PE 结构...
    dumpbin /headers "%EDITOR_EXE%" | findstr /C:"size of code" /C:"size of initialized" /C:"size of uninitialized" /C:"size of image"
    echo.
) else (
    echo 提示: 未找到 dumpbin 工具，跳过详细分析
    echo.
)

echo ===================================
echo 编译配置建议
echo ===================================
echo.
echo 当前构建: target=editor (包含调试符号)
echo 大小: 约 86MB
echo.
echo 推荐配置:
echo 1. Release 编译 (不含调试符号):
echo    scons target=template_release debug_symbols=no
echo    预计大小: 约 15-20MB
echo.
echo 2. Release + LTO 优化:
echo    scons target=template_release debug_symbols=no use_lto=yes
echo    预计大小: 约 12-18MB
echo.
echo 3. Release + LTO + 精简功能:
echo    scons target=template_release debug_symbols=no use_lto=yes ^\
echo          module_websocket_enabled=no module_mbedtls_enabled=no
echo    预计大小: 约 10-15MB
echo.
echo 使用 build_editor_release.bat 进行 Release 编译
echo.

pause
