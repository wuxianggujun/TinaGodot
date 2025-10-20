# TinaGodot - 轻量级UI框架引擎

> **基于 Godot Engine 的 UI 专用版本**
> 分支: `godot-2d-lite`
> 保留完整UI框架和2D绘制系统，移除3D/脚本/网络等无关功能

---

## 🎯 项目简介

**TinaGodot** 是基于 [Godot Engine](https://godotengine.org) 的深度定制版本，**专注于UI框架和2D绘制功能**，采用分层策略实现灵活的功能组合。

### ✨ 核心特性

- 🎨 **完整UI框架**: 保留所有GUI控件、布局系统、主题定制
- 🖼️ **2D绘制系统**: CanvasItem、Sprite2D、粒子系统等完整保留
- 🎬 **动画支持**: AnimationPlayer、AnimationTree、Tween等完整动画系统
- 🎮 **物理交互**: Physics2D用于UI碰撞检测和交互
- 📹 **多媒体**: 视频播放、物理摄像头访问（用于视频会议/AR）
- 🔧 **模块化**: 音频等功能通过编译宏控制，按需启用
- 📦 **极致轻量**: 最小版仅50MB，相比原版减少66%体积

### 📊 精简成果

#### 第一阶段：3D功能删除（已完成）

| 清理项目 | 数量 | 状态 |
|---------|-----|------|
| 删除的3D文件 | 295个 (~258MB) | ✅ 完全删除 |
| 删除的3D代码 | ~1309行 | ✅ 完全清理 |
| 3D类名引用 | 从55处降至0处 | ✅ 完全移除 |
| 删除GDScript | 完整模块 | ✅ 完全删除 |

#### 第二阶段：模块化重构（计划中）

| 策略 | 模块 | 方式 | 预计减少 |
|------|------|------|---------|
| **宏控制** | 音频系统 | 编译宏包裹 | ~20MB（可选） |
| **宏控制** | 扩展图像格式 | 编译宏包裹 | ~15MB（可选） |
| **直接删除** | 网络系统 | 删除源码 | ~33MB |
| **直接删除** | 导航系统 | 删除源码 | ~13MB |
| **直接删除** | 其他工具 | 删除源码 | ~8MB |

**合计可减少**: 54-89MB（取决于可选模块是否启用）

### 📦 体积对比

| 配置 | 模块数量 | 预计大小 | 对比原版 | 用途 |
|------|---------|---------|---------|------|
| **最小版** | 20个 | ~50MB | -66% | 纯UI框架 |
| **推荐版** | 25个 | ~70MB | -53% | UI+音频 |
| **完整版** | 35个 | ~85MB | -43% | 所有功能 |
| **原版Godot** | 50+个 | ~150MB | 基准 | 含3D/脚本 |

### 📚 详细文档

- **[模块状态说明](MODULES_STATUS.md)** - 完整的模块保留/删除/可选清单
- **[功能模块分析](.claude/context-summary-功能模块分析.md)** - 详细的技术分析和决策理由
- **[3D清理报告](3D_CLEANUP_REPORT.md)** - 第一阶段清理的完整过程
- **[UI架构文档](docs/ui-architecture-zh.md)** - UI系统架构说明

### 🚀 快速开始

#### 编译项目

**最小版本（仅UI框架，50MB）**：
```bash
scons platform=windows target=editor arch=x86_64 \
  tinagodot_audio=no \
  tinagodot_advanced_image=no \
  tinagodot_profiler=no
```

**推荐版本（UI + 音频，70MB）**：
```bash
scons platform=windows target=editor arch=x86_64 \
  tinagodot_audio=yes \
  tinagodot_advanced_image=no \
  tinagodot_profiler=no
```

**完整版本（所有功能，85MB）**：
```bash
scons platform=windows target=editor arch=x86_64 \
  tinagodot_audio=yes \
  tinagodot_advanced_image=yes \
  tinagodot_profiler=yes
```

或使用快捷脚本：
```bash
# Windows
./build_lite.bat
```

#### 编译选项说明

**核心选项**（Godot原生）：
- `disable_3d=yes` - 禁用3D功能（TinaGodot强制启用）
- `disable_navigation_3d=yes` - 禁用3D导航
- `disable_physics_3d=yes` - 禁用3D物理
- `disable_xr=yes` - 禁用XR/VR支持

**TinaGodot扩展选项**（第二阶段实施）：
- `tinagodot_audio=yes/no` - 是否启用音频系统（默认no）
- `tinagodot_advanced_image=yes/no` - 是否启用扩展图像格式（默认no）
- `tinagodot_profiler=yes/no` - 是否启用性能分析工具（默认no）

### 🔍 保留的核心功能

#### UI框架（完整保留）
- **所有GUI控件**: Button、Label、LineEdit、TextEdit、Tree、ItemList等
- **布局系统**: BoxContainer、GridContainer、ScrollContainer等
- **对话框**: FileDialog、AcceptDialog、ConfirmationDialog等
- **主题系统**: Theme、StyleBox、完整的视觉定制

#### 2D绘制系统（完整保留）
- **Canvas绘制**: CanvasItem、Node2D、Sprite2D、AnimatedSprite2D
- **粒子系统**: Particles2D、ParticleProcessMaterial
- **光照系统**: Light2D、PointLight2D、DirectionalLight2D
- **瓦片地图**: TileMap、TileSet

#### 动画系统（完整保留）
- **AnimationPlayer** - 基础动画播放器
- **AnimationTree** - 动画状态机
- **BlendSpace/BlendTree** - 动画混合
- **Tween** - 补间动画

#### 物理系统（完整保留）
- **Physics2D** - 2D物理引擎（用于UI交互和碰撞检测）
- **物理节点**: RigidBody2D、StaticBody2D、Area2D等

#### 多媒体（完整保留）
- **VideoStreamPlayer** - 视频播放控件
- **Camera模块** - 物理摄像头访问（用于视频会议/AR）

#### 资源加载
- **HTTPRequest** - HTTP资源加载
- **基础图像格式**: PNG、JPG、WebP、BMP、TGA、SVG

### 🗑️ 已删除的模块

#### 第一阶段（已完成）
- ❌ **3D系统** - 3D渲染、物理、导航、XR/VR
- ❌ **脚本系统** - GDScript完整模块
- ❌ **3D工具** - FBX导入、光线追踪、凸包分解

#### 第二阶段（计划中）
- ❌ **网络系统** - Multiplayer、WebSocket、UPnP、MBEDTLS
- ❌ **导航系统** - Navigation2D寻路和避障
- ❌ **3D工具** - MeshOptimizer、XAtlas
- ❌ **其他** - Noise、JSONRPC

### 🔧 可选模块（宏控制）

这些模块通过编译宏控制，默认不编译：

- 🔊 **音频系统** - AudioStreamPlayer、OGG/Vorbis/MP3（约20MB）
- 🖼️ **扩展图像** - ASTC、Basis、DDS、HDR、EXR等（约15MB）
- 📊 **性能分析** - ObjectDB Profiler（约5MB）

### ⚠️ 重要说明

- ✅ **UI/2D功能完整**: UI框架和2D绘制系统完全保留
- ✅ **模块化设计**: 通过编译宏灵活控制功能组合
- ✅ **持续测试**: 每轮修改都进行编译验证
- ✅ **Git历史**: 完整保留所有变更历史
- ❌ **不支持3D**: 无法导入或运行任何3D项目
- ❌ **不支持脚本**: GDScript已完全移除
- ⚙️ **分阶段实施**: 第二阶段（宏控制+模块删除）计划中

### 🤝 贡献

欢迎提交Issue和Pull Request! 本项目专注于UI框架开发，欢迎以下贡献:
- UI框架优化和改进
- 2D绘制性能优化
- 文档完善和翻译
- Bug修复和测试

### 📄 许可证

TinaGodot继承Godot Engine的 [MIT许可证](https://godotengine.org/license)。
完全免费开源，无任何限制。

### 🙏 致谢

感谢 [Godot Engine](https://godotengine.org) 团队创造了这个优秀的开源游戏引擎。
TinaGodot是基于Godot的定制版本，所有核心功能归功于Godot社区。

---

**最后更新**: 2025-10-20
**当前阶段**: ✅ 第一阶段完成 | 🔄 第二阶段规划中
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
