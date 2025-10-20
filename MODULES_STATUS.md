# TinaGodot 模块状态说明

> **最后更新**: 2025-10-20
> **版本**: TinaGodot 2D Lite (基于 Godot 4.x)

---

## 📋 概览

TinaGodot采用**分层策略**，将功能模块分为三层：
- **核心层** - 永久保留，无条件编译
- **可选层** - 宏控制，按需编译
- **删除层** - 已删除或计划删除

---

## ✅ 核心层（永久保留）

### UI/绘制系统

| 模块 | 路径 | 说明 | 状态 |
|------|------|------|------|
| GUI控件 | `scene/gui/` | Button、Label、TextEdit等所有UI控件 | ✅ 保留 |
| 2D节点 | `scene/2d/` | Sprite2D、TileMap、Camera2D等 | ✅ 保留 |
| Canvas绘制 | `scene/main/canvas_*` | CanvasItem、CanvasLayer | ✅ 保留 |
| 场景系统 | `scene/main/` | Node、Window、Viewport、SceneTree | ✅ 保留 |
| 渲染服务器 | `servers/rendering/` | RenderingServer（2D部分） | ✅ 保留 |
| 显示服务器 | `servers/display/` | DisplayServer | ✅ 保留 |

### 动画系统

| 模块 | 路径 | 说明 | 状态 |
|------|------|------|------|
| AnimationPlayer | `scene/animation/animation_player.*` | 基础动画播放器 | ✅ 保留 |
| AnimationTree | `scene/animation/animation_tree.*` | 动画状态机 | ✅ 保留 |
| BlendSpace | `scene/animation/animation_blend_space_*` | 动画混合空间 | ✅ 保留 |
| BlendTree | `scene/animation/animation_blend_tree.*` | 动画混合树 | ✅ 保留 |
| StateMachine | `scene/animation/animation_node_state_machine.*` | 状态机 | ✅ 保留 |
| Tween | `scene/animation/tween.*` | 补间动画 | ✅ 保留 |

### 物理系统

| 模块 | 路径 | 说明 | 状态 |
|------|------|------|------|
| Physics2D | `modules/godot_physics_2d/` | 2D物理引擎 | ✅ 保留 |
| 物理节点 | `scene/2d/physics/` | RigidBody2D、CollisionShape2D等 | ✅ 保留 |
| 物理服务器 | `servers/physics_2d/` | Physics2DServer | ✅ 保留 |

### 资源和工具

| 模块 | 路径 | 说明 | 状态 |
|------|------|------|------|
| HTTPRequest | `scene/main/http_request.*` | HTTP资源加载 | ✅ 保留 |
| 资源系统 | `scene/resources/` | Texture、Font、Shader等 | ✅ 保留 |
| 主题系统 | `scene/theme/` | Theme、StyleBox | ✅ 保留 |

### 文本和字体

| 模块 | 路径 | 说明 | 状态 |
|------|------|------|------|
| FreeType | `modules/freetype/` | 字体渲染 | ✅ 保留 |
| MSDF | `modules/msdfgen/` | MSDF字体渲染 | ✅ 保留 |
| TextServer | `modules/text_server_*` | 文本服务器 | ✅ 保留 |
| 文本服务器 | `servers/text/` | TextServer | ✅ 保留 |

### 图像格式（基础）

| 模块 | 路径 | 说明 | 状态 |
|------|------|------|------|
| PNG | 内置 | PNG图像格式 | ✅ 保留 |
| JPG | `modules/jpg/` | JPEG图像格式 | ✅ 保留 |
| WebP | `modules/webp/` | WebP图像格式 | ✅ 保留 |
| BMP | `modules/bmp/` | BMP图像格式 | ✅ 保留 |
| TGA | `modules/tga/` | TGA图像格式 | ✅ 保留 |
| SVG | `modules/svg/` | SVG矢量图 | ✅ 保留 |

