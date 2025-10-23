# FlowNodeLibraryDock 调试指南

## 问题：Dock 没有显示在编辑器中

虽然代码编译通过，但 FlowNodeLibraryDock 没有在编辑器左侧面板显示为标签页。

## 可能的原因和解决方案

### 1. 编辑器布局缓存问题

Godot 编辑器会保存布局配置。如果之前的配置中没有这个 dock，它可能被隐藏了。

**解决方案：重置编辑器布局**

1. 在 Godot 编辑器中，点击顶部菜单 **Editor** → **Editor Layout** → **Default**
2. 或者删除编辑器布局配置文件：
   - Windows: `%APPDATA%\Godot\editor_layouts\`
   - 或项目目录下的 `.godot/editor/` 文件夹

### 2. 检查调试输出

我已经在构造函数中添加了调试输出：

```cpp
print_line("FlowNodeLibraryDock: Constructor called");
print_line("FlowNodeLibraryDock: Initialized with title '", get_title(), "' and slot ", get_default_slot());
```

**检查步骤：**

1. 重新编译项目
2. 运行编辑器
3. 查看控制台输出，确认是否看到这两行调试信息
4. 如果看到了，说明 dock 被创建了，只是可能被隐藏
5. 如果没看到，说明构造函数没有被调用，需要检查注册代码

### 3. 手动启用 Dock

如果 dock 被创建但隐藏了，可以通过菜单手动启用：

1. 点击顶部菜单 **Editor** → **Docks**
2. 查找 **Node Library** 选项
3. 点击启用它

### 4. 检查 Dock 菜单

在编辑器运行时：

1. 右键点击任何 dock 标签页
2. 查看弹出菜单中是否有 "Node Library" 选项
3. 如果有，点击它来显示 dock

### 5. 验证代码注册

确认 `editor/editor_node.cpp` 中的注册代码：

```cpp
// TinaFlowStudio: Add FlowNodes dock
memnew(FlowNodeLibraryDock);
editor_dock_manager->add_dock(FlowNodeLibraryDock::get_singleton());
```

这段代码应该在 `ImportDock` 之后，`FileSystemDock` 之前。

### 6. 检查头文件包含

确认 `editor/editor_node.cpp` 包含了头文件：

```cpp
#include "editor/flow_studio/flow_node_library_dock.h"
```

### 7. 完全重新编译

有时候增量编译可能有问题：

```bash
# 清理构建
scons --clean

# 完全重新编译
scons platform=windows target=editor
```

### 8. 检查 Dock 是否在其他位置

Dock 可能被移动到了其他位置。检查：

- 右侧面板（Inspector/Node 区域）
- 底部面板
- 可能被最小化或隐藏

### 9. 临时测试：改变 Dock 位置

尝试将 dock 放到不同的位置来测试：

```cpp
// 尝试放到右侧面板
set_default_slot(EditorDockManager::DOCK_SLOT_RIGHT_UL);

// 或者底部左侧
set_default_slot(EditorDockManager::DOCK_SLOT_LEFT_BL);
```

### 10. 检查是否有错误消息

查看编辑器启动时的控制台输出，看是否有任何错误或警告消息。

## 预期结果

成功后，你应该看到：

1. 控制台输出：
   ```
   FlowNodeLibraryDock: Constructor called
   FlowNodeLibraryDock: Initialized with title 'Node Library' and slot 2
   ```

2. 编辑器左上角有三个标签页：
   - Scene
   - Import
   - **Node Library** ← 新增的

3. 点击 "Node Library" 标签页后，显示节点树：
   - Event
     - Start
   - Input/Output
     - Print
   - Data
     - Constant
     - Variable
   - Math
     - Add
     - Subtract
     - Multiply
     - Divide
   - Control Flow
     - If/Else
     - Loop

## 下一步

如果以上方法都不行，请提供：

1. 控制台的完整输出
2. 编辑器启动时的任何错误消息
3. 是否看到了调试输出
4. Editor → Docks 菜单中是否有 "Node Library" 选项

这将帮助我们进一步诊断问题。
