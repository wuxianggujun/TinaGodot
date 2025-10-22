# FlowStudio 核心类 C++ 使用指南

## 概述

FlowStudio 核心数据类提供了构建可视化编程系统的基础结构。所有类都是纯 C++ 实现，继承自 `RefCounted`，使用 Godot 的引用计数系统。

## 核心类

### 1. FlowPort - 端口类

端口是节点的输入/输出接口。

```cpp
#include "editor/flow_studio/core/flow_port.h"

// 创建一个整数输入端口
Ref<FlowPort> input_port;
input_port.instantiate();
input_port->set_name("value");
input_port->set_type(FlowPort::TYPE_INT);
input_port->set_direction(FlowPort::INPUT);
input_port->set_default_value(0);

// 创建一个执行流输出端口
Ref<FlowPort> exec_port;
exec_port.instantiate();
exec_port->set_name("execute");
exec_port->set_type(FlowPort::TYPE_EXECUTION);
exec_port->set_direction(FlowPort::OUTPUT);

// 类型兼容性检查
if (port1->is_compatible_with(port2)) {
    // 可以连接
}

// 获取类型默认值
Variant default_val = FlowPort::get_type_default_value(FlowPort::TYPE_INT); // 返回 0
```

**支持的端口类型：**
- `TYPE_EXECUTION` - 执行流（控制流）
- `TYPE_INT` - 整数
- `TYPE_FLOAT` - 浮点数
- `TYPE_STRING` - 字符串
- `TYPE_BOOL` - 布尔值
- `TYPE_VECTOR2` - 2D 向量
- `TYPE_COLOR` - 颜色
- `TYPE_ANY` - 任意类型

### 2. FlowNode - 节点类

节点是可视化编程的基本单元。

```cpp
#include "editor/flow_studio/core/flow_node.h"

// 创建一个节点
Ref<FlowNode> node;
node.instantiate();
node->set_type("math.add");
node->set_position(Vector2(100, 200));

// 添加属性
node->set_property("operation", "+");
node->set_property("description", "加法节点");

// 添加输入端口
Ref<FlowPort> input_a;
input_a.instantiate();
input_a->set_name("A");
input_a->set_type(FlowPort::TYPE_INT);
node->add_input_port(input_a);

Ref<FlowPort> input_b;
input_b.instantiate();
input_b->set_name("B");
input_b->set_type(FlowPort::TYPE_INT);
node->add_input_port(input_b);

// 添加输出端口
Ref<FlowPort> output;
output.instantiate();
output->set_name("Result");
output->set_type(FlowPort::TYPE_INT);
node->add_output_port(output);

// 获取端口
Ref<FlowPort> port = node->get_input_port("A");
if (port.is_valid()) {
    // 使用端口
}

// 获取所有端口
Array inputs = node->get_input_ports();
for (int i = 0; i < inputs.size(); i++) {
    Ref<FlowPort> p = inputs[i];
    print_line("Input port: " + p->get_name());
}
```

### 3. FlowConnection - 连接类

连接将两个节点的端口连接起来。

```cpp
#include "editor/flow_studio/core/flow_connection.h"

// 创建连接
Ref<FlowConnection> conn;
conn.instantiate();
conn->set_from_node(node1_id);
conn->set_from_port("Result");
conn->set_to_node(node2_id);
conn->set_to_port("A");

// 或使用构造函数
Ref<FlowConnection> conn2;
conn2.instantiate();
// 注意：需要手动设置，因为 Godot 的 Ref 不支持带参数的构造
conn2->set_from_node(1);
conn2->set_from_port("output");
conn2->set_to_node(2);
conn2->set_to_port("input");

// 验证连接
if (conn->is_valid()) {
    // 连接有效
}

// 比较连接
if (conn1->equals(conn2)) {
    // 两个连接相同
}
```

### 4. FlowGraph - 流程图类

流程图管理所有节点和连接。