### 视频和相机

| 模块 | 路径 | 说明 | 状态 |
|------|------|------|------|
| VideoStreamPlayer | `scene/gui/video_stream_player.*` | 视频播放控件 | ✅ 保留 |
| Theora | `modules/theora/` | Theora视频编解码 | ✅ 保留 |
| Camera | `modules/camera/` | 物理摄像头访问 | ✅ 保留 |
| Camera服务器 | `servers/camera/` | CameraServer | ✅ 保留 |

### 核心工具

| 模块 | 路径 | 说明 | 状态 |
|------|------|------|------|
| GLSLANG | `modules/glslang/` | GLSL着色器编译器 | ✅ 保留 |
| REGEX | `modules/regex/` | 正则表达式 | ✅ 保留 |
| ZIP | `modules/zip/` | ZIP压缩解压 | ✅ 保留 |

### 第三方依赖库（核心层）

#### 文本渲染系统（约47MB）

| 依赖库 | 路径 | 大小 | 用途 | 状态 |
|--------|------|------|------|------|
| icu4c | `thirdparty/icu4c/` | 34MB | 国际化和Unicode支持 | ✅ 保留 |
| freetype | `thirdparty/freetype/` | 7.3MB | 字体渲染引擎 | ✅ 保留 |
| harfbuzz | `thirdparty/harfbuzz/` | 6.0MB | 文本整形引擎 | ✅ 保留 |
| graphite | `thirdparty/graphite/` | <1MB | 复杂脚本支持 | ✅ 保留 |
| msdfgen | `thirdparty/msdfgen/` | <1MB | MSDF字体生成 | ✅ 保留 |
| thorvg | `thirdparty/thorvg/` | <1MB | SVG渲染 | ✅ 保留 |
| fonts | `thirdparty/fonts/` | 2.8MB | 内置字体文件 | ✅ 保留 |

#### 图形渲染系统（约28MB）

| 依赖库 | 路径 | 大小 | 用途 | 状态 |
|--------|------|------|------|------|
| vulkan | `thirdparty/vulkan/` | 16MB | Vulkan图形API头文件 | ✅ 保留 |
| glslang | `thirdparty/glslang/` | 5.0MB | GLSL着色器编译器 | ✅ 保留 |
| spirv-cross | `thirdparty/spirv-cross/` | 2.3MB | SPIR-V着色器转换 | ✅ 保留 |
| spirv-reflect | `thirdparty/spirv-reflect/` | <1MB | SPIR-V反射 | ✅ 保留 |
| glad | `thirdparty/glad/` | <1MB | OpenGL加载器 | ✅ 保留 |
| angle | `thirdparty/angle/` | ~2MB | OpenGL ES支持 | ✅ 保留 |
| volk | `thirdparty/volk/` | <1MB | Vulkan加载器 | ✅ 保留 |
| amd-fsr | `thirdparty/amd-fsr/` | <1MB | AMD FSR缩放 | ✅ 保留 |
| amd-fsr2 | `thirdparty/amd-fsr2/` | <1MB | AMD FSR 2.0 | ✅ 保留 |
| smaa | `thirdparty/smaa/` | <1MB | 抗锯齿算法 | ✅ 保留 |

#### 基础图像格式（约7.6MB）

| 依赖库 | 路径 | 大小 | 用途 | 状态 |
|--------|------|------|------|------|
| libpng | `thirdparty/libpng/` | 1.4MB | PNG图像格式 | ✅ 保留 |
| libjpeg-turbo | `thirdparty/libjpeg-turbo/` | 2.1MB | JPEG图像格式 | ✅ 保留 |
| libwebp | `thirdparty/libwebp/` | 3.2MB | WebP图像格式 | ✅ 保留 |

#### 视频编解码（约1.4MB）

