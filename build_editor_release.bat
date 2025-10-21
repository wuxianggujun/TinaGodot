@echo off
echo Building Godot Editor (Release/Optimized)...
echo.

REM Release 构建配置:
REM - target=template_release: Release 优化版本
REM - optimize=speed: 优化速度
REM - use_lto=yes: 启用链接时优化 (LTO)
REM - debug_symbols=no: 不包含调试符号
REM - use_static_cpp=yes: 静态链接 C++ 运行时

scons -j8 p=windows target=template_release optimize=speed use_lto=yes debug_symbols=no use_static_cpp=yes %*

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Build Successful!
    echo Binary: bin\godot.windows.template_release.x86_64.exe
    echo.
    echo Checking binary size...
    dir bin\godot.windows.template_release.x86_64.exe | findstr "godot"
) else (
    echo.
    echo Build Failed!
)

pause
