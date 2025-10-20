@echo off
setlocal ENABLEDELAYEDEXPANSION
cd /d "%~dp0\.."
echo [信息] 第二阶段清理：删除 thirdparty 目录（需本地执行）。
echo.
set LIST=thirdparty\enet thirdparty\mbedtls thirdparty\miniupnpc thirdparty\wslay thirdparty\meshoptimizer thirdparty\xatlas thirdparty\recastnavigation thirdparty\rvo2

for %%D in (%LIST%) do (
  if exist "%%D" (
    echo [清理] %%D
    git rm -r --cached --quiet "%%D" 2>nul
    rmdir /s /q "%%D" 2>nul
  ) else (
    echo [跳过] %%D (不存在)
  )
)

echo [完成] 如需提交：git add -A && git commit -m "chore(purge-thirdparty): 移除网络/导航/3D工具相关依赖"
endlocal
exit /b 0

