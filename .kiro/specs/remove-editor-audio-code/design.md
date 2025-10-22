# Design Document

## Overview

本设计文档描述了如何从 Godot 引擎的 editor 模块中物理删除所有被注释的音频相关代码。通过分析代码库，我们发现音频代码主要通过以下方式被禁用：

1. 使用 `#if 0` 宏包裹整个代码块
2. 使用 `// TinaFlowStudio` 注释标记被禁用的代码
3. 在 SCsub 构建文件中注释掉音频模块的编译

删除这些代码将减少代码库的复杂度，使维护更加简单，并确保不会意外重新启用音频功能。

## Architecture

### 代码分类

根据研究，editor 模块中的音频代码可以分为以下几类：

1. **音频资源选择器 (Audio Resource Picker)**
   - 文件：`editor/inspector/editor_resource_picker.h`
   - 文件：`editor/inspector/editor_resource_picker.cpp`
   - 类：`EditorAudioStreamPicker`

2. **音频预览插件 (Audio Preview Plugin)**
   - 文件：`editor/inspector/editor_preview_plugins.h`
   - 文件：`editor/inspector/editor_preview_plugins.cpp`
   - 类：`EditorAudioStreamPreviewPlugin`

3. **音频属性编辑器 (Audio Property Editor)**
   - 文件：`editor/inspector/editor_properties.cpp`
   - 相关代码：AudioStream 类型的属性编辑器创建逻辑

4. **动画轨道音频编辑器 (Animation Track Audio Editor)**
   - 文件：`editor/animation/animation_track_editor_plugins.h`
   - 文件：`editor/animation/animation_track_editor_plugins.cpp`
   - 类：`AnimationTrackEditAudio`、`AnimationTrackEditTypeAudio`

5. **音频主题样式 (Audio Theme Styles)**
   - 文件：`editor/themes/editor_theme_manager.cpp`
   - 相关代码：EditorAudioBus 样式定义

6. **音频编辑器主节点引用 (Editor Node Audio References)**
   - 文件：`editor/editor_node.h`
   - 文件：`editor/editor_node.cpp`
   - 相关代码：音频预览生成器、音频导入设置对话框、音频总线编辑器

7. **构建系统 (Build System)**
   - 文件：`editor/SCsub`
   - 相关代码：audio 子目录的编译配置

### 删除策略

采用自底向上的删除策略：

1. **第一阶段**：删除独立的类定义和实现
   - 删除 `#if 0` 包裹的完整类定义
   - 删除相关的实现代码

2. **第二阶段**：删除类引用和使用
   - 删除头文件包含语句
   - 删除对象创建和注册代码
   - 删除成员变量声明

3. **第三阶段**：删除主题和样式代码
   - 删除音频总线相关的样式设置

4. **第四阶段**：清理注释和空行
   - 删除 TinaFlowStudio 标记注释
   - 整理代码格式

## Components and Interfaces

### 受影响的文件列表

#### 头文件 (.h)
1. `editor/inspector/editor_resource_picker.h`
   - 删除：`EditorAudioStreamPicker` 类定义（第 197-220 行）
   
2. `editor/inspector/editor_preview_plugins.h`
   - 删除：`EditorAudioStreamPreviewPlugin` 类定义（第 113-122 行）

3. `editor/animation/animation_track_editor_plugins.h`
   - 删除：`AnimationTrackEditAudio` 类定义（第 58-107 行）
   - 删除：`AnimationTrackEditTypeAudio` 类定义（第 109-167 行）
   - 删除：`create_audio_track_edit()` 方法声明（第 169-171 行）

4. `editor/editor_node.h`
   - 删除：音频相关的前向声明和注释（第 65-69 行）
   - 删除：音频相关的成员变量声明（第 419-481 行）

#### 实现文件 (.cpp)
1. `editor/inspector/editor_resource_picker.cpp`
   - 删除：头文件包含注释（第 33-35 行）
   - 删除：`EditorAudioStreamPicker` 完整实现（第 1404-1581 行）

2. `editor/inspector/editor_preview_plugins.cpp`
   - 删除：`EditorAudioStreamPreviewPlugin` 完整实现（第 637-700+ 行）

3. `editor/inspector/editor_properties.cpp`
   - 删除：AudioStream 属性编辑器创建代码（第 3461-3470 行）