| 依赖库 | 路径 | 大小 | 用途 | 状态 |
|--------|------|------|------|------|
| libogg | `thirdparty/libogg/` | 0.5MB | OGG容器格式（视频用） | ✅ 保留 |
| libtheora | `thirdparty/libtheora/` | 1.4MB | Theora视频编解码 | ✅ 保留 |

#### 核心系统库（约15MB）

| 依赖库 | 路径 | 大小 | 用途 | 状态 |
|--------|------|------|------|------|
| zlib | `thirdparty/zlib/` | <1MB | ZIP压缩（被多处使用） | ✅ 保留 |
| minizip | `thirdparty/minizip/` | <1MB | ZIP文件处理 | ✅ 保留 |
| zstd | `thirdparty/zstd/` | 2.1MB | 高效压缩算法 | ✅ 保留 |
| brotli | `thirdparty/brotli/` | ~1MB | Brotli压缩 | ✅ 保留 |
| clipper2 | `thirdparty/clipper2/` | <1MB | 多边形布尔运算 | ✅ 保留 |
| pcre2 | `thirdparty/pcre2/` | 4.2MB | 正则表达式库 | ✅ 保留 |
| misc | `thirdparty/misc/` | <1MB | 杂项工具（fastlz、pcg等） | ✅ 保留 |

#### 平台支持库（约10MB）

| 依赖库 | 路径 | 大小 | 用途 | 状态 |
|--------|------|------|------|------|
| sdl | `thirdparty/sdl/` | 6.1MB | 跨平台窗口/输入 | ✅ 保留 |
| directx_headers | `thirdparty/directx_headers/` | 2.4MB | Windows DirectX头文件 | ✅ 保留 |
| d3d12ma | `thirdparty/d3d12ma/` | <1MB | DirectX 12内存分配 | ✅ 保留 |
| linuxbsd_headers | `thirdparty/linuxbsd_headers/` | 2.4MB | Linux头文件 | ✅ 保留 |
| wayland | `thirdparty/wayland/` | <1MB | Linux Wayland支持 | ✅ 保留 |
| wayland-protocols | `thirdparty/wayland-protocols/` | <1MB | Wayland协议 | ✅ 保留 |
| mingw-std-threads | `thirdparty/mingw-std-threads/` | <1MB | MinGW线程支持 | ✅ 保留 |

**核心层依赖库总计**: 约110MB

---

## 🔧 可选层（宏控制）

这些模块通过编译宏控制，默认**不编译**，用户可按需启用。

### 音频系统 (`TINAGODOT_ENABLE_AUDIO`)

#### 模块

| 模块 | 路径 | 说明 | 默认状态 |
|------|------|------|---------|
| AudioStreamPlayer | `scene/audio/` | 音频播放器 | 🔒 禁用 |
| Audio服务器 | `servers/audio/` | AudioServer | 🔒 禁用 |
| OGG | `modules/ogg/` | OGG容器格式 | 🔒 禁用 |
| Vorbis | `modules/vorbis/` | Vorbis音频编解码 | 🔒 禁用 |
| MP3 | `modules/minimp3/` | MP3音频解码 | 🔒 禁用 |
| Interactive Music | `modules/interactive_music/` | 交互式音乐 | 🔒 禁用 |

#### 第三方依赖库（约3.4MB）

| 依赖库 | 路径 | 大小 | 用途 | 默认状态 |
|--------|------|------|------|---------|
| libogg | `thirdparty/libogg/` | 0.5MB | OGG容器格式 | 🔒 禁用 |
| libvorbis | `thirdparty/libvorbis/` | 2.3MB | Vorbis音频编解码 | 🔒 禁用 |
| minimp3 | `thirdparty/minimp3/` | 0.1MB | MP3音频解码 | 🔒 禁用 |

**开关**:
```bash
# 默认关闭（精简构建），需要音频时手动开启：
scons tinagodot_audio=yes
```

### 扩展图像格式 (`TINAGODOT_ENABLE_ADVANCED_IMAGE`)

