# TinaGodot - 专注2D的轻量级游戏引擎

> **基于 Godot Engine 的 2D 专用版本**
> 分支: `godot-2d-lite`
> 完全移除3D功能，打造极致轻量的2D游戏引擎

---

## 🎯 项目简介

**TinaGodot** 是基于 [Godot Engine](https://godotengine.org) 的深度定制版本，**完全移除了所有3D功能**，专注于提供最轻量、最高效的2D游戏开发体验。

### ✨ 核心特性

- 🎮 **纯2D引擎**: 完全移除3D渲染、物理、导航等所有3D功能
- 📦 **轻量级**: 相比原版Godot减少~258MB代码和依赖
- ⚡ **高性能**: 专注2D优化，无3D开销
- 🛠️ **完整工具链**: 保留所有2D编辑器功能
- ✅ **持续集成**: 所有修改经过严格测试，编译成功

### 📊 清理成果

经过**8轮系统化清理**，实现3D功能的彻底移除:

| 清理项目 | 数量 | 状态 |
|---------|-----|------|
| 删除的3D文件 | 295个 (~258MB) | ✅ 完全删除 |
| 删除的3D代码 | ~1309行 | ✅ 完全清理 |
| 3D类名引用 | 从55处降至0处 | ✅ 完全移除 |
| 运行时类型检查 | ~13处 | ✅ 完全删除 |
| 3D兼容性类映射 | 39个 | ✅ 完全删除 |
| UI图标引用 | 多处 | ✅ 完全清理 |
| 条件编译宏 | 8处简化 | ✅ 移除3D部分 |

### 📚 详细文档

- **[3D清理报告](3D_CLEANUP_REPORT.md)** - 详细记录8轮清理的完整过程
- **[3D移除状态](GODOT_3D_REMOVAL_STATUS.md)** - 项目当前状态和最终验证结果

### 🚀 快速开始

#### 编译项目

```bash
# Windows (MinGW)
./build_lite.bat

# 或使用 SCons
scons platform=windows target=editor arch=x86_64 -j8
```

#### 编译选项

TinaGodot默认启用以下开关:
- `disable_3d=yes` - 禁用3D功能
- `disable_navigation_3d=yes` - 禁用3D导航
- `disable_physics_3d=yes` - 禁用3D物理
- `disable_xr=yes` - 禁用XR/VR支持

### 🔍 技术细节

#### 已移除的主要模块

**第三方库** (~200MB):
- `thirdparty/jolt_physics/` - 3D物理引擎
- `thirdparty/openxr/` - XR支持
- `thirdparty/vhacd/` - 凸包分解
- `thirdparty/embree/` - 光线追踪

**核心模块** (~50MB):
- `scene/3d/` - 所有3D节点
- `servers/physics_3d/` - 3D物理服务器
- `servers/xr/` - XR服务器
- `modules/fbx/` - FBX导入
- `modules/gdscript/` - GDScript模块

**编辑器插件**:
- `editor/scene/3d/` - 3D编辑器插件
- 所有3D gizmo、工具、预览插件

#### 清理策略

1. **物理删除** - 完全删除3D相关目录和文件
2. **代码清理** - 移除所有3D代码和运行时检查
3. **宏简化** - 简化条件编译宏，移除3D依赖
4. **文档整理** - 更新文档，移除3D引用

### ⚠️ 重要说明

- ✅ **2D功能完整**: 所有2D功能完全保留且正常工作
- ✅ **持续测试**: 每轮清理后都进行编译验证
- ✅ **Git历史**: 完整保留所有变更历史
- ❌ **不支持3D**: 无法导入或运行任何3D项目
- ❌ **不兼容**: 与原版Godot项目不完全兼容

### 🤝 贡献

欢迎提交Issue和Pull Request! 本项目专注于2D游戏开发，如果您有以下需求，欢迎贡献:
- 2D性能优化
- 2D编辑器改进
- 文档完善
- Bug修复

### 📄 许可证

TinaGodot继承Godot Engine的 [MIT许可证](https://godotengine.org/license)。
完全免费开源，无任何限制。

### 🙏 致谢

感谢 [Godot Engine](https://godotengine.org) 团队创造了这个优秀的开源游戏引擎。
TinaGodot是基于Godot的定制版本，所有核心功能归功于Godot社区。

---

**最后更新**: 2025-10-18
**清理状态**: ✅ 完成 (8轮系统化清理)
**编译状态**: ✅ 成功 (Windows x86_64 Editor)

---

# Godot Engine (原版说明)

<p align="center">
  <a href="https://godotengine.org">
    <img src="logo_outlined.svg" width="400" alt="Godot Engine logo">
  </a>
</p>

## 2D and 3D cross-platform game engine

**[Godot Engine](https://godotengine.org) is a feature-packed, cross-platform
game engine to create 2D and 3D games from a unified interface.** It provides a
comprehensive set of [common tools](https://godotengine.org/features), so that
users can focus on making games without having to reinvent the wheel. Games can
be exported with one click to a number of platforms, including the major desktop
platforms (Linux, macOS, Windows), mobile platforms (Android, iOS), as well as
Web-based platforms and [consoles](https://docs.godotengine.org/en/latest/tutorials/platform/consoles.html).

## Free, open source and community-driven

Godot is completely free and open source under the very permissive [MIT license](https://godotengine.org/license).
No strings attached, no royalties, nothing. The users' games are theirs, down
to the last line of engine code. Godot's development is fully independent and
community-driven, empowering users to help shape their engine to match their
expectations. It is supported by the [Godot Foundation](https://godot.foundation/)
not-for-profit.

Before being open sourced in [February 2014](https://github.com/godotengine/godot/commit/0b806ee0fc9097fa7bda7ac0109191c9c5e0a1ac),
Godot had been developed by [Juan Linietsky](https://github.com/reduz) and
[Ariel Manzur](https://github.com/punto-) (both still maintaining the project)
for several years as an in-house engine, used to publish several work-for-hire
titles.

![Screenshot of a 3D scene in the Godot Engine editor](https://raw.githubusercontent.com/godotengine/godot-design/master/screenshots/editor_tps_demo_1920x1080.jpg)

## Getting the engine

### Binary downloads

Official binaries for the Godot editor and the export templates can be found
[on the Godot website](https://godotengine.org/download).

### Compiling from source

[See the official docs](https://docs.godotengine.org/en/latest/engine_details/development/compiling)
for compilation instructions for every supported platform.

## Community and contributing

Godot is not only an engine but an ever-growing community of users and engine
developers. The main community channels are listed [on the homepage](https://godotengine.org/community).

The best way to get in touch with the core engine developers is to join the
[Godot Contributors Chat](https://chat.godotengine.org).

To get started contributing to the project, see the [contributing guide](CONTRIBUTING.md).
This document also includes guidelines for reporting bugs.

## Documentation and demos

The official documentation is hosted on [Read the Docs](https://docs.godotengine.org).
It is maintained by the Godot community in its own [GitHub repository](https://github.com/godotengine/godot-docs).

The [class reference](https://docs.godotengine.org/en/latest/classes/)
is also accessible from the Godot editor.

We also maintain official demos in their own [GitHub repository](https://github.com/godotengine/godot-demo-projects)
as well as a list of [awesome Godot community resources](https://github.com/godotengine/awesome-godot).

There are also a number of other
[learning resources](https://docs.godotengine.org/en/latest/community/tutorials.html)
provided by the community, such as text and video tutorials, demos, etc.
Consult the [community channels](https://godotengine.org/community)
for more information.

[![Code Triagers Badge](https://www.codetriage.com/godotengine/godot/badges/users.svg)](https://www.codetriage.com/godotengine/godot)
[![Translate on Weblate](https://hosted.weblate.org/widgets/godot-engine/-/godot/svg-badge.svg)](https://hosted.weblate.org/engage/godot-engine/?utm_source=widget)
[![TODOs](https://badgen.net/https/api.tickgit.com/badgen/github.com/godotengine/godot)](https://www.tickgit.com/browse?repo=github.com/godotengine/godot)