4. `editor/animation/animation_track_editor_plugins.cpp`
   - 删除：头文件包含注释（第 34-36 行）
   - 删除：`AnimationTrackEditAudio` 实现（第 189-364 行）
   - 删除：`AnimationTrackEditTypeAudio` 实现（第 795-1171 行）
   - 删除：`create_audio_track_edit()` 实现（第 1380-1385 行）
   - 删除：音频轨道创建逻辑（第 1337-1345 行）

5. `editor/themes/editor_theme_manager.cpp`
   - 删除：EditorAudioBus 样式设置（第 2002-2007 行）
   - 删除：相关注释（第 2002 行）

6. `editor/editor_node.cpp`
   - 删除：头文件包含注释（第 76-79 行、第 113-115 行）
   - 删除：音频导入设置对话框相关代码（第 7967-7970 行）
   - 删除：音频预览生成器相关代码（第 8458-8461 行）
   - 删除：音频总线编辑器相关代码（第 8511-8536 行）
   - 删除：音频预览插件注册代码（第 8556-8558 行）

#### 构建文件
1. `editor/SCsub`
   - 删除：audio 子目录编译注释（第 96-98 行）

### 代码块识别模式

删除代码时需要识别以下模式：

1. **完整的 #if 0 块**
```cpp
// TinaFlowStudio - [描述]
#if 0
[代码内容]
#endif // TinaFlowStudio
```

2. **嵌套的 #if 0 块**
```cpp
#if 0
    // 外层代码
    #if 0
    // 内层代码
    #endif
#endif
```

3. **单行注释**
```cpp
// TinaFlowStudio - [描述]
// [被注释的代码]
```

4. **注释的头文件包含**
```cpp
// TinaFlowStudio - [描述]
// #include "path/to/header.h"
```

## Data Models

不涉及数据模型变更，仅删除代码。

## Error Handling

### 潜在问题和解决方案

1. **编译错误**
   - 问题：删除代码后可能导致未定义的符号引用
   - 解决：在删除前使用 grep 搜索所有引用，确保没有遗漏

2. **链接错误**
   - 问题：其他模块可能依赖被删除的类
   - 解决：由于这些代码已经被 `#if 0` 禁用，理论上不应该有外部依赖

3. **格式问题**
   - 问题：删除代码后可能留下多余的空行
   - 解决：删除代码块时，同时删除前后的空行，保持代码整洁

4. **宏匹配问题**
   - 问题：嵌套的 `#if 0` 可能导致匹配错误
   - 解决：手动检查每个 `#if 0` 块，确保正确匹配 `#endif`

## Testing Strategy

### 验证步骤

1. **语法验证**
   - 使用编译器检查语法错误
   - 确保所有头文件可以正常解析

2. **编译验证**
   - 编译 editor 模块
   - 确保没有未定义符号错误
   - 确保没有链接错误

3. **功能验证**
   - 启动编辑器
   - 验证非音频功能正常工作
   - 确认音频相关功能已完全移除

4. **代码审查**
   - 检查是否还有残留的音频相关注释
   - 确认所有 TinaFlowStudio 标记的代码都已删除
   - 验证代码格式整洁

### 回滚计划

如果删除过程中出现问题：
1. 使用 Git 回滚到删除前的状态
2. 分析错误原因
3. 调整删除策略
4. 重新执行删除操作

## Implementation Notes

### 删除顺序

按照以下顺序删除代码，以最小化依赖问题：

1. 删除 `editor/inspector/editor_resource_picker.cpp` 中的实现
2. 删除 `editor/inspector/editor_resource_picker.h` 中的类定义
3. 删除 `editor/inspector/editor_preview_plugins.cpp` 中的实现
4. 删除 `editor/inspector/editor_preview_plugins.h` 中的类定义
5. 删除 `editor/animation/animation_track_editor_plugins.cpp` 中的实现
6. 删除 `editor/animation/animation_track_editor_plugins.h` 中的类定义
7. 删除 `editor/inspector/editor_properties.cpp` 中的引用
8. 删除 `editor/editor_node.cpp` 中的引用
9. 删除 `editor/editor_node.h` 中的声明
10. 删除 `editor/themes/editor_theme_manager.cpp` 中的样式
11. 删除 `editor/SCsub` 中的注释

### 代码行数统计

预计删除的代码行数：
- 头文件：约 150 行
- 实现文件：约 800 行
- 注释和空行：约 50 行
- 总计：约 1000 行

### 风险评估

- **低风险**：这些代码已经被 `#if 0` 禁用，不参与编译
- **中风险**：可能存在未被发现的引用
- **缓解措施**：在删除前进行全面的代码搜索，确保没有遗漏的引用