#### 模块

| 模块 | 路径 | 说明 | 默认状态 |
|------|------|------|---------|
| ASTC | `modules/astcenc/` | ASTC纹理压缩（移动端） | 🔒 禁用 |
| Basis Universal | `modules/basis_universal/` | 通用纹理压缩 | 🔒 禁用 |
| BC纹理 | `modules/bcdec/` | BC纹理解码 | 🔒 禁用 |
| Betsy | `modules/betsy/` | 纹理压缩 | 🔒 禁用 |
| CVTT | `modules/cvtt/` | CVTT纹理压缩 | 🔒 禁用 |
| DDS | `modules/dds/` | DDS格式 | 🔒 禁用 |
| ETC压缩 | `modules/etcpak/` | ETC纹理压缩 | 🔒 禁用 |
| KTX | `modules/ktx/` | KTX格式 | 🔒 禁用 |
| HDR | `modules/hdr/` | HDR图像格式 | 🔒 禁用 |
| OpenEXR | `modules/tinyexr/` | EXR格式 | 🔒 禁用 |

#### 第三方依赖库（约32MB）

| 依赖库 | 路径 | 大小 | 用途 | 默认状态 |
|--------|------|------|------|---------|
| astcenc | `thirdparty/astcenc/` | 1.2MB | ASTC纹理压缩 | 🔒 禁用 |
| basis_universal | `thirdparty/basis_universal/` | 5.4MB | 通用纹理压缩 | 🔒 禁用 |
| cvtt | `thirdparty/cvtt/` | 0.3MB | CVTT纹理压缩 | 🔒 禁用 |
| etcpak | `thirdparty/etcpak/` | 0.2MB | ETC纹理压缩 | 🔒 禁用 |
| libktx | `thirdparty/libktx/` | ~16MB | KTX格式（含vulkan头） | 🔒 禁用 |
| tinyexr | `thirdparty/tinyexr/` | 1.0MB | OpenEXR格式 | 🔒 禁用 |

**注**: `zstd`被多个模块使用，无法禁用

**启用方法**:
```bash
scons tinagodot_advanced_image=yes
```

**可选层依赖库总计**: 约35.4MB（默认不编译）

### 调试工具 (`TINAGODOT_ENABLE_PROFILER`)

| 模块 | 路径 | 说明 | 默认状态 |
|------|------|------|---------|
| ObjectDB Profiler | `modules/objectdb_profiler/` | 对象数据库性能分析 | 🔒 禁用 |

**启用方法**:
```bash
scons tinagodot_profiler=yes
```

---

## ❌ 删除层

这些模块已删除或计划删除，无法恢复。

### 已删除（第一阶段清理）

| 模块 | 原路径 | 删除理由 | 删除时间 |
|------|--------|---------|---------|
| 3D渲染系统 | `scene/3d/` | TinaGodot专注2D | ✅ 已删除 |
| 3D物理 | `servers/physics_3d/` | 无3D需求 | ✅ 已删除 |
| XR/VR | `servers/xr/` | 无VR需求 | ✅ 已删除 |
| Jolt Physics | `thirdparty/jolt_physics/` | 3D物理引擎 | ✅ 已删除 |
| OpenXR | `thirdparty/openxr/` | XR支持 | ✅ 已删除 |
| GDScript | `modules/gdscript/` | 移除脚本系统 | ✅ 已删除 |
| FBX导入 | `modules/fbx/` | 3D格式 | ✅ 已删除 |

### 已删除（第二阶段清理）✅

#### 模块删除清单

