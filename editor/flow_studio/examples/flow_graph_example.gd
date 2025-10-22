extends Node

## FlowGraph 核心类使用示例
## 演示如何创建节点、端口、连接，并序列化/反序列化

func _ready():
	print("=== FlowGraph 使用示例 ===\n")
	
	# 示例 1: 创建一个简单的 Hello World 流程图
	example_1_hello_world()
	
	# 示例 2: 创建一个带变量的计算流程图
	example_2_calculator()
	
	# 示例 3: 序列化和反序列化
	example_3_serialization()

## 示例 1: Hello World 流程图
## Start -> Print("Hello, World!")
func example_1_hello_world():
	print("--- 示例 1: Hello World ---")
	
	# 1. 创建流程图
	var graph = FlowGraph.new()
	
	# 2. 创建 Start 节点
	var start_node = FlowNode.new()
	start_node.set_type("event.start")
	start_node.set_position(Vector2(100, 100))
	
	# 添加输出端口（执行流）
	var start_exec_port = FlowPort.new()
	start_exec_port.set_name("执行")
	start_exec_port.set_type(FlowPort.TYPE_EXECUTION)
	start_node.add_output_port(start_exec_port)
	
	# 添加节点到图
	var start_id = graph.add_node(start_node)
	print("创建 Start 节点，ID: ", start_id)
	
	# 3. 创建 Print 节点
	var print_node = FlowNode.new()
	print_node.set_type("io.print")
	print_node.set_position(Vector2(300, 100))
	
	# 添加输入端口（执行流）
	var print_exec_in = FlowPort.new()
	print_exec_in.set_name("执行")
	print_exec_in.set_type(FlowPort.TYPE_EXECUTION)
	print_node.add_input_port(print_exec_in)
	
	# 添加输入端口（消息）
	var print_msg_in = FlowPort.new()
	print_msg_in.set_name("消息")
	print_msg_in.set_type(FlowPort.TYPE_STRING)
	print_msg_in.set_default_value("Hello, World!")
	print_node.add_input_port(print_msg_in)
	
	# 添加输出端口（执行流）
	var print_exec_out = FlowPort.new()
	print_exec_out.set_name("执行")
	print_exec_out.set_type(FlowPort.TYPE_EXECUTION)
	print_node.add_output_port(print_exec_out)
	
	# 添加节点到图
	var print_id = graph.add_node(print_node)
	print("创建 Print 节点，ID: ", print_id)
	
	# 4. 创建连接
	var connection = FlowConnection.new()
	connection.set_from_node(start_id)
	connection.set_from_port("执行")
	connection.set_to_node(print_id)
	connection.set_to_port("执行")
	graph.add_connection(connection)
	print("创建连接: Start.执行 -> Print.执行")
	
	# 5. 查询图信息
	print("图中节点数: ", graph.get_node_count())
	print("图中连接数: ", graph.get_connection_count())
	print()

