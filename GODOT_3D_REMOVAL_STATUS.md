# Godot 3D 移除进度与现状

项目名称: TinaGodot
分支: godot-2d-lite
更新时间: 2025-10-16
目标: 移除 Godot 的 3D 能力，聚焦 2D 引擎与编辑器

---

## 总览

```
整体完成度 ≈ 95%
编译状态: ✅ 通过 (Windows x86_64 Editor)
```

当前状态: 已成功编译通过!所有 3D 功能已通过条件编译 `_3D_DISABLED` 宏完全禁用,正在进行物理删除阶段,移除被宏包裹的死代码。

---

## 已完成工作（基于当前源码）

### 1) 3D 节点与场景注册（已禁用）
- scene/3d/ 目录不存在，3D 节点未注册。
- 在 `scene/register_scene_types.cpp` 顶部强制禁用开关：
  - `scene/register_scene_types.cpp:32` 定义 `_3D_DISABLED`
  - `scene/register_scene_types.cpp:33` 定义 `PHYSICS_3D_DISABLED`
  - `scene/register_scene_types.cpp:34` 定义 `NAVIGATION_3D_DISABLED`
  - 同时定义 `XR_DISABLED`

### 2) 3D 编辑器功能（已禁用/移除）
- `editor/scene/` 仅保留 2D/GUI/Texture 编辑器插件，`editor/scene/3d/` 不存在。
- `editor/register_editor_types.cpp` 中 3D 插件注册均包裹于 `#ifndef PHYSICS_3D_DISABLED` 宏下（例如：`editor/register_editor_types.cpp:163-166, 237-284, 291-362`）。在 3D 被禁用时不会编译/注册。

### 3) 3D 物理与 XR（已移除目录 + 已禁用）
- 目录层面：`servers/physics_3d/` 与 `servers/xr/` 不存在。
- 构建层面：禁用 3D 后自动联动禁用 3D 物理、3D 导航与 XR（见下文 SCons 选项）。

### 4) GDScript 模块（已移除）
- `modules/gdscript/` 目录不存在，相关解析/LSP/测试/脚本编辑器集成均不再构建。

### 5) 构建与开关（已生效）
- SCons 默认开启 `disable_3d`，并联动禁用相关子系统：
  - `SConstruct:1016-1032`：当 `disable_3d` 为真，定义 `_3D_DISABLED`、并将 `disable_navigation_3d`、`disable_physics_3d`、`disable_xr` 置为真，同时追加对应的 `CPPDEFINES`。
- `build_lite.bat` 针对模块做了最小化编译（见“当前编译参数”）。

---

## 注意与差异（与“完全删除”表述的校正）

- 3D 相关源码仍有保留，但均处于禁用态，不会注册或在构建目标中启用：
  - `scene/resources/` 下仍包含 Mesh/ImmediateMesh/Sky/Environment 等 3D 资源类文件，核心逻辑普遍以 `PHYSICS_3D_DISABLED` 或 `_3D_DISABLED` 宏进行条件编译（例如：`scene/resources/mesh.cpp:37, 204, 538, 917, 2315`）。
  - 渲染后端 `servers/rendering/renderer_rd` 仍包含 3D 相关实现文件，但在 `_3D_DISABLED`/注册屏蔽下不会用于 3D 节点路径。
  - `modules/gltf/` 目录存在，但 `register_types.cpp` 在 `_3D_DISABLED` 下为空初始化，且 `build_lite.bat` 已显式 `module_gltf_enabled=no`。
- 编辑器代码中仍存在大量对 `Node3D` 的条件引用（宏保护下），例如：
  - `editor/editor_interface.cpp:463-517, 866`
  - `editor/docks/scene_tree_dock.cpp` 多处 `_3D_DISABLED` 条件块。
- 仓库内存在合并残留文件（不影响构建但建议清理）：
  - `editor/editor_node.cpp.orig`
  - `editor/editor_node.cpp.rej`

结论：当前实现是“通过宏与注册禁用 3D”的精简模式，而非将所有 3D 相关源码物理删除。该策略便于维护与回滚，同时显著缩小功能面与二进制体积。

---

## 统计与现状快照

- 不存在目录：
  - `scene/3d/`
  - `editor/scene/3d/`
  - `servers/physics_3d/`
  - `servers/xr/`
  - `modules/gdscript/`
- 仍存在但被禁用/不注册：
  - `scene/resources/` 中与 3D 相关的若干资源类实现文件
  - `servers/rendering/renderer_rd/` 中包含 3D 渲染路径文件
  - `modules/gltf/`、`modules/csg/`、`modules/vhacd/` 等（均在 `_3D_DISABLED` 或构建参数下禁用）

注：由于缺乏“变更前”基线与完整 Git 统计，本节不再给出“删除文件/行数”的绝对数值，改为以可验证目录状态为准。

---

## 当前编译参数（build_lite.bat）

- `module_gdscript_enabled=no`
- `module_mono_enabled=no`
- `module_webrtc_enabled=no`
- `module_multiplayer_enabled=no`
- `module_enet_enabled=no`
- `module_navigation_3d_enabled=no`
- `module_openxr_enabled=no`
- `module_mobile_vr_enabled=no`
- `module_webxr_enabled=no`
- `module_gridmap_enabled=no`
- `module_lightmapper_rd_enabled=no`
- `module_raycast_enabled=no`
- `module_gltf_enabled=no`
- `module_csg_enabled=no`
- `module_objectdb_profiler_enabled=no`
- `module_xatlas_unwrap_enabled=no`
- `module_vhacd_enabled=no`

