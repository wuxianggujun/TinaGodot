# TinaGodot 源码目录结构详细分析

## 概览

TinaGodot 是基于 Godot 4.x 的 2D 专注引擎版本，源码采用模块化架构设计，主要分为核心系统、场景系统、服务器系统、编辑器系统和平台适配层。

---

## 📁 核心目录结构 (Core Directories)

### 1. **core/** - 引擎核心系统
**作用**: 提供引擎的底层基础设施和核心功能，所有其他系统都依赖于此。

**主要子模块**:

#### **core/io/** - 输入输出系统
- 文件访问: `file_access.cpp`, `dir_access.cpp`
- 资源加载/保存: `resource_loader.cpp`, `resource_saver.cpp`
- 网络通信: `http_client.cpp`, `packet_peer.cpp`, `tcp_server.cpp`
- 图片处理: `image.cpp`, `image_loader.cpp`
- 数据压缩: `compression.cpp`, `file_access_compressed.cpp`
- 序列化: `json.cpp`, `marshalls.cpp`, `xml_parser.cpp`

#### **core/object/** - 对象系统
- 对象基类: `object.cpp` - 所有 Godot 对象的基类
- 类型系统: `class_db.cpp` - 类注册和反射系统
- 脚本系统: `script_language.cpp`, `script_instance.cpp`
- 方法绑定: `method_bind.cpp` - C++ 到脚本的桥接
- 引用计数: `ref_counted.cpp` - 自动内存管理
- 消息队列: `message_queue.cpp` - 延迟调用系统
- 线程池: `worker_thread_pool.cpp`

#### **core/math/** - 数学库
- 基础类型: `vector2.cpp`, `vector3.cpp`, `vector4.cpp`
- 变换: `transform_2d.cpp`, `transform_3d.cpp`
- 几何: `rect2.cpp`, `aabb.cpp`, `plane.cpp`
- 旋转: `quaternion.cpp`, `basis.cpp`
- 算法: `geometry_2d.cpp`, `geometry_3d.cpp`
- 随机数: `random_pcg.cpp`, `random_number_generator.cpp`
- 寻路: `a_star.cpp`, `a_star_grid_2d.cpp`
- 空间结构: `bvh.h` (Bounding Volume Hierarchy), `dynamic_bvh.cpp`

#### **core/config/** - 配置系统
- 项目设置: `project_settings.cpp`
- 引擎配置: `engine.cpp`

#### **core/string/** - 字符串系统
- 国际化: `translation.cpp`, `translation_server.cpp`
- 字符串处理: `ustring.cpp`

#### **core/crypto/** - 加密系统
- 加密算法: `crypto.cpp`, `hashing_context.cpp`

#### **core/debugger/** - 调试器
- 引擎调试: `engine_debugger.cpp`
- 性能分析: `engine_profiler.cpp`

#### **core/extension/** - 扩展系统
- GDExtension: 允许 C/C++ 扩展
- 扩展管理: `gdextension_manager.cpp`

#### **core/input/** - 输入系统
- 输入处理: `input.cpp`
- 输入映射: `input_map.cpp`

#### **core/os/** - 操作系统抽象层
- 线程: `thread.cpp`, `mutex.cpp`, `semaphore.cpp`
- 时间: `time.cpp`
- 内存: `memory.cpp`
- OS接口: `os.cpp`

---

### 2. **scene/** - 场景系统
**作用**: 提供场景树节点、UI 控件、资源管理等游戏对象系统。

**主要子模块**:

#### **scene/2d/** - 2D 节点系统
- 基础节点: `node_2d.cpp` - 所有 2D 节点的基类
- 精灵: `sprite_2d.cpp`, `animated_sprite_2d.cpp`
- 相机: `camera_2d.cpp`
- 粒子: `cpu_particles_2d.cpp`, `gpu_particles_2d.cpp`
- 光照: `light_2d.cpp`, `light_occluder_2d.cpp`
- 音频: `audio_stream_player_2d.cpp`
- 网格: `mesh_instance_2d.cpp`, `multimesh_instance_2d.cpp`
- 瓦片地图: `tile_map.cpp`, `tile_map_layer.cpp`
- 路径: `path_2d.cpp`
- 骨骼: `skeleton_2d.cpp`
- 物理: `scene/2d/physics/` - 碰撞体、刚体等
- 导航: `scene/2d/navigation/` - 导航代理、区域等

