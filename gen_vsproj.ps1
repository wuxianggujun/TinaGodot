<#
  在仓库根目录生成 Visual Studio 解决方案（仅生成，不编译）
  需要已安装 VS 2022 C++ 工具链、Python3 与 SCons。
  用法：在 PowerShell 中执行 .\gen_vsproj.ps1 [额外 SCons 参数]
#>

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

Set-Location -Path $PSScriptRoot
Write-Host "[信息] 正在准备生成 Visual Studio 工程（vsproj）……"

function Test-Command($name) {
  try { Get-Command $name -ErrorAction Stop | Out-Null; return $true } catch { return $false }
}

$sconsCmd = $null
if (Test-Command 'scons') {
  $sconsCmd = @('scons')
} elseif (Test-Command 'py') {
  try { py -m scons --version | Out-Null; $sconsCmd = @('py','-m','scons') } catch {}
}

if (-not $sconsCmd) {
  Write-Error "[错误] 未找到 scons。请先安装：py -m pip install scons"
  exit 1
}

& $sconsCmd[0] $sconsCmd[1..($sconsCmd.Length-1)] `
  platform=windows target=editor arch=x86_64 dev_build=yes `
  vsproj=yes vsproj_gen_only=yes @args

Write-Host "[完成] 生成成功。如存在：$(Join-Path (Get-Location) 'godot.sln')"
Write-Host "         在 VS 选择配置：editor | x64"

