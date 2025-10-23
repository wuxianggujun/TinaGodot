# Visual Studio 项目生成错误修复

## 问题描述
生成的 `godot.vcxproj` 文件包含了 CMake 临时文件，导致 MSBuild 解析错误：
```
无法计算表达式""".31"。未找到方法"System.String.31"。
```

## 根本原因
SCons 的 `glob_recursive_2` 函数会递归扫描所有目录，包括：
- `out/` - CMake 构建输出目录
- `.vs/` - Visual Studio 临时文件
- `build/` - 其他构建输出

这些目录中的文件路径包含版本号（如 `3.31.6`），MSBuild 误将其解析为表达式。

## 解决方案

### 已修复
修改了 `methods.py` 中的 `glob_recursive_2` 函数，添加了目录排除列表：

```python
excluded_dirs = ["out", ".vs", ".git", ".github", ".vscode", "bin", "build"]
```

### 重新生成项目
运行以下命令或双击 `regenerate_vs_project.bat`：

```bash
# 清理旧文件
del godot.vcxproj godot.sln godot.vcxproj.filters

# 重新生成
scons platform=windows vsproj=yes
```

## 验证
生成后检查 `godot.vcxproj` 文件，确保不包含：
- `out\build\` 路径
- `CMakeFiles\` 路径
- 任何带版本号的临时文件

## 预防措施
1. 不要在 Godot 源码目录中运行 CMake
2. 使用 SCons 作为唯一的构建系统
3. 定期清理构建输出目录

## 相关文件
- `methods.py:1005` - `glob_recursive_2` 函数
- `regenerate_vs_project.bat` - 快速重新生成脚本
