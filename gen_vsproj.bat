@echo off
setlocal ENABLEDELAYEDEXPANSION

rem 中文：在仓库根目录生成 Visual Studio 解决方案（仅生成，不编译）
rem 需求：已安装 VS 2022 C++ 工作负载、Python3 与 SCons。

cd /d "%~dp0"
echo [信息] 正在生成 Visual Studio 工程（vsproj），仅生成不构建...

set SCONS_CMD=scons
where scons >nul 2>nul
if errorlevel 1 (
    py -m scons --version >nul 2>nul
    if errorlevel 1 (
        echo [错误] 未找到 scons。请先安装：py -m pip install scons
        exit /b 1
    ) else (
        set SCONS_CMD=py -m scons
    )
)

"%SCONS_CMD%" platform=windows target=editor arch=x86_64 dev_build=yes vsproj=yes vsproj_gen_only=yes %*
if errorlevel 1 (
    echo [错误] 生成 VS 工程失败。
    exit /b 1
)

if exist "godot.sln" (
    echo [完成] 已生成解决方案：%cd%\godot.sln
    echo          在 VS 中选择配置：editor | x64
) else (
    echo [注意] 未检测到 godot.sln，请检查 SCons 输出是否改变了生成位置。
)

endlocal
exit /b 0

