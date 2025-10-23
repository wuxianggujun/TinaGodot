# TinaFlowStudio MVP - 解释执行引擎需求文档

## Introduction

本文档定义 TinaFlowStudio MVP（最小可行产品）的需求。MVP 的目标是实现一个基于解释执行的可视化编程系统，支持基础节点和简单程序的运行。

## Glossary

- **FlowGraph**：节点图，包含节点和连接的完整程序
- **FlowNode**：流程节点，可视化编程的基本单元
- **Port**：端口，节点的输入/输出接口
- **Connection**：连接，连接两个端口的线
- **Interpreter**：解释器，执行节点图的引擎
- **Executor**：执行器，执行单个节点的逻辑
- **ExecutionContext**：执行上下文，存储执行状态和变量

## Requirements

### Requirement 1: 核心数据结构

**User Story:** 作为开发者，我需要定义节点图的核心数据结构，以便存储和操作可视化程序。

#### Acceptance Criteria

1. THE System SHALL define a FlowGraph class that contains nodes and connections
2. THE System SHALL define a FlowNode class with id, type, position, properties, and ports
3. THE System SHALL define a Port class with name, type, direction (input/output), and connection info
4. THE System SHALL define a Connection class with from_node, from_port, to_node, to_port
5. THE System SHALL support serialization to JSON format

### Requirement 2: 解释执行引擎

**User Story:** 作为开发者，我需要一个解释执行引擎，以便运行节点图程序。

#### Acceptance Criteria

1. THE System SHALL provide a FlowGraphInterpreter class that executes FlowGraph
2. WHEN a Start event is triggered, THE Interpreter SHALL begin execution from Start nodes
3. THE Interpreter SHALL follow execution flow connections to execute nodes in sequence
4. THE Interpreter SHALL evaluate data flow connections lazily (only when needed)
5. THE Interpreter SHALL cache port values within a single frame to avoid redundant computation
6. THE Interpreter SHALL handle execution errors gracefully without crashing

### Requirement 3: 节点执行器框架

**User Story:** 作为开发者，我需要一个节点执行器框架，以便实现不同类型节点的逻辑。

#### Acceptance Criteria

1. THE System SHALL define a NodeExecutor base class with execute() method
2. THE System SHALL support registering custom executors for different node types
3. WHEN a node is executed, THE System SHALL call the appropriate executor
4. THE Executor SHALL receive input port values and node properties
5. THE Executor SHALL return output port values
6. THE System SHALL provide built-in executors for MVP nodes (Start, Print, Constant, Variable, Add, If)

### Requirement 4: 事件系统

**User Story:** 作为用户，我需要事件驱动的执行模型，以便程序可以响应不同的触发条件。

#### Acceptance Criteria

1. THE System SHALL provide an EventQueue class for managing events
2. THE System SHALL support Start event type with highest priority
3. WHEN an event is triggered, THE System SHALL find all nodes listening to that event
4. THE System SHALL execute event handler nodes in priority order
5. THE System SHALL process events every frame in _process()

### Requirement 5: 基础节点实现

**User Story:** 作为用户，我需要基础节点来构建简单程序。

#### Acceptance Criteria

1. THE System SHALL provide a Start node (event entry point)
2. THE System SHALL provide a Print node (output to console)
3. THE System SHALL provide Constant nodes (Int and String)
4. THE System SHALL provide a Variable node (read and write)
5. THE System SHALL provide an Add node (addition operation)
6. THE System SHALL provide an If/Else node (conditional branching)
7. ALL nodes SHALL follow the node definition schema (id, type, ports, properties)

### Requirement 6: 类型系统

**User Story:** 作为开发者，我需要基础的类型系统，以便进行类型检查和转换。

#### Acceptance Criteria

1. THE System SHALL support Int, Float, String, Bool, and Execution types
2. THE System SHALL perform implicit conversion from Int to Float
3. THE System SHALL perform implicit conversion from any type to String
4. THE System SHALL validate type compatibility when connecting ports
5. THE System SHALL provide default values for all types (Int=0, String="", Bool=false)

### Requirement 7: 图编辑器 UI

**User Story:** 作为用户，我需要一个图形界面来创建和编辑节点图。

#### Acceptance Criteria

1. THE System SHALL provide a GraphEdit-based canvas for node editing
2. WHEN a user drags a node from the library, THE System SHALL create a new node instance
3. WHEN a user connects two ports, THE System SHALL create a connection
4. THE System SHALL validate connections (type compatibility, no cycles in data flow)
5. THE System SHALL display nodes with different colors based on category
6. THE System SHALL show port types with color coding

### Requirement 8: 调试支持

**User Story:** 作为用户，我需要基础的调试功能，以便理解程序的执行过程。

#### Acceptance Criteria

1. WHEN a node is executing, THE System SHALL highlight the node
2. THE System SHALL display current port values when hovering over ports
3. THE System SHALL output execution logs to a console panel
4. THE System SHALL show error messages when execution fails
5. THE System SHALL provide a Run/Stop button to control execution

### Requirement 9: 图的保存和加载

**User Story:** 作为用户，我需要保存和加载我的程序，以便持久化工作。

#### Acceptance Criteria

1. THE System SHALL save FlowGraph to JSON file (.tflow extension)
2. THE System SHALL load FlowGraph from JSON file
3. THE System SHALL validate graph structure when loading
4. THE System SHALL preserve node positions, properties, and connections
5. THE System SHALL handle file format errors gracefully

### Requirement 10: 示例程序

**User Story:** 作为用户，我需要示例程序来学习如何使用系统。

#### Acceptance Criteria

1. THE System SHALL provide a "Hello World" example
2. THE System SHALL provide a "Counter" example (using variables)
3. THE System SHALL provide a "Conditional" example (using If/Else)
4. THE System SHALL provide a "Calculator" example (using Add and variables)
5. ALL examples SHALL be loadable and runnable

## Non-Functional Requirements

### Performance

1. THE System SHALL support graphs with up to 1000 nodes
2. THE System SHALL maintain 60 FPS (frame time < 16ms)
3. THE System SHALL start up in less than 1 second

### Usability

1. THE System SHALL be learnable within 5 minutes for basic operations
2. THE System SHALL provide clear error messages in Chinese
3. THE System SHALL have responsive UI with no noticeable lag

### Reliability

1. THE System SHALL NOT crash on invalid input
2. THE System SHALL handle all runtime errors gracefully
3. THE System SHALL validate all user operations

## Out of Scope (Future Versions)

- 循环节点（Loop）
- 等待节点（Wait）
- 定时器事件（Timer）
- 2D 图形节点
- 断点和单步调试
- 代码生成
- 子图/函数节点
- 自定义节点
- 多语言支持