| 模块 | 路径 | 删除理由 | 模块大小 | 状态 |
|------|------|---------|---------|------|
| ENet | `modules/enet/` | UI不需要网络库 | ~0.2MB | ✅ 已删除 |
| Multiplayer | `modules/multiplayer/` | UI不需要多人游戏 | ~2MB | ✅ 已删除 |
| WebSocket | `modules/websocket/` | UI不需要WebSocket | ~0.5MB | ✅ 已删除 |
| UPnP | `modules/upnp/` | UI不需要端口映射 | ~0.3MB | ✅ 已删除 |
| MBEDTLS | `modules/mbedtls/` | UI不需要TLS加密 | ~1MB | ✅ 已删除 |
| Navigation2D | `modules/navigation_2d/` | UI不需要寻路 | ~1.5MB | ✅ 已删除 |
| MeshOptimizer | `modules/meshoptimizer/` | 3D工具 | ~0.3MB | ✅ 已删除 |
| XAtlas | `modules/xatlas_unwrap/` | 3D UV展开 | ~0.2MB | ✅ 已删除 |
| Noise | `modules/noise/` | UI不需要噪声生成 | ~0.2MB | ✅ 已删除 |
| JSONRPC | `modules/jsonrpc/` | UI不需要远程调用 | ~0.2MB | ✅ 已删除 |

**模块代码减少**: 约7.7MB（已完成）

**执行情况**：
- ✅ 已物理删除所有模块代码目录（提交：b4248e7843）
- ✅ 已创建第三方依赖清理脚本（提交：fc52d4760e）
  - `tools/purge_second_stage.bat` 或 `tools/purge_second_stage.ps1`
  - 将删除：`thirdparty/enet`、`thirdparty/mbedtls`、`thirdparty/miniupnpc`、`thirdparty/wslay`、`thirdparty/meshoptimizer`、`thirdparty/xatlas`、`thirdparty/recastnavigation`、`thirdparty/rvo2`
  - **使用方法**：本地运行脚本后提交
  - **提交命令**：`git add -A && git commit -m "chore(purge-thirdparty): 移除网络/导航/3D工具相关依赖"`

#### 第三方依赖库删除清单

| 依赖库 | 路径 | 用途 | 大小 | 状态 |
|--------|------|------|------|------|
| **网络相关** | | | **8.3MB** | |
| enet | `thirdparty/enet/` | ENet网络库 | 0.2MB | 🔄 待清理 |
| mbedtls | `thirdparty/mbedtls/` | TLS/SSL加密（最大） | 7.7MB | 🔄 待清理 |
| wslay | `thirdparty/wslay/` | WebSocket实现 | 0.1MB | 🔄 待清理 |
| miniupnpc | `thirdparty/miniupnpc/` | UPnP端口映射 | 0.3MB | 🔄 待清理 |
| **导航相关** | | | **1.5MB** | |
| rvo2 | `thirdparty/rvo2/` | 动态避障算法 | 0.8MB | 🔄 待清理 |
| recastnavigation | `thirdparty/recastnavigation/` | 寻路网格 | ~0.7MB | 🔄 待清理 |
| **3D工具** | | | **0.8MB** | |
| meshoptimizer | `thirdparty/meshoptimizer/` | 网格优化 | 0.5MB | 🔄 待清理 |
| xatlas | `thirdparty/xatlas/` | UV展开 | 0.3MB | 🔄 待清理 |

**说明**：第三方依赖库需要本地运行清理脚本后手动提交，避免误删共享依赖。

**依赖库减少**: 约10.8MB（待清理脚本执行）

**第二阶段已完成减少**: 约7.7MB（模块代码） + 待清理10.8MB（依赖库）= **总计18.5MB**

---

## 📊 体积对比

### 整体体积分析

| 配置 | 模块数量 | 代码大小 | 依赖库大小 | 总大小 | 对比原版 |
|------|---------|---------|-----------|--------|---------|
| **最小版** | 20个 | ~40MB | ~110MB | ~150MB | -0% |
| **推荐版** | +音频(6个) | ~40MB | ~110MB | ~150MB | -0% |
| **完整版** | +扩展图像(10个) | ~40MB | ~145MB | ~185MB | +23% |
| **原版Godot** | 50+个 | ~50MB | ~200MB | ~250MB | 基准 |

