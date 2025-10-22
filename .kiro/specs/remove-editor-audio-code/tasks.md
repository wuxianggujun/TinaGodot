# Implementation Plan

- [x] 1. 删除音频资源选择器代码


  - 删除 `editor/inspector/editor_resource_picker.cpp` 中 `EditorAudioStreamPicker` 类的完整实现（第 1404-1581 行）
  - 删除 `editor/inspector/editor_resource_picker.h` 中 `EditorAudioStreamPicker` 类定义（第 197-220 行）
  - 删除相关的 TinaFlowStudio 注释
  - _Requirements: 1.1, 1.3, 2.1, 3.1_


- [x] 2. 删除音频预览插件代码

  - 删除 `editor/inspector/editor_preview_plugins.cpp` 中 `EditorAudioStreamPreviewPlugin` 类的完整实现（从第 637 行开始的 #if 0 块）
  - 删除 `editor/inspector/editor_preview_plugins.h` 中 `EditorAudioStreamPreviewPlugin` 类定义（第 113-122 行）
  - 删除相关的 TinaFlowStudio 注释
  - _Requirements: 1.1, 1.3, 2.1, 3.1_

- [x] 3. 删除动画轨道音频编辑器代码




  - 删除 `editor/animation/animation_track_editor_plugins.cpp` 中 `AnimationTrackEditAudio` 类实现（第 189-364 行）
  - 删除 `editor/animation/animation_track_editor_plugins.cpp` 中 `AnimationTrackEditTypeAudio` 类实现（第 795-1171 行）
  - 删除 `editor/animation/animation_track_editor_plugins.cpp` 中 `create_audio_track_edit()` 方法实现（第 1380-1385 行）
  - 删除 `editor/animation/animation_track_editor_plugins.cpp` 中音频轨道创建逻辑（第 1337-1345 行）
  - 删除 `editor/animation/animation_track_editor_plugins.h` 中 `AnimationTrackEditAudio` 类定义（第 58-107 行）
  - 删除 `editor/animation/animation_track_editor_plugins.h` 中 `AnimationTrackEditTypeAudio` 类定义（第 109-167 行）
  - 删除 `editor/animation/animation_track_editor_plugins.h` 中 `create_audio_track_edit()` 方法声明（第 169-171 行）
  - 删除相关的头文件包含注释和 TinaFlowStudio 标记
  - _Requirements: 1.1, 1.3, 2.1, 3.1_


- [x] 4. 删除音频属性编辑器代码

  - 删除 `editor/inspector/editor_properties.cpp` 中 AudioStream 类型的属性编辑器创建代码（第 3461-3470 行）
  - 删除相关的 TinaFlowStudio 注释
  - _Requirements: 1.1, 1.3, 2.1_


- [x] 5. 删除编辑器主节点中的音频引用

  - 删除 `editor/editor_node.h` 中音频相关的前向声明和注释（第 65-69 行）
  - 删除 `editor/editor_node.h` 中音频相关的成员变量声明（第 419-481 行）
  - 删除 `editor/editor_node.cpp` 中音频相关的头文件包含注释（第 76-79 行、第 113-115 行）
  - 删除 `editor/editor_node.cpp` 中音频导入设置对话框相关代码（第 7967-7970 行）
  - 删除 `editor/editor_node.cpp` 中音频预览生成器相关代码（第 8458-8461 行）
  - 删除 `editor/editor_node.cpp` 中音频总线编辑器相关代码（第 8511-8536 行）
  - 删除 `editor/editor_node.cpp` 中音频预览插件注册代码（第 8556-8558 行）
  - _Requirements: 1.1, 1.3, 2.1, 3.3_


- [x] 6. 删除音频主题样式代码

  - 删除 `editor/themes/editor_theme_manager.cpp` 中 EditorAudioBus 样式设置（第 2002-2007 行）
  - 删除相关的注释
  - _Requirements: 1.3, 2.1, 4.1, 4.2_

- [x] 7. 删除构建系统中的音频模块注释


  - 删除 `editor/SCsub` 中 audio 子目录编译的注释行（第 96-98 行）
  - _Requirements: 1.3, 2.1_


- [x] 8. 验证代码完整性和编译正确性


  - 使用 grep 搜索确认没有残留的 TinaFlowStudio 音频相关注释
  - 使用 grep 搜索确认没有残留的 `#if 0` 音频代码块
  - 检查代码格式，确保没有多余的空行
  - 编译 editor 模块，确保没有编译错误
  - 确保没有未定义符号或链接错误
  - _Requirements: 6.1, 6.2, 6.3_