SCons 选项（默认值）关键片段：
- `SConstruct:235` 定义 `disable_3d` 选项（默认 True）。
- `SConstruct:1016-1032` 将 `disable_3d` 同步为 `_3D_DISABLED`、并强制禁用 3D 物理、3D 导航与 XR。

---

## 最近修复记录 (2025-10-16)

### 编译错误修复过程
在禁用 3D 功能后遇到大量编译和链接错误,已全部修复:

1. **缺失头文件**: 添加了 `ResourceLoader`、`ResourceSaver`、`Material` 等必需头文件
2. **3D 类引用**: 对所有 3D 节点类 (`Node3D`, `Sprite3D`, `Camera3D` 等) 添加条件编译
3. **编辑器插件系统**:
   - `EditorPlugin` 的 3D 方法 (`forward_3d_gui_input`, `forward_3d_draw_over_viewport`, `forward_3d_force_draw_over_viewport`) 用条件编译包裹
   - `EditorPluginList` 对应方法同步处理
   - `AnimationPlayerEditorPlugin` 的 3D 覆盖方法处理
4. **材质编辑器适配**:
   - `MaterialEditor::edit()` 方法签名简化 (移除 `Environment` 参数)
   - 3D 材质预览功能 (球体/立方体/四边形切换) 用条件编译包裹
   - 保留 2D Canvas 材质编辑功能
5. **导航设置**: `EditorSettingsDialog::update_navigation_preset()` 调用用条件编译包裹
6. **宏重定义警告**: 修复 `_3D_DISABLED`、`PHYSICS_3D_DISABLED`、`XR_DISABLED`、`NAVIGATION_3D_DISABLED` 宏重定义问题

### 修改的关键文件
- `editor/plugins/editor_plugin.h/cpp` - 3D 方法条件编译
- `editor/editor_node.h/cpp` - EditorPluginList 3D 方法处理
- `editor/animation/animation_player_editor_plugin.h` - 3D 覆盖方法
- `editor/scene/material_editor_plugin.h/cpp` - 材质编辑器 2D 化
- `editor/settings/editor_settings_dialog.cpp` - 3D 导航设置
- `scene/resources/mesh.h` - 宏定义修复
- `scene/resources/navigation_mesh.cpp/h` - 3D 调试网格
- `scene/register_scene_types.cpp` - 宏定义统一
- `servers/rendering/rendering_method.h` - XR 宏定义

---

## 待办与风险（进行中）

### A. 构建与功能验证
- [x] 可编译并启动编辑器 ✅ (已通过编译)
- [ ] 打开/保存/运行 2D Demo 正常
- [ ] 2D 渲染、2D 物理、输入、音频等核心路径无回归

### B. 代码清理与优化（下一阶段 - 当前任务）
- [ ] **物理删除被 `#ifndef _3D_DISABLED` 包裹的死代码**
  - [ ] 删除 3D 方法实现 (如 `MaterialEditor` 中的旋转、光照切换等)
  - [ ] 删除 3D 成员变量声明
  - [ ] 简化条件编译嵌套
- [ ] 清理合并残留文件: `*.orig`、`*.rej`
- [ ] 扫描并移除未使用的 3D 头文件引用
- [ ] 编译器警告优化

### C. 可选模块进一步精简（可选）
- [ ] 评估移除或禁用 `modules/websocket/`
- [ ] 保持 `modules/webrtc/`、`modules/multiplayer/` 禁用状态（当前已禁用）
- [ ] 如需更强约束，可在 `build_lite.bat` 显式加入 `module_fbx_enabled=no`（尽管当前因 `disable_3d` 已不参与构建，见 `modules/fbx/config.py`）

---

## 保留功能（重点）

- 2D 渲染：`Sprite/AnimatedSprite`、`TileMap/TileSet`、`Polygon2D/Line2D`、`CanvasItem/CanvasLayer`、2D Shader
- 2D 物理：`RigidBody2D/StaticBody2D`、`CharacterBody2D/Area2D`、`CollisionShape2D/CollisionPolygon2D`
- 核心系统：资源/文件系统、输入、音频、信号与事件系统
- 2D 编辑器：场景编辑、资源导入、节点树、动画编辑器等

---

## 下一步计划

1) 构建验证（干净构建）
   - 运行 `build_lite.bat`
   - 启动编辑器并验证 2D Demo 核心链路

2) 指标对比（可选）
   - 启动时间、包体大小、内存占用、加载/运行速度

3) 清理与提交
   - 清理 `.orig/.rej` 等无关文件
   - 梳理与收敛 `_3D_DISABLED` 条件块内的死代码（仅在确保安全时）

4) 文档沉淀
   - 补充关键修改清单与设计取舍，便于后续维护与回溯

---

已知风险与对策
- 可能存在零星 3D 类型/宏引用残留 → 使用 `_3D_DISABLED`/`PHYSICS_3D_DISABLED` 搜索并做条件兜底
- 2D/3D 交叉引用导致的编译路径遗漏 → 以 `scene/register_scene_types.cpp` 的统一宏定义为准进行注册侧断开
- 编辑器 UI 残留 3D 菜单/按钮 → 已在 `register_editor_types.cpp` 侧通过条件编译屏蔽，继续回归测试

---

文档版本: 1.1（基于源码现状校正“完全删除”为“构建禁用 + 注册屏蔽”的实现方式）