#### **scene/gui/** - UI 控件系统
- 基础控件: `control.cpp` - 所有 UI 控件的基类
- 容器: `container.cpp`, `box_container.cpp`, `grid_container.cpp`
- 按钮: `button.cpp`, `check_button.cpp`, `option_button.cpp`
- 文本: `label.cpp`, `line_edit.cpp`, `text_edit.cpp`, `rich_text_label.cpp`
- 列表: `item_list.cpp`, `tree.cpp`
- 对话框: `dialogs.cpp`, `file_dialog.cpp`
- 进度条: `progress_bar.cpp`, `texture_progress_bar.cpp`
- 滑块: `slider.cpp`, `scroll_bar.cpp`
- 标签页: `tab_bar.cpp`, `tab_container.cpp`
- 图表: `graph_edit.cpp`, `graph_node.cpp`
- 颜色选择器: `color_picker.cpp`
- 代码编辑器: `code_edit.cpp`

#### **scene/main/** - 场景核心
- 场景树: `scene_tree.cpp` - 管理整个节点树
- 节点基类: `node.cpp` - 所有节点的基类
- 窗口: `window.cpp` - 窗口管理
- 视口: `viewport.cpp` - 渲染视口
- 计时器: `timer.cpp`

#### **scene/animation/** - 动画系统
- 动画播放器: `animation_player.cpp`
- 动画树: `animation_tree.cpp`
- 动画混合器: `animation_mixer.cpp`
- 补间动画: `tween.cpp`

#### **scene/audio/** - 音频节点
- 音频播放器: `audio_stream_player.cpp`
- 音频总线: 音频效果处理

#### **scene/resources/** - 资源系统
- 材质: `material.cpp`, `canvas_item_material.cpp`
- 纹理: `texture.cpp`, `image_texture.cpp`, `atlas_texture.cpp`
- 网格: `mesh.cpp`, `immediate_mesh.cpp`
- 字体: `font.cpp`
- 动画: `animation.cpp`, `animation_library.cpp`
- 着色器: `shader.cpp`, `visual_shader.cpp`
- 主题: `theme.cpp`
- 打包场景: `packed_scene.cpp`
- 导航: `navigation_mesh.cpp`
- 2D资源: `scene/resources/2d/` - 多边形、线段等

#### **scene/theme/** - 主题系统
- 主题数据库: `theme_db.cpp`
- 默认主题: `default_theme.cpp`
- 图标: `scene/theme/icons/`

---

### 3. **servers/** - 服务器系统
**作用**: 提供底层渲染、物理、音频等核心服务的实现。

**主要子模块**:

#### **servers/rendering/** - 渲染服务器
- 渲染服务器: `rendering_server_default.cpp`
- 场景渲染: `renderer_scene_cull.cpp`, `renderer_scene_render.cpp`
- 视口渲染: `renderer_viewport.cpp`
- 画布渲染: `renderer_canvas_cull.cpp`, `renderer_canvas_render.cpp`
- 存储系统: `storage/` - 纹理、网格、材质存储
- 渲染后端:
  - `servers/rendering/renderer_rd/` - RenderingDevice 后端
  - `drivers/gles3/` - OpenGL ES 3.0 后端
  - `drivers/vulkan/` - Vulkan 后端
  - `drivers/d3d12/` - DirectX 12 后端
  - `drivers/metal/` - Metal 后端

#### **servers/physics_2d/** - 2D 物理服务器
- 物理服务器: `physics_server_2d.cpp`
- 物理实现: `godot_physics_2d/` - Godot 自带物理引擎
  - 空间: `godot_space_2d.cpp`
  - 刚体: `godot_body_2d.cpp`
  - 区域: `godot_area_2d.cpp`
  - 碰撞: `godot_collision_solver_2d.cpp`

#### **servers/navigation_2d/** - 2D 导航服务器
- 导航服务器: `navigation_server_2d.cpp`
- 路径查找和导航网格处理

#### **servers/audio/** - 音频服务器
- 音频服务器: `audio_server.cpp`
- 音频驱动: `audio_driver_dummy.cpp`
- 音频效果: `effects/` - 混响、延迟等

#### **servers/text/** - 文本服务器
- 文本服务器: `text_server.cpp`
- 文本排版和渲染

#### **servers/display/** - 显示服务器
- 显示服务器: `display_server.cpp`
- 窗口管理、输入事件处理

#### **servers/camera/** - 相机服务器
- 相机管理: `camera_server.cpp`

