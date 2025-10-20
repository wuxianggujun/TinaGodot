<#
  第二阶段清理脚本：删除 thirdparty 目录（需本地执行）
  目标：enet、mbedtls、miniupnpc、wslay、meshoptimizer、xatlas、recastnavigation、rvo2
#>
Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

Set-Location -Path (Join-Path $PSScriptRoot '..')
$dirs = @(
  'thirdparty/enet',
  'thirdparty/mbedtls',
  'thirdparty/miniupnpc',
  'thirdparty/wslay',
  'thirdparty/meshoptimizer',
  'thirdparty/xatlas',
  'thirdparty/recastnavigation',
  'thirdparty/rvo2'
)

foreach ($d in $dirs) {
  if (Test-Path $d) {
    Write-Host "[清理] $d"
    try { git rm -r --cached --quiet $d | Out-Null } catch {}
    Remove-Item -Recurse -Force $d
  } else {
    Write-Host "[跳过] $d (不存在)"
  }
}

Write-Host "[完成] 如需提交：git add -A; git commit -m 'chore(purge-thirdparty): 移除网络/导航/3D工具相关依赖'"

