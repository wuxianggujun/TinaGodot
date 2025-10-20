<#
  中文：在仓库根目录生成 Visual Studio 工程（仅生成，不编译）
  需求：VS 2022 C++ 工作负载、Python3 与 SCons。
  用法：在 PowerShell 中执行 .\gen_vsproj.ps1
#>

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

Set-Location -Path $PSScriptRoot
Write-Host "[信息] 正在生成 Visual Studio 工程（vsproj），仅生成不构建..."

function Test-Command($name) {
  try { Get-Command $name -ErrorAction Stop | Out-Null; return $true } catch { return $false }
}

$scons = 'scons'
if (-not (Test-Command 'scons')) {
  if (Test-Command 'py') {
    try { py -m scons --version | Out-Null; $scons = 'py -m scons' } catch {}
  }
}

if (-not (Test-Command 'scons') -and $scons -ne 'py -m scons') {
  Write-Error "[错误] 未找到 scons。请先安装：py -m pip install scons"
}

$argsLine = $args -join ' '
& $scons platform=windows target=editor arch=x86_64 dev_build=yes vsproj=yes vsproj_gen_only=yes $args

Write-Host "[完成] 若成功，已生成解决方案：$(Join-Path (Get-Location) 'godot.sln')"
Write-Host "          在 VS 中选择配置：editor | x64"