```cpp
#include "editor/flow_studio/core/flow_graph.h"

// 创建流程图
Ref<FlowGraph> graph;
graph.instantiate();

// 添加节点
Ref<FlowNode> start_node;
start_node.instantiate();
start_node->set_type("event.start");
int start_id = graph->add_node(start_node);

Ref<FlowNode> print_node;
print_node.instantiate();
print_node->set_type("io.print");
int print_id = graph->add_node(print_node);

// 添加连接
Ref<FlowConnection> conn;
conn.instantiate();
conn->set_from_node(start_id);
conn->set_from_port("execute");
conn->set_to_node(print_id);
conn->set_to_port("execute");
graph->add_connection(conn);

// 获取节点
Ref<FlowNode> node = graph->get_node(start_id);
if (node.is_valid()) {
    print_line("Node type: " + node->get_type());
}

// 获取所有节点
Array nodes = graph->get_all_nodes();
for (int i = 0; i < nodes.size(); i++) {
    Ref<FlowNode> n = nodes[i];
    print_line("Node " + itos(n->get_id()) + ": " + n->get_type());
}

// 获取节点的连接
Array from_conns = graph->get_connections_from_node(start_id);
Array to_conns = graph->get_connections_to_node(print_id);

// 变量管理
graph->set_variable("counter", 0);
int counter = graph->get_variable("counter", 0);
graph->set_variable("counter", counter + 1);

// 删除节点（会自动删除相关连接）
graph->remove_node(start_id);

// 清空图
graph->clear();
```

## 序列化和持久化

所有核心类都支持序列化为 Dictionary，可以保存为 JSON 文件。

```cpp
// 序列化图
Dictionary graph_dict = graph->to_dict();

// 保存为 JSON
String json = JSON::stringify(graph_dict, "\t");
Ref<FileAccess> file = FileAccess::open("res://my_graph.tflow", FileAccess::WRITE);
if (file.is_valid()) {
    file->store_string(json);
}

// 从 JSON 加载
Ref<FileAccess> load_file = FileAccess::open("res://my_graph.tflow", FileAccess::READ);
if (load_file.is_valid()) {
    String json_str = load_file->get_as_text();
    Ref<JSON> json_parser;
    json_parser.instantiate();
    Error err = json_parser->parse(json_str);
    if (err == OK) {
        Dictionary loaded_dict = json_parser->get_data();
        graph->from_dict(loaded_dict);
    }
}
```

## 完整示例：创建一个简单的加法程序

