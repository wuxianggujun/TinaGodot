# FlowNodeLibraryDock 编译错误修复

## 问题描述

编译时出现类型转换错误：
```
error C2664: 'void EditorDockManager::add_dock(EditorDock *)': 
无法将参数 1 从 'FlowNodeLibraryDock *' 转换为 'EditorDock *'
```

## 根本原因

`FlowNodeLibraryDock` 继承自 `VBoxContainer`，但 `EditorDockManager::add_dock()` 方法需要 `EditorDock*` 类型的参数。

在 Godot 编辑器中，所有的 dock 面板都必须继承自 `EditorDock` 基类，而不是直接继承自 GUI 容器类。

## 解决方案

### 1. 修改头文件 (flow_node_library_dock.h)

**修改前：**
```cpp
#include "scene/gui/box_container.h"
#include "scene/gui/tree.h"

class FlowNodeLibraryDock : public VBoxContainer {
	GDCLASS(FlowNodeLibraryDock, VBoxContainer);
```

**修改后：**
```cpp
#include "editor/docks/editor_dock.h"
#include "scene/gui/box_container.h"
#include "scene/gui/tree.h"

class FlowNodeLibraryDock : public EditorDock {
	GDCLASS(FlowNodeLibraryDock, EditorDock);
```

### 2. 修改实现文件 (flow_node_library_dock.cpp)

**修改前：**
```cpp
FlowNodeLibraryDock::FlowNodeLibraryDock() {
	singleton = this;
	set_name("FlowNodes");
	
	// Title
	Label *title = memnew(Label);
	title->set_text(TTR("Node Library"));
	add_child(title);
	
	// Node tree
	node_tree = memnew(Tree);
	// ...
	add_child(node_tree);
	
	_populate_nodes();
}
```

**修改后：**
```cpp
FlowNodeLibraryDock::FlowNodeLibraryDock() {
	singleton = this;
	set_name(TTR("FlowNodes"));
	set_title(TTR("Node Library"));
	set_icon_name("NodeWarning"); // 使用临时图标
	set_default_slot(EditorDockManager::DOCK_SLOT_LEFT_UR); // 与 Scene 和 Import 在同一位置
	
	// Create a VBoxContainer to hold the content
	VBoxContainer *vbox = memnew(VBoxContainer);
	vbox->set_v_size_flags(SIZE_EXPAND_FILL);
	add_child(vbox);
	
	// Node tree
	node_tree = memnew(Tree);
	// ...
	vbox->add_child(node_tree);
	
	_populate_nodes();
}
```

## 关键改进

1. **继承正确的基类**：从 `VBoxContainer` 改为 `EditorDock`
2. **使用 EditorDock API**：
   - `set_title()` - 设置 dock 标题
   - `set_icon_name()` - 设置 dock 图标
   - `set_default_slot()` - 设置默认停靠位置
3. **正确的布局结构**：
   - EditorDock 继承自 MarginContainer
   - 在其中添加 VBoxContainer 来组织内容
   - 所有子控件添加到 VBoxContainer 中

## Dock 位置说明

使用 `EditorDockManager::DOCK_SLOT_LEFT_UR` 将 FlowNodeLibraryDock 放置在：
- 左侧面板
- 上半部分
- 右侧区域

这样它会作为一个新的标签页出现在 Scene 和 Import 面板旁边。

## 其他可用的 Dock 位置

```cpp
DOCK_SLOT_LEFT_UL,  // 左上-左侧
DOCK_SLOT_LEFT_BL,  // 左下-左侧
DOCK_SLOT_LEFT_UR,  // 左上-右侧 (Scene, Import 的位置)
DOCK_SLOT_LEFT_BR,  // 左下-右侧 (FileSystem 的位置)
DOCK_SLOT_RIGHT_UL, // 右上-左侧 (Inspector, Node 的位置)
DOCK_SLOT_RIGHT_BL, // 右下-左侧
DOCK_SLOT_RIGHT_UR, // 右上-右侧
DOCK_SLOT_RIGHT_BR, // 右下-右侧
```

## 验证

编译后，FlowNodeLibraryDock 应该：
1. 成功编译，没有类型转换错误
2. 在编辑器中显示为 "Node Library" 标签页
3. 与 Scene 和 Import 面板在同一个 TabContainer 中
4. 显示节点库的树形列表

## 参考

- `editor/docks/editor_dock.h` - EditorDock 基类定义
- `editor/docks/import_dock.cpp` - ImportDock 实现参考
- `editor/docks/scene_tree_dock.cpp` - SceneTreeDock 实现参考
