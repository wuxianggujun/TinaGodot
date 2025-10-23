# FlowGraph 节点库 Dock 修正

## 问题

之前的实现将节点库作为独立的 Dock 面板添加，导致它和"场景"、"导入"分开显示。

## 解决方案

将 `FlowNodeLibraryDock` 注册到 `EditorDockManager`，这样它会自动作为标签页添加到左侧 Dock 区域。

## 修改内容

### 1. FlowNodeLibraryDock 改为单例

**文件**: `editor/flow_studio/flow_node_library_dock.h/cpp`

```cpp
// 添加单例支持
private:
    static FlowNodeLibraryDock *singleton;
    
public:
    static FlowNodeLibraryDock *get_singleton() { return singleton; }
```

### 2. 在 EditorNode 中注册

**文件**: `editor/editor_node.cpp`

```cpp
// 在 SceneTreeDock 和 ImportDock 之后添加
memnew(FlowNodeLibraryDock);
editor_dock_manager->add_dock(FlowNodeLibraryDock::get_singleton());
```

### 3. 移除 CanvasItemEditorPlugin 中的注册

**文件**: `editor/scene/canvas_item_editor_plugin.cpp`

移除了在插件中添加 Dock 的代码，改为在 EditorNode 中统一管理。

## 效果

现在左侧 Dock 区域会有三个标签页：
- **场景** (Scene)
- **导入** (Import)
- **FlowNodes** (节点库) ← 新增

用户可以点击标签切换，就像切换"场景"和"导入"一样。

## 界面预览

```
┌─────────────────────────────────────────┐
│ [场景] [导入] [FlowNodes]               │ ← 标签页
├─────────────────────────────────────────┤
│ FlowNodes 面板内容                      │
│                                         │
│ Event                                   │
│  └─ Start                               │
│                                         │
│ Input/Output                            │
│  └─ Print                               │
│                                         │
│ Data                                    │
│  ├─ Constant                            │
│  └─ Variable                            │
│                                         │
│ Math                                    │
│  ├─ Add                                 │
│  ├─ Subtract                            │
│  ├─ Multiply                            │
│  └─ Divide                              │
│                                         │
│ Control Flow                            │
│  ├─ If/Else                             │
│  └─ Loop                                │
└─────────────────────────────────────────┘
```

## 技术细节

### EditorDockManager

Godot 的 `EditorDockManager` 负责管理所有 Dock 面板：
- 自动将 Dock 组织成标签页
- 处理 Dock 的拖拽和重新排列
- 保存和恢复 Dock 布局

### Dock 注册顺序

```cpp
// editor/editor_node.cpp 中的注册顺序
1. SceneTreeDock    // 场景树
2. ImportDock       // 导入
3. FlowNodeLibraryDock  // 节点库 (新增)
4. FileSystemDock   // 文件系统
5. InspectorDock    // 检查器
6. NodeDock         // 节点
7. HistoryDock      // 历史
```

默认情况下，前几个 Dock 会在左上角区域显示为标签页。

## 编译和测试

```bash
# 编译
scons platform=windows target=editor

# 测试
1. 启动编辑器
2. 点击 "FlowGraph" 标签
3. 查看左侧 Dock 区域
4. 应该看到 [场景] [导入] [FlowNodes] 三个标签
5. 点击 [FlowNodes] 标签
6. 应该看到节点库内容
```

## 下一步

1. 实现节点拖拽到画布
2. 在画布上创建 FlowGraphNode
3. 实现节点连接功能

---

**状态**: ✅ 修正完成，等待编译测试