```cpp
void create_add_program() {
    // 创建图
    Ref<FlowGraph> graph;
    graph.instantiate();
    
    // 1. 创建 Start 节点
    Ref<FlowNode> start_node;
    start_node.instantiate();
    start_node->set_type("event.start");
    start_node->set_position(Vector2(100, 100));
    
    Ref<FlowPort> start_exec;
    start_exec.instantiate();
    start_exec->set_name("execute");
    start_exec->set_type(FlowPort::TYPE_EXECUTION);
    start_node->add_output_port(start_exec);
    
    int start_id = graph->add_node(start_node);
    
    // 2. 创建常量节点 A (值=10)
    Ref<FlowNode> const_a;
    const_a.instantiate();
    const_a->set_type("data.constant");
    const_a->set_position(Vector2(100, 200));
    const_a->set_property("value", 10);
    
    Ref<FlowPort> const_a_out;
    const_a_out.instantiate();
    const_a_out->set_name("value");
    const_a_out->set_type(FlowPort::TYPE_INT);
    const_a->add_output_port(const_a_out);
    
    int const_a_id = graph->add_node(const_a);
    
    // 3. 创建常量节点 B (值=20)
    Ref<FlowNode> const_b;
    const_b.instantiate();
    const_b->set_type("data.constant");
    const_b->set_position(Vector2(100, 300));
    const_b->set_property("value", 20);
    
    Ref<FlowPort> const_b_out;
    const_b_out.instantiate();
    const_b_out->set_name("value");
    const_b_out->set_type(FlowPort::TYPE_INT);
    const_b->add_output_port(const_b_out);
    
    int const_b_id = graph->add_node(const_b);
    
    // 4. 创建加法节点
    Ref<FlowNode> add_node;
    add_node.instantiate();
    add_node->set_type("math.add");
    add_node->set_position(Vector2(300, 250));
    
    Ref<FlowPort> add_in_a;
    add_in_a.instantiate();
    add_in_a->set_name("A");
    add_in_a->set_type(FlowPort::TYPE_INT);
    add_node->add_input_port(add_in_a);
    
    Ref<FlowPort> add_in_b;
    add_in_b.instantiate();
    add_in_b->set_name("B");
    add_in_b->set_type(FlowPort::TYPE_INT);
    add_node->add_input_port(add_in_b);
    
    Ref<FlowPort> add_out;
    add_out.instantiate();
    add_out->set_name("Result");
    add_out->set_type(FlowPort::TYPE_INT);
    add_node->add_output_port(add_out);
    
    int add_id = graph->add_node(add_node);
    
    // 5. 创建打印节点
    Ref<FlowNode> print_node;
    print_node.instantiate();
    print_node->set_type("io.print");
    print_node->set_position(Vector2(500, 200));
    
    Ref<FlowPort> print_exec_in;
    print_exec_in.instantiate();
    print_exec_in->set_name("execute");
    print_exec_in->set_type(FlowPort::TYPE_EXECUTION);
    print_node->add_input_port(print_exec_in);
    
    Ref<FlowPort> print_msg_in;
    print_msg_in.instantiate();
    print_msg_in->set_name("message");
    print_msg_in->set_type(FlowPort::TYPE_ANY);
    print_node->add_input_port(print_msg_in);
    
    Ref<FlowPort> print_exec_out;
    print_exec_out.instantiate();
    print_exec_out->set_name("execute");
    print_exec_out->set_type(FlowPort::TYPE_EXECUTION);
    print_node->add_output_port(print_exec_out);
    
    int print_id = graph->add_node(print_node);
    
    // 6. 创建连接
    // Start -> Print (执行流)
    Ref<FlowConnection> conn1;
    conn1.instantiate();
    conn1->set_from_node(start_id);
    conn1->set_from_port("execute");
    conn1->set_to_node(print_id);
    conn1->set_to_port("execute");
    graph->add_connection(conn1);
    
    // Const A -> Add.A (数据流)
    Ref<FlowConnection> conn2;
    conn2.instantiate();
    conn2->set_from_node(const_a_id);
    conn2->set_from_port("value");
    conn2->set_to_node(add_id);
    conn2->set_to_port("A");
    graph->add_connection(conn2);
    
    // Const B -> Add.B (数据流)
    Ref<FlowConnection> conn3;
    conn3.instantiate();
    conn3->set_from_node(const_b_id);
    conn3->set_from_port("value");
    conn3->set_to_node(add_id);
    conn3->set_to_port("B");
    graph->add_connection(conn3);
    
    // Add.Result -> Print.message (数据流)
    Ref<FlowConnection> conn4;
    conn4.instantiate();
    conn4->set_from_node(add_id);
    conn4->set_from_port("Result");
    conn4->set_to_node(print_id);
    conn4->set_to_port("message");
    graph->add_connection(conn4);
    
    // 7. 保存图
    Dictionary graph_dict = graph->to_dict();
    String json = JSON::stringify(graph_dict, "\t");
    print_line("Graph JSON:");
    print_line(json);
    
    // 输出统计
    print_line("Graph created:");
    print_line("  Nodes: " + itos(graph->get_node_count()));
    print_line("  Connections: " + itos(graph->get_connection_count()));
}
```

## 下一步

这些核心数据类已经准备好了。接下来你需要：

1. **解释执行引擎** - 实现 `FlowGraphInterpreter` 来执行图
2. **节点执行器** - 为每种节点类型实现执行逻辑
3. **图编辑器 UI** - 创建可视化编辑界面

所有这些都将使用纯 C++ 实现，不需要 GDScript。
