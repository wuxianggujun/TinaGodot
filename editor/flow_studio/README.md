# TinaFlowStudio - 可视化编程系统

## 概述

TinaFlowStudio 是一个基于 Godot 引擎的可视化编程系统，采用节点图（Node Graph）的方式让用户通过拖拽和连接节点来创建程序逻辑。

## 项目结构

```
editor/flow_studio/
├── core/                      # 核心数据类
│   ├── flow_graph.h/cpp      # 流程图容器
│   ├── flow_node.h/cpp       # 流程节点
│   ├── flow_port.h/cpp       # 端口（输入/输出）
│   └── flow_connection.h/cpp # 节点连接
├── examples/                  # 使用示例
│   ├── flow_graph_example.gd # GDScript 示例
│   └── cpp_usage_example.md  # C++ 使用文档
├── README.md                  # 本文件
└── SCsub                      # 构建脚本
```

## 核心类说明

### FlowGraph（流程图）

流程图是整个可视化程序的容器，包含：
- **节点集合**：所有的流程节点
- **连接集合**：节点之间的连接关系
- **变量存储**：图级别的变量

**主要功能**：
- 添加/删除节点
- 添加/删除连接
- 管理图变量
- 序列化/反序列化（保存/加载）

### FlowNode（流程节点）

节点是可视化编程的基本单元，每个节点代表一个操作或功能。

**属性**：
- `id`：节点唯一标识
- `type`：节点类型（如 "event.start", "io.print"）
- `position`：节点在画布上的位置
- `properties`：节点的自定义属性
- `input_ports`：输入端口列表
- `output_ports`：输出端口列表

**节点类型示例**：
- `event.start` - 程序入口
- `io.print` - 打印输出
- `data.constant` - 常量
- `data.variable` - 变量
- `math.add` - 加法运算
- `control.if_else` - 条件分支

### FlowPort（端口）

端口是节点的输入/输出接口，用于连接节点之间的数据流和控制流。

**属性**：
- `name`：端口名称
- `type`：端口类型（Int, Float, String, Bool, Execution 等）
- `direction`：方向（INPUT 或 OUTPUT）
- `default_value`：默认值

**端口类型**：
- `TYPE_EXECUTION` - 执行流（控制流）
- `TYPE_INT` - 整数
- `TYPE_FLOAT` - 浮点数
- `TYPE_STRING` - 字符串
- `TYPE_BOOL` - 布尔值
- `TYPE_VECTOR2` - 二维向量
- `TYPE_COLOR` - 颜色
- `TYPE_ANY` - 任意类型

**类型兼容规则**：
- 执行流只能连接执行流
- Int 可以隐式转换为 Float
- 任意类型可以转换为 String（用于打印）
- Bool 可以转换为 Int

### FlowConnection（连接）

连接表示两个节点之间的数据流或控制流关系。

**属性**：
- `from_node`：源节点 ID
- `from_port`：源端口名称
- `to_node`：目标节点 ID
- `to_port`：目标端口名称

## 快速开始

### GDScript 使用示例

```gdscript
# 创建一个简单的 Hello World 流程图
var graph = FlowGraph.new()

# 创建 Start 节点
var start_node = FlowNode.new()
start_node.set_type("event.start")
var start_id = graph.add_node(start_node)

# 创建 Print 节点
var print_node = FlowNode.new()
print_node.set_type("io.print")
var print_id = graph.add_node(print_node)

# 连接节点
var connection = FlowConnection.new()
connection.set_from_node(start_id)
connection.set_from_port("执行")
connection.set_to_node(print_id)
connection.set_to_port("执行")
graph.add_connection(connection)

# 保存图
var dict = graph.to_dict()
var json = JSON.stringify(dict, "  ")
print(json)
```

### C++ 使用示例

```cpp
// 创建流程图
Ref<FlowGraph> graph;
graph.instantiate();

// 创建节点
Ref<FlowNode> node;
node.instantiate();
node->set_type("event.start");
node->set_position(Vector2(100, 100));

// 添加端口
Ref<FlowPort> port;
port.instantiate();
port->set_name("执行");
port->set_type(FlowPort::TYPE_EXECUTION);
node->add_output_port(port);

// 添加到图
int node_id = graph->add_node(node);
```

详细示例请参考：
- [GDScript 示例](examples/flow_graph_example.gd)
- [C++ 使用文档](examples/cpp_usage_example.md)

## 开发路线图

### ✅ 阶段 1：核心数据结构（已完成）
- [x] FlowGraph - 流程图容器
- [x] FlowNode - 流程节点
- [x] FlowPort - 端口系统
- [x] FlowConnection - 连接管理
- [x] 序列化/反序列化支持

### 🚧 阶段 2：解释执行引擎（进行中）
- [ ] ExecutionContext - 执行上下文
- [ ] EventQueue - 事件队列
- [ ] FlowGraphInterpreter - 解释器
- [ ] NodeExecutor - 节点执行器框架
- [ ] 基础节点执行器（Start, Print, Constant, Variable, Add, If）

### 📋 阶段 3：图编辑器 UI（计划中）
- [ ] FlowGraphEditor - 图编辑器主界面
- [ ] NodeLibrary - 节点库面板
- [ ] FlowGraphNode - 节点可视化
- [ ] 连接验证和绘制
- [ ] 调试支持（高亮、端口值显示）

### 📋 阶段 4：扩展功能（计划中）
- [ ] 更多节点类型（循环、等待、定时器等）
- [ ] 子图/函数节点
- [ ] 自定义节点支持
- [ ] 2D 图形节点
- [ ] 断点和单步调试

## 设计文档

- [MVP 解释执行引擎需求](../../.kiro/specs/mvp-interpreter-engine/requirements.md)
- [MVP 实现任务列表](../../.kiro/specs/mvp-interpreter-engine/tasks.md)
- [节点类型系统设计](../../.kiro/specs/node-type-system/design.md)

## 技术特点

1. **解释执行**：采用解释执行方式，支持热重载和调试
2. **强类型系统**：端口类型检查，支持有限的隐式转换
3. **事件驱动**：基于事件队列的执行模型
4. **惰性求值**：数据流按需计算，提高性能
5. **可序列化**：完整的 JSON 序列化支持

## 贡献指南

欢迎贡献代码！请遵循以下步骤：

1. Fork 本项目
2. 创建特性分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 开启 Pull Request

## 许可证

本项目遵循 Godot Engine 的 MIT 许可证。

## 联系方式

- 项目主页：https://tinaflowstudio.org
- 问题反馈：GitHub Issues

---

**当前版本**：0.1.0-alpha  
**最后更新**：2024-01
