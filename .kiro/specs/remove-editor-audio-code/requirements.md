# Requirements Document

## Introduction

本功能旨在从 Godot 引擎的 editor 模块中物理删除所有被宏注释（如 `#if 0`、`DISABLE_AUDIO`）或直接注释的音频相关代码。这是 TinaFlowStudio 项目精简编辑器功能的一部分，目标是创建一个更轻量级的 2D 编辑器版本。

## Glossary

- **Editor Module**: Godot 引擎中的编辑器模块，位于 `editor/` 目录
- **Audio Code**: 与音频功能相关的代码，包括音频流处理、音频总线、音频预览等
- **Macro-Commented Code**: 使用预处理器宏（如 `#if 0`、`#ifndef DISABLE_AUDIO`）注释掉的代码块
- **Direct-Commented Code**: 使用 `//` 或 `/* */` 直接注释的代码
- **Physical Deletion**: 从源文件中完全移除代码，而不是仅仅注释掉

## Requirements

### Requirement 1

**User Story:** 作为开发者，我希望删除所有被宏注释的音频代码，以便减少代码库的复杂度和维护负担

#### Acceptance Criteria

1. WHEN 扫描 editor 目录时，THE System SHALL 识别所有使用 `#if 0` 包裹的音频相关代码块
2. WHEN 扫描 editor 目录时，THE System SHALL 识别所有使用 `#ifndef DISABLE_AUDIO` 或 `#ifdef DISABLE_AUDIO` 包裹的音频相关代码块
3. WHEN 识别到宏注释的音频代码块时，THE System SHALL 完全删除该代码块及其宏定义行
4. WHEN 删除代码后，THE System SHALL 确保文件语法正确且可编译

### Requirement 2

**User Story:** 作为开发者，我希望删除所有直接注释的音频相关代码，以便清理代码库

#### Acceptance Criteria

1. WHEN 扫描 editor 目录时，THE System SHALL 识别所有包含 "TinaFlowStudio - Disabled audio" 注释的代码段
2. WHEN 识别到直接注释的音频代码时，THE System SHALL 删除注释行及其后续的被注释代码
3. WHEN 删除代码后，THE System SHALL 保持文件的代码结构完整性

### Requirement 3

**User Story:** 作为开发者，我希望删除音频相关的类定义和实现，以便彻底移除音频功能

#### Acceptance Criteria

1. WHEN 识别到音频相关的类（如 `EditorAudioStreamPicker`、`EditorAudioStreamPreviewPlugin`）时，THE System SHALL 删除整个类定义
2. WHEN 识别到音频相关的类实现文件时，THE System SHALL 删除整个实现代码
3. WHEN 删除类后，THE System SHALL 更新相关的头文件包含和前向声明

### Requirement 4

**User Story:** 作为开发者，我希望删除音频相关的主题和样式代码，以便减少不必要的 UI 资源

#### Acceptance Criteria

1. WHEN 扫描主题管理器代码时，THE System SHALL 识别所有音频总线（EditorAudioBus）相关的样式设置
2. WHEN 识别到音频相关的主题代码时，THE System SHALL 删除这些样式定义
3. WHEN 删除后，THE System SHALL 确保主题系统仍然正常工作

### Requirement 5

**User Story:** 作为开发者，我希望删除音频导入器相关代码，以便移除音频资源导入功能

#### Acceptance Criteria

1. WHEN 识别到音频导入器类（如 `ResourceImporterWAV`）的被注释代码时，THE System SHALL 删除这些代码
2. WHEN 删除导入器代码后，THE System SHALL 确保不影响其他资源导入器的功能
3. WHEN 删除后，THE System SHALL 更新导入器注册代码

### Requirement 6

**User Story:** 作为开发者，我希望验证删除后的代码可以正常编译，以便确保没有破坏现有功能

#### Acceptance Criteria

1. WHEN 完成所有音频代码删除后，THE System SHALL 能够成功编译 editor 模块
2. WHEN 编译时，THE System SHALL 不产生与已删除音频代码相关的链接错误
3. WHEN 编译时，THE System SHALL 不产生未定义符号错误