#### **servers/movie_writer/** - 视频导出
- 视频录制: `movie_writer.cpp`

---

### 4. **editor/** - 编辑器系统
**作用**: 提供 Godot 编辑器的所有功能和 UI。

**主要子模块**:

#### **editor/** (根目录)
- 编辑器主节点: `editor_node.cpp` - 编辑器入口
- 编辑器数据: `editor_data.cpp`
- 编辑器接口: `editor_interface.cpp`
- 编辑器设置: `editor_settings.cpp`

#### **editor/scene/** - 场景编辑
- 场景树编辑: `scene_tree_editor.cpp`
- 2D 编辑器: `scene/gui/`
- 纹理编辑: `texture/`

#### **editor/inspector/** - 属性检查器
- 属性编辑: `editor_inspector.cpp`
- 自定义编辑器: 各种属性编辑器

#### **editor/import/** - 资源导入
- 导入系统: 各种资源导入器

#### **editor/docks/** - 编辑器停靠面板
- 文件系统: `filesystem_dock.cpp`
- 场景树: `scene_tree_dock.cpp`

#### **editor/plugins/** - 编辑器插件
- 各种节点的编辑器插件

#### **editor/debugger/** - 调试器
- 调试器节点: `editor_debugger_node.cpp`
- 远程调试: 脚本调试、性能分析

#### **editor/project_manager/** - 项目管理器
- 项目列表和创建

#### **editor/themes/** - 编辑器主题
- 编辑器图标: `icons/`
- 编辑器字体: `editor_fonts.cpp`

#### **editor/export/** - 导出系统
- 导出预设管理

---

### 5. **platform/** - 平台适配层
**作用**: 为不同操作系统和平台提供适配实现。

**平台目录**:
- `platform/windows/` - Windows 平台
- `platform/linuxbsd/` - Linux/BSD 平台
- `platform/macos/` - macOS 平台
- `platform/android/` - Android 平台
- `platform/ios/` - iOS 平台
- `platform/web/` - Web (Emscripten) 平台
- `platform/visionos/` - Apple Vision Pro 平台

**每个平台包含**:
- 显示服务器实现
- OS 接口实现
- 平台特定功能
- 导出插件

---

### 6. **modules/** - 可选模块
**作用**: 可编译/禁用的功能模块。

**保留的 2D 相关模块**:
- `modules/godot_physics_2d/` - 2D 物理引擎
- `modules/freetype/` - 字体渲染
- `modules/svg/` - SVG 支持
- `modules/basis_universal/` - 纹理压缩
- `modules/enet/` - 网络库
- `modules/regex/` - 正则表达式
- `modules/zip/` - ZIP 压缩

**已删除的 3D 模块**:
- ~~modules/fbx/~~ - FBX 导入
- ~~modules/vhacd/~~ - 凸包分解
- ~~modules/gdscript/~~ - GDScript

---

### 7. **drivers/** - 硬件驱动抽象
**作用**: 提供图形、音频等硬件驱动的抽象层。

**子目录**:
- `drivers/gles3/` - OpenGL ES 3.0 渲染后端
- `drivers/vulkan/` - Vulkan 渲染后端
- `drivers/d3d12/` - DirectX 12 后端
- `drivers/metal/` - Metal 后端
- `drivers/alsa/` - ALSA 音频 (Linux)
- `drivers/pulseaudio/` - PulseAudio (Linux)
- `drivers/wasapi/` - WASAPI 音频 (Windows)
- `drivers/coreaudio/` - CoreAudio (macOS)
- `drivers/xaudio2/` - XAudio2 (Windows)
- `drivers/png/` - PNG 图片
- `drivers/unix/` - Unix 平台通用代码
- `drivers/windows/` - Windows 平台驱动

---

### 8. **thirdparty/** - 第三方库
**作用**: 包含所有第三方依赖库的源码。

**主要第三方库**:

#### 图形和渲染:
- `glslang/` - GLSL 着色器编译器
- `vulkan/` - Vulkan SDK
- `angle/` - ANGLE (OpenGL ES to D3D)
- `glad/` - OpenGL 加载器
- `libktx/` - KTX 纹理格式

#### 图片处理:
- `libpng/` - PNG 编解码
- `libjpeg-turbo/` - JPEG 编解码
- `basis_universal/` - 纹理压缩
- `astcenc/` - ASTC 纹理压缩
- `etcpak/` - ETC 纹理压缩
- `cvtt/` - BC7 压缩