## 示例 2: 计算器流程图
## Start -> Constant(10) -> Add -> Print
##       -> Constant(20) -^
func example_2_calculator():
	print("--- 示例 2: 计算器 (10 + 20) ---")
	
	var graph = FlowGraph.new()
	
	# 1. 创建 Start 节点
	var start_node = FlowNode.new()
	start_node.set_type("event.start")
	start_node.set_position(Vector2(100, 150))
	
	var start_exec = FlowPort.new()
	start_exec.set_name("执行")
	start_exec.set_type(FlowPort.TYPE_EXECUTION)
	start_node.add_output_port(start_exec)
	
	var start_id = graph.add_node(start_node)
	
	# 2. 创建 Constant(10) 节点
	var const_10 = FlowNode.new()
	const_10.set_type("data.constant")
	const_10.set_position(Vector2(300, 100))
	const_10.set_property("value", 10)
	const_10.set_property("type", "int")
	
	var const_10_out = FlowPort.new()
	const_10_out.set_name("值")
	const_10_out.set_type(FlowPort.TYPE_INT)
	const_10_out.set_default_value(10)
	const_10.add_output_port(const_10_out)
	
	var const_10_id = graph.add_node(const_10)
	
	# 3. 创建 Constant(20) 节点
	var const_20 = FlowNode.new()
	const_20.set_type("data.constant")
	const_20.set_position(Vector2(300, 200))
	const_20.set_property("value", 20)
	const_20.set_property("type", "int")
	
	var const_20_out = FlowPort.new()
	const_20_out.set_name("值")
	const_20_out.set_type(FlowPort.TYPE_INT)
	const_20_out.set_default_value(20)
	const_20.add_output_port(const_20_out)
	
	var const_20_id = graph.add_node(const_20)
	
	# 4. 创建 Add 节点
	var add_node = FlowNode.new()
	add_node.set_type("math.add")
	add_node.set_position(Vector2(500, 150))
	
	var add_a = FlowPort.new()
	add_a.set_name("A")
	add_a.set_type(FlowPort.TYPE_INT)
	add_node.add_input_port(add_a)
	
	var add_b = FlowPort.new()
	add_b.set_name("B")
	add_b.set_type(FlowPort.TYPE_INT)
	add_node.add_input_port(add_b)
	
	var add_result = FlowPort.new()
	add_result.set_name("结果")
	add_result.set_type(FlowPort.TYPE_INT)
	add_node.add_output_port(add_result)
	
	var add_id = graph.add_node(add_node)
	
	# 5. 创建 Print 节点
	var print_node = FlowNode.new()
	print_node.set_type("io.print")
	print_node.set_position(Vector2(700, 150))
	
	var print_exec_in = FlowPort.new()
	print_exec_in.set_name("执行")
	print_exec_in.set_type(FlowPort.TYPE_EXECUTION)
	print_node.add_input_port(print_exec_in)
	
	var print_msg = FlowPort.new()
	print_msg.set_name("消息")
	print_msg.set_type(FlowPort.TYPE_ANY)
	print_node.add_input_port(print_msg)
	
	var print_id = graph.add_node(print_node)
	
	# 6. 创建连接
	# Start -> Print (执行流)
	var conn1 = FlowConnection.new()
	conn1.set_from_node(start_id)
	conn1.set_from_port("执行")
	conn1.set_to_node(print_id)
	conn1.set_to_port("执行")
	graph.add_connection(conn1)
	
	# Constant(10) -> Add.A (数据流)
	var conn2 = FlowConnection.new()
	conn2.set_from_node(const_10_id)
	conn2.set_from_port("值")
	conn2.set_to_node(add_id)
	conn2.set_to_port("A")
	graph.add_connection(conn2)
	
	# Constant(20) -> Add.B (数据流)
	var conn3 = FlowConnection.new()
	conn3.set_from_node(const_20_id)
	conn3.set_from_port("值")
	conn3.set_to_node(add_id)
	conn3.set_to_port("B")
	graph.add_connection(conn3)
	
	# Add.结果 -> Print.消息 (数据流)
	var conn4 = FlowConnection.new()
	conn4.set_from_node(add_id)
	conn4.set_from_port("结果")
	conn4.set_to_node(print_id)
	conn4.set_to_port("消息")
	graph.add_connection(conn4)
	
	print("创建了 ", graph.get_node_count(), " 个节点")
	print("创建了 ", graph.get_connection_count(), " 个连接")
	
	# 7. 查询连接信息
	var add_connections = graph.get_connections_to_node(add_id)
	print("Add 节点的输入连接数: ", add_connections.size())
	print()

## 示例 3: 序列化和反序列化
func example_3_serialization():
	print("--- 示例 3: 序列化和反序列化 ---")
	
	# 1. 创建一个简单的图
	var graph = FlowGraph.new()
	
	var node = FlowNode.new()
	node.set_type("test.node")
	node.set_position(Vector2(100, 200))
	node.set_property("name", "测试节点")
	node.set_property("value", 42)
	
	var port = FlowPort.new()
	port.set_name("输出")
	port.set_type(FlowPort.TYPE_INT)
	node.add_output_port(port)
	
	graph.add_node(node)
	graph.set_variable("counter", 10)
	graph.set_variable("message", "Hello")
	
	# 2. 序列化为字典
	var dict = graph.to_dict()
	print("序列化后的字典:")
	print(JSON.stringify(dict, "  "))
	
	# 3. 从字典反序列化
	var new_graph = FlowGraph.new()
	new_graph.from_dict(dict)
	
	print("\n反序列化后的图:")
	print("节点数: ", new_graph.get_node_count())
	print("变量 counter: ", new_graph.get_variable("counter"))
	print("变量 message: ", new_graph.get_variable("message"))
	
	# 4. 验证节点信息
	var nodes = new_graph.get_all_nodes()
	if nodes.size() > 0:
		var loaded_node = nodes[0]
		print("节点类型: ", loaded_node.get_type())
		print("节点位置: ", loaded_node.get_position())
		print("节点属性 name: ", loaded_node.get_property("name"))
		print("节点属性 value: ", loaded_node.get_property("value"))
		
		var ports = loaded_node.get_output_ports()
		print("输出端口数: ", ports.size())
		if ports.size() > 0:
			var loaded_port = ports[0]
			print("端口名称: ", loaded_port.get_name())
			print("端口类型: ", FlowPort.get_type_name(loaded_port.get_type()))
	
	print()

## 示例 4: 端口类型兼容性检查
func example_4_type_compatibility():
	print("--- 示例 4: 端口类型兼容性 ---")
	
	var int_port = FlowPort.new()
	int_port.set_type(FlowPort.TYPE_INT)
	
	var float_port = FlowPort.new()
	float_port.set_type(FlowPort.TYPE_FLOAT)
	
	var string_port = FlowPort.new()
	string_port.set_type(FlowPort.TYPE_STRING)
	
	var exec_port = FlowPort.new()
	exec_port.set_type(FlowPort.TYPE_EXECUTION)
	
	print("Int -> Float: ", int_port.is_compatible_with(float_port))  # true (隐式转换)
	print("Float -> Int: ", float_port.is_compatible_with(int_port))  # false
	print("Int -> String: ", int_port.is_compatible_with(string_port))  # true (任意类型可转字符串)
	print("Exec -> Int: ", exec_port.is_compatible_with(int_port))  # false (执行流只能连执行流)
	print()
