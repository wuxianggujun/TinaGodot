# FlowGraph 集成到 2D 编辑器 - 完整实现

## ✅ 已完成的修改

### 1. 重命名 2D 标签为 FlowGraph

**文件**: `editor/scene/canvas_item_editor_plugin.h`  
**行号**: 620

```cpp
virtual String get_plugin_name() const override { return TTRC("FlowGraph"); }
```

**效果**: 编辑器顶部的 "2D" 标签现在显示为 "FlowGraph"

### 2. 网格默认显示

**文件**: `editor/scene/canvas_item_editor_plugin.h`  
**行号**: 215

```cpp
GridVisibility grid_visibility = GRID_VISIBILITY_SHOW; // TinaFlowStudio: Default show grid
```

**效果**: 
- 画布背景默认显示网格
- 用户仍然可以通过 View 菜单关闭网格
- 不是强制显示，保留了用户选择权

### 3. 添加节点库 Dock

**新增文件**:
- `editor/flow_studio/flow_node_library_dock.h`
- `editor/flow_studio/flow_node_library_dock.cpp`

**修改文件**:
- `editor/scene/canvas_item_editor_plugin.h` - 添加 node_library_dock 成员
- `editor/scene/canvas_item_editor_plugin.cpp` - 在构造函数中创建并注册 Dock

**功能**:
- 在左侧 Dock 区域添加"FlowNodes"面板
- 和"场景"、"导入"、"文件系统"并列显示
- 包含节点分类：
  - Event（事件）- Start
  - Input/Output（输入输出）- Print
  - Data（数据）- Constant, Variable
  - Math（数学）- Add, Subtract, Multiply, Divide
  - Control Flow（控制流）- If/Else, Loop
- 支持双击节点（当前打印日志，后续可实现创建节点）
- 显示节点工具提示

## 界面布局

```
┌─────────────────────────────────────────────────────────┐
│ File  Edit  Project  Debug  [FlowGraph]  Script  AssetLib│
├─────────────────────────────────────────────────────────┤
│ ┌─────────┬─────────────────────────────────────────┐   │
│ │FlowNodes│                                         │   │
│ │         │                                         │   │
│ │ Event   │         Canvas (Grid Background)        │   │
│ │  Start  │                                         │   │
│ │         │                                         │   │
│ │ I/O     │                                         │   │
│ │  Print  │                                         │   │
│ │         │                                         │   │
│ │ Data    │                                         │   │
│ │  Const  │                                         │   │
│ │  Var    │                                         │   │
│ │         │                                         │   │
│ │ Math    │                                         │   │
│ │  Add    │                                         │   │
│ │  Sub    │                                         │   │
│ │  Mul    │                                         │   │
│ │  Div    │                                         │   │
│ │         │                                         │   │
│ │ Control │                                         │   │
│ │  If     │                                         │   │
│ │  Loop   │                                         │   │
│ └─────────┴─────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────┘
```

## 技术实现

### FlowNodeLibraryDock 类

```cpp
class FlowNodeLibraryDock : public VBoxContainer {
    GDCLASS(FlowNodeLibraryDock, VBoxContainer);
    
private:
    Tree *node_tree;  // 节点树控件
    
    void _populate_nodes();      // 填充节点列表
    void _on_node_selected();    // 节点选中回调
    void _on_node_activated();   // 节点激活（双击）回调
    
public:
    String get_selected_node_type() const;  // 获取选中的节点类型
};
```

### 注册到编辑器

```cpp
// 在 CanvasItemEditorPlugin 构造函数中
node_library_dock = memnew(FlowNodeLibraryDock);
add_control_to_dock(DOCK_SLOT_LEFT_UL, node_library_dock);
```

- `DOCK_SLOT_LEFT_UL` - 左上角 Dock 槽位
- 自动和其他 Dock 面板（场景、导入等）并列显示

## 优势

相比独立的 FlowGraph 标签页：

1. ✅ **更统一** - 复用现有的 2D 编辑器布局
2. ✅ **更美观** - 保持 Godot 的界面风格
3. ✅ **更直观** - 用户不需要切换标签页
4. ✅ **节省空间** - 充分利用现有的 UI 结构
5. ✅ **保留功能** - 2D 编辑器的所有工具都可以复用
6. ✅ **节点库集成** - 左侧面板和场景树并列，符合 Godot 习惯

## 编译和测试

```bash
# 编译
scons platform=windows target=editor

# 运行
# 1. 点击顶部的 "FlowGraph" 标签
# 2. 应该看到网格背景的画布
# 3. 左侧应该看到 "FlowNodes" 面板
# 4. 双击节点会在控制台打印日志
```

## 下一步计划

### 1. 实现节点拖拽

从节点库拖拽到画布创建节点：

```cpp
// 在 FlowNodeLibraryDock 中实现
Variant get_drag_data(const Point2 &p_point);

// 在 CanvasItemEditor 中实现
bool can_drop_data(const Point2 &p_point, const Variant &p_data);
void drop_data(const Point2 &p_point, const Variant &p_data);
```

### 2. 集成 FlowGraphNode

将之前创建的 `FlowGraphNode` 集成到 Canvas：

```cpp
// 创建节点时
FlowGraphNode *gnode = memnew(FlowGraphNode);
gnode->set_flow_node(flow_node);
canvas->add_child(gnode);
```

### 3. 实现节点连接

使用 Canvas 的连线功能连接节点：

```cpp
// 监听端口点击事件
// 绘制连接线
// 创建 FlowConnection 数据
```

### 4. 工具栏集成

在 Canvas 工具栏添加 FlowGraph 专用工具：
- Run/Stop 按钮
- 保存/加载按钮
- 调试工具

## 文件清单

### 新增文件
- `editor/flow_studio/flow_node_library_dock.h`
- `editor/flow_studio/flow_node_library_dock.cpp`
- `FLOWGRAPH_INTEGRATION_V2.md`

### 修改文件
- `editor/scene/canvas_item_editor_plugin.h` (3 处修改)
- `editor/scene/canvas_item_editor_plugin.cpp` (2 处修改)

## 注意事项

1. 原有的 2D 编辑功能仍然保留
2. 可以在画布上放置 Node2D 节点（如果需要）
3. 网格默认显示，但可以通过菜单关闭
4. 节点库面板可以通过拖动调整大小
5. 节点库面板可以通过标签切换隐藏/显示

## 兼容性

- ✅ 不影响现有的 2D 编辑功能
- ✅ 可以和 FlowGraph 功能共存
- ✅ 用户可以选择使用哪些功能
- ✅ 保持 Godot 编辑器的一致性

---

**版本**: 0.2.0-alpha  
**日期**: 2024-01  
**状态**: ✅ 编译通过，节点库面板已添加