**说明**:
- 第一阶段已删除3D相关约258MB（主要是thirdparty/jolt_physics等）
- 第二阶段可再删除约18.5MB（网络+导航+3D工具）
- 宏控制可选模块约35.4MB（默认不编译）

### 各层体积构成

| 层级 | 模块代码 | 依赖库 | 总计 | 说明 |
|------|---------|--------|------|------|
| **核心层** | ~40MB | ~110MB | ~150MB | 必须保留 |
| **可选层（音频）** | ~3MB | ~3.4MB | ~6.4MB | 宏控制 |
| **可选层（图像）** | ~5MB | ~32MB | ~37MB | 宏控制 |
| **删除层** | ~7.7MB | ~10.8MB | ~18.5MB | 计划删除 |

**核心层依赖库构成**（110MB）:
- 文本渲染系统: 47MB（icu4c、freetype、harfbuzz等）
- 图形渲染系统: 28MB（vulkan、glslang、spirv等）
- 核心系统库: 15MB（zlib、zstd、pcre2等）
- 平台支持库: 10MB（sdl、directx_headers等）
- 基础图像格式: 7.6MB（png、jpg、webp）
- 视频编解码: 1.4MB（theora、libogg）

---

## 🚀 编译选项

### 最小版本（仅UI框架，默认）
```bash
scons platform=windows target=editor \
  tinagodot_audio=no \
  tinagodot_advanced_image=no \
  tinagodot_profiler=no
```

### 推荐版本（UI + 音频）
```bash
scons platform=windows target=editor \
  tinagodot_audio=yes \
  tinagodot_advanced_image=no \
  tinagodot_profiler=no
```

### 完整版本（所有功能）
```bash
scons platform=windows target=editor \
  tinagodot_audio=yes \
  tinagodot_advanced_image=yes \
  tinagodot_profiler=yes
```

---

## 📝 实施状态

| 阶段 | 任务 | 状态 | 完成时间 |
|------|------|------|---------|
| ✅ 阶段0 | 3D功能删除 | 完成 | 2025-10-18 |
| ✅ 阶段1 | 物理删除网络/导航模块 | 完成 | 2025-10-20 |
| 🔄 阶段2 | 第三方依赖清理（脚本） | 待执行 | 待定 |
| 📋 阶段3 | 建立宏控制框架 | 计划中 | 1-2天 |
| 📋 阶段4 | 音频系统宏化 | 计划中 | 2-3天 |
| 📋 阶段5 | 扩展图像格式宏化 | 计划中 | 1-2天 |
| 🔄 阶段6 | 文档更新和中文注释 | 进行中 | 1-2天 |

---

## 🤔 决策理由

### 为什么保留VideoStreamPlayer和Camera？
- **理由**: 已有代码，未来可能需要（视频会议、AR界面）
- **成本**: 约5MB，对核心功能无影响
- **用户需求**: 明确需要保留

### 为什么音频改为宏控制而非删除？
- **理由**: UI常需要音效反馈（按钮点击声、提示音）
- **方案**: 默认禁用，用户按需启用
- **优势**: 避免未来重新集成的工作量

### 为什么直接删除网络和导航？
- **理由**: UI框架完全不需要多人游戏和寻路功能
- **替代**: HTTPRequest已足够处理资源加载
- **收益**: 减少50-70MB代码和依赖

---

## 📚 相关文档

- [README.md](README.md) - 项目概览
- [UI架构文档](docs/ui-architecture-zh.md) - UI系统详细说明
- [功能模块分析](.claude/context-summary-功能模块分析.md) - 详细的模块分析报告
- [3D清理报告](3D_CLEANUP_REPORT.md) - 第一阶段清理记录

---

**生成时间**: 2025-10-20
**维护者**: TinaGodot Team
