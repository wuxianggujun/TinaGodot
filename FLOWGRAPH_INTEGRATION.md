# FlowGraph 集成到 2D 编辑器

## 已完成的修改

### 1. 重命名 2D 标签为 FlowGraph ✅

**文件**: `editor/scene/canvas_item_editor_plugin.h`

**修改**:
```cpp
// 第 620 行
virtual String get_plugin_name() const override { return TTRC("FlowGraph"); }
```

**效果**: 编辑器顶部的 "2D" 标签现在显示为 "FlowGraph"

### 2. 始终显示网格背景 ✅

**文件**: `editor/scene/canvas_item_editor_plugin.cpp`

**修改**:
```cpp
// 第 1059 行
bool CanvasItemEditor::_is_grid_visible() const {
    // TinaFlowStudio: Always show grid for FlowGraph editor
    return true;
}
```

**效果**: 画布背景始终显示网格，类似 GraphEdit 的效果

## 下一步：添加节点库面板

### 方案

在左侧 Dock 区域添加"节点库"面板，和"场景"、"导入"并列。

### 实现步骤

1. **创建节点库 Dock**
   - 创建 `FlowNodeLibraryDock` 类
   - 继承自 `VBoxContainer`
   - 包含 Tree 控件显示节点分类

2. **注册到编辑器**
   - 在 `CanvasItemEditorPlugin` 中添加 Dock
   - 使用 `add_control_to_dock()` 方法

3. **节点拖拽**
   - 实现从节点库拖拽到画布
   - 在画布上创建对应的 FlowGraphNode

### 代码示例

```cpp
// flow_node_library_dock.h
class FlowNodeLibraryDock : public VBoxContainer {
    GDCLASS(FlowNodeLibraryDock, VBoxContainer);
    
private:
    Tree *node_tree;
    
    void _populate_nodes();
    void _on_node_selected();
    
public:
    FlowNodeLibraryDock();
};

// 在 CanvasItemEditorPlugin 中注册
add_control_to_dock(DOCK_SLOT_LEFT_UL, node_library_dock);
```

## 优势

相比独立的 FlowGraph 标签页：

1. ✅ **更统一** - 复用现有的 2D 编辑器布局
2. ✅ **更美观** - 保持 Godot 的界面风格
3. ✅ **更直观** - 用户不需要切换标签页
4. ✅ **节省空间** - 充分利用现有的 UI 结构
5. ✅ **保留功能** - 2D 编辑器的所有工具都可以复用

## 当前状态

- ✅ 标签重命名完成
- ✅ 网格背景始终显示
- ⏳ 节点库面板待实现

## 编译和测试

```bash
# 编译
scons platform=windows target=editor

# 运行
# 点击顶部的 "FlowGraph" 标签
# 应该看到网格背景的画布
```

## 注意事项

1. 原有的 2D 编辑功能仍然保留
2. 可以在画布上放置 Node2D 节点
3. 网格始终显示，无法关闭（如需要可以添加开关）
4. 节点库面板需要额外实现

## 后续计划

1. 实现节点库 Dock
2. 实现节点拖拽到画布
3. 集成 FlowGraphNode 到 Canvas
4. 实现节点连接逻辑