#### 字体和文本:
- `freetype/` - 字体栅格化
- `harfbuzz/` - 文本排版
- `graphite/` - 复杂字体支持
- `icu4c/` - Unicode 支持
- `fonts/` - 内置字体文件

#### 音频:
- `libogg/` - Ogg 容器
- `libvorbis/` - Vorbis 音频解码
- `libtheora/` - Theora 视频解码

#### 网络:
- `enet/` - 可靠 UDP 网络库
- `mbedtls/` - TLS/SSL 库

#### 压缩:
- `brotli/` - Brotli 压缩
- `zlib/` - zlib 压缩
- `zstd/` - Zstandard 压缩

#### 其他:
- `doctest/` - 单元测试框架
- `clipper2/` - 2D 多边形裁剪
- `recastnavigation/` - 导航网格
- `tinyexr/` - EXR 图片格式
- `thorvg/` - SVG 渲染

---

### 9. **main/** - 引擎入口
**作用**: 引擎启动和主循环。

**关键文件**:
- `main.cpp` - 引擎 main() 函数入口
- `main_timer_sync.cpp` - 帧率同步
- `performance.cpp` - 性能监控

---

### 10. **misc/** - 杂项工具
**作用**: 构建脚本、工具和配置。

**子目录**:
- `misc/dist/` - 发布相关文件
- `misc/scripts/` - Python 脚本工具
- `misc/utility/` - 构建工具

---

### 11. **tests/** - 测试系统
**作用**: 单元测试和功能测试。

**测试类型**:
- 核心测试: `tests/core/`
- 场景测试: `tests/scene/`
- 服务器测试: `tests/servers/`

---

### 12. **doc/** - 文档系统
**作用**: API 文档的 XML 源文件。

**内容**:
- `doc/classes/` - 类文档 (XML)
- `doc/translations/` - 文档翻译 (仅保留中文)

---

### 13. **bin/** - 二进制输出
**作用**: 编译后的可执行文件和库。

**输出**:
- `godot.windows.editor.x86_64.exe` - Windows 编辑器
- `godot.windows.editor.dev.x86_64.exe` - Windows Debug 版本

---

## 🔧 构建系统

### SCons 构建文件:
- `SConstruct` - 主构建脚本
- `*/SCsub` - 各模块的子构建脚本

### Python 构建器:
- `methods.py` - 构建辅助函数
- `*_builders.py` - 各模块的代码生成器
- `platform_methods.py` - 平台相关构建方法

### Visual Studio:
- `godot.sln` - VS 解决方案
- `generate_vs_solution.bat` - 生成 VS 项目

### 自定义构建脚本:
- `build_lite.bat` - 精简版构建
- `build_editor_debug.bat` - Debug 版本
- `build_editor_release.bat` - Release 版本

---

## 📊 TinaGodot 2D Lite 特化

### 已移除的 3D 目录:
- ~~scene/3d/~~ - 所有 3D 节点
- ~~editor/scene/3d/~~ - 3D 编辑器
- ~~servers/physics_3d/~~ - 3D 物理
- ~~servers/xr/~~ - XR/VR 支持
- ~~thirdparty/jolt_physics/~~ - 3D 物理引擎
- ~~thirdparty/openxr/~~ - OpenXR SDK

### 资源优化:
- 字体: 仅保留 5 个 (英文、代码、中文、日文)
- 翻译: 仅保留 3 种中文变体 (zh_CN, zh_TW, zh_HK)
- 文档: 完全禁用打包

---

## 💡 目录架构设计理念

### 分层架构:
1. **Core Layer** (core/) - 底层基础设施
2. **Server Layer** (servers/) - 核心服务实现
3. **Scene Layer** (scene/) - 游戏对象系统
4. **Editor Layer** (editor/) - 工具和编辑器
5. **Platform Layer** (platform/) - 平台适配

### 模块化设计:
- 每个模块可独立编译
- 使用 `*_DISABLED` 宏条件编译
- 通过 SCons 参数控制模块启用/禁用

### 插件系统:
- GDExtension: C/C++ 扩展
- Editor Plugins: 编辑器插件
- Module System: 编译时模块

---

**生成时间**: 2025-10-22
**基于版本**: TinaGodot 2D Lite (Godot 4.x fork)
**总目录数**: 100+ 个主要目录
**总文件数**: 10,000+ 个源文件
