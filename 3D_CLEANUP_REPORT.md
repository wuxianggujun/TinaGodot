# 3D残留代码清理报告

## 已完成清理 (2025-10-17)

### 高优先级清理 ✅
1. **第三方库** (4个, ~200MB)
   - ✅ thirdparty/jolt_physics/
   - ✅ thirdparty/openxr/
   - ✅ thirdparty/vhacd/
   - ✅ thirdparty/embree/

2. **模块** (2个, ~50MB)
   - ✅ modules/fbx/
   - ✅ modules/vhacd/

3. **测试文件** (15个)
   - ✅ 所有 test_*_3d.h

4. **文档** (141个XML)
   - ✅ doc/classes/*3D*.xml

5. **图标** (118个SVG)
   - ✅ editor/icons/*3D*.svg

### 总计已清理
- **删除目录**: 6个
- **删除文件**: 274个
- **减少体积**: ~257MB

---

## 剩余3D宏包裹代码 (低优先级)

### 仍包含宏的文件统计
- `_3D_DISABLED`: 35个文件
- `PHYSICS_3D_DISABLED`: 15个文件
- `NAVIGATION_3D_DISABLED`: 8个文件
- `XR_DISABLED`: 15个文件

### 主要文件列表

#### Core (1个)
- core/config/project_settings.cpp

#### Editor (14个)
- editor/animation/animation_player_editor_plugin.cpp
- editor/animation/animation_track_editor.h
- editor/debugger/script_editor_debugger.cpp
- editor/docks/filesystem_dock.cpp
- editor/editor_interface.cpp
- editor/editor_node.cpp
- editor/plugins/editor_plugin.h
- editor/project_manager/project_manager.cpp
- editor/scene/scene_create_dialog.cpp
- editor/scene/scene_tree_editor.cpp
- editor/scene/sprite_frames_editor_plugin.cpp
- editor/settings/editor_settings_dialog.cpp
- editor/shader/visual_shader_editor_plugin.cpp/h

#### Main (3个)
- main/main.cpp
- main/performance.cpp/h

#### Scene (10个)
- scene/2d/tile_map_layer.cpp
- scene/animation/root_motion_view.cpp
- scene/main/scene_tree.cpp/h
- scene/main/scene_tree_fti.cpp/h
- scene/main/scene_tree_fti_tests.cpp
- scene/main/viewport.cpp
- scene/register_scene_types.cpp
- scene/resources/packed_scene.cpp
- scene/resources/physics_material.cpp/h

#### Servers (3个)
- servers/register_server_types.cpp
- servers/rendering/renderer_scene_cull.cpp
- servers/rendering/renderer_viewport.cpp

#### Tests (2个)
- tests/scene/test_text_edit.h
- tests/test_main.cpp

---

## 建议

### 选项1: 保持现状 (推荐)
- 这些宏包裹的代码已经被禁用，不会编译
- 保留宏便于未来可能的回滚或调试
- 对运行时性能无影响

### 选项2: 继续删除宏包裹代码
- 需要逐个文件检查和删除
- 风险: 可能误删2D相关代码
- 收益: 代码更简洁，但工作量大

### 选项3: 删除整个被宏包裹的代码块
- 使用脚本批量删除 `#ifndef _3D_DISABLED` 到 `#endif` 之间的代码
- 风险较高，需要仔细验证

---

## 结论

当前已完成高优先级和中优先级的清理工作，项目体积减少约257MB。
剩余的宏包裹代码对运行时无影响，建议保持现状。
