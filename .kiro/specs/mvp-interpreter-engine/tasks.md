# TinaFlowStudio MVP - 实现任务列表

## Task List

- [x] 1. 创建项目结构和核心数据类



  - 在 Godot 项目中创建 `addons/tina_flow_studio/` 目录
  - 创建核心数据类的 GDScript 文件
  - _Requirements: 1.1, 1.2, 1.3, 1.4_



  - [ ] 1.1 创建 FlowGraph 类
    - 定义 FlowGraph.gd 文件
    - 实现 nodes, connections, variables 属性
    - 实现 add_node(), remove_node(), add_connection(), remove_connection() 方法
    - 实现 to_dict() 和 from_dict() 序列化方法


    - _Requirements: 1.1, 1.5_

  - [ ] 1.2 创建 FlowNode 类
    - 定义 FlowNode.gd 文件
    - 实现 id, type, position, properties 属性
    - 实现 input_ports, output_ports 数组

    - 实现 get_port(), add_port() 方法
    - 实现 to_dict() 和 from_dict() 序列化方法
    - _Requirements: 1.2, 1.5_





  - [ ] 1.3 创建 Port 类
    - 定义 Port.gd 文件
    - 实现 name, type, direction, default_value 属性
    - 实现 is_compatible_with() 类型兼容性检查
    - 实现 to_dict() 和 from_dict() 序列化方法
    - _Requirements: 1.3, 1.5, 6.4_

  - [ ] 1.4 创建 Connection 类
    - 定义 Connection.gd 文件
    - 实现 from_node, from_port, to_node, to_port 属性
    - 实现 to_dict() 和 from_dict() 序列化方法
    - _Requirements: 1.4, 1.5_

- [ ] 2. 实现解释执行引擎核心
  - 创建解释器和执行上下文类
  - 实现事件队列和调度
  - 实现节点执行逻辑
  - _Requirements: 2.1, 2.2, 2.3, 2.4, 2.5, 2.6_

  - [ ] 2.1 创建 ExecutionContext 类
    - 定义 ExecutionContext.gd 文件
    - 实现 graph, variables, event_queue, port_cache 属性
    - 实现 get_variable(), set_variable() 方法
    - 实现 clear_cache() 方法
    - _Requirements: 2.1_

  - [ ] 2.2 创建 EventQueue 类
    - 定义 EventQueue.gd 文件
    - 实现 events 优先级队列
    - 实现 push(), pop(), clear() 方法
    - 实现事件按优先级和时间排序
    - _Requirements: 4.1, 4.2, 4.5_

  - [ ] 2.3 创建 FlowGraphInterpreter 类
    - 定义 FlowGraphInterpreter.gd 文件
    - 实现 context, graph, node_executors 属性
    - 实现 execute_node() 方法
    - 实现 read_port_value() 方法（惰性求值 + 缓存）
    - 实现 write_port_value() 方法
    - 实现 trigger_execution_flow() 方法
    - _Requirements: 2.1, 2.2, 2.3, 2.4, 2.5_

  - [ ] 2.4 实现错误处理机制
    - 在 Interpreter 中添加 try-catch 错误捕获
    - 实现错误日志记录
    - 实现错误恢复策略（跳过错误节点，继续执行）
    - 添加错误信号（error_occurred）
    - _Requirements: 2.6_

- [ ] 3. 实现节点执行器框架
  - 创建节点执行器基类和注册机制
  - 实现 MVP 所需的 6 个节点执行器
  - _Requirements: 3.1, 3.2, 3.3, 3.4, 3.5, 3.6_

  - [ ] 3.1 创建 NodeExecutor 基类
    - 定义 NodeExecutor.gd 文件
    - 定义 execute(inputs: Dictionary, properties: Dictionary) -> Dictionary 接口
    - 添加文档注释说明执行器的使用方法
    - _Requirements: 3.1_

  - [ ] 3.2 实现执行器注册机制
    - 在 Interpreter 中添加 register_executor() 方法
    - 实现 node_type -> executor 的映射
    - 添加内置执行器的自动注册
    - _Requirements: 3.2, 3.3_

  - [ ] 3.3 实现 Start 节点执行器
    - 创建 StartExecutor.gd
    - 实现 execute() 方法（无输入，触发执行流输出）
    - 注册为 "event.start" 类型
    - _Requirements: 3.6, 5.1_

  - [ ] 3.4 实现 Print 节点执行器
    - 创建 PrintExecutor.gd
    - 实现 execute() 方法（读取 message 输入，调用 print()）
    - 注册为 "io.print" 类型
    - _Requirements: 3.6, 5.2_

  - [ ] 3.5 实现 Constant 节点执行器
    - 创建 ConstantExecutor.gd
    - 实现 execute() 方法（返回 properties 中的 value）
    - 支持 Int 和 String 类型
    - 注册为 "data.constant" 类型
    - _Requirements: 3.6, 5.3_

  - [ ] 3.6 实现 Variable 节点执行器
    - 创建 VariableExecutor.gd
    - 实现 execute() 方法（读取或写入 context.variables）
    - 支持 Get 和 Set 两种模式
    - 注册为 "data.variable" 类型
    - _Requirements: 3.6, 5.4_

  - [ ] 3.7 实现 Add 节点执行器
    - 创建 AddExecutor.gd
    - 实现 execute() 方法（A + B，支持 Int 和 Float）
    - 实现类型提升（Int -> Float）
    - 注册为 "math.add" 类型
    - _Requirements: 3.6, 5.5, 6.2_

  - [ ] 3.8 实现 If/Else 节点执行器
    - 创建 IfElseExecutor.gd
    - 实现 execute() 方法（根据 condition 选择分支）
    - 触发 true 或 false 执行流输出
    - 注册为 "control.if_else" 类型
    - _Requirements: 3.6, 5.6_

- [ ] 4. 实现类型系统
  - 定义类型枚举和默认值
  - 实现类型兼容性检查
  - 实现隐式类型转换
  - _Requirements: 6.1, 6.2, 6.3, 6.4, 6.5_

  - [ ] 4.1 创建 FlowType 类
    - 定义 FlowType.gd 文件
    - 定义类型枚举（INT, FLOAT, STRING, BOOL, EXECUTION）
    - 实现 get_default_value(type) 静态方法

    - 实现 is_compatible(from_type, to_type) 静态方法
    - 实现 convert(value, from_type, to_type) 静态方法
    - _Requirements: 6.1, 6.2, 6.3, 6.4, 6.5_

  - [x] 4.2 实现类型转换规则



    - Int -> Float 隐式转换
    - Any -> String 隐式转换
    - Bool -> Int 隐式转换
    - 添加单元测试验证转换规则
    - _Requirements: 6.2, 6.3_


- [ ] 5. 实现图编辑器 UI
  - 创建基于 GraphEdit 的编辑器界面
  - 实现节点库面板
  - 实现节点创建和连接
  - _Requirements: 7.1, 7.2, 7.3, 7.4, 7.5, 7.6_


  - [ ] 5.1 创建 FlowGraphEditor 场景
    - 创建 FlowGraphEditor.tscn 场景
    - 添加 GraphEdit 节点作为主画布
    - 添加工具栏（Run/Stop 按钮）
    - 添加节点库面板（左侧）

    - 添加控制台面板（底部）
    - _Requirements: 7.1, 8.3_

  - [ ] 5.2 创建 FlowGraphEditor 脚本
    - 创建 FlowGraphEditor.gd 脚本
    - 实现 graph 属性（当前编辑的图）
    - 实现 interpreter 属性（解释器实例）

    - 实现 _ready() 初始化
    - _Requirements: 7.1_

  - [ ] 5.3 实现节点库面板
    - 创建 NodeLibrary.gd 脚本
    - 定义 MVP 节点的元数据（名称、图标、颜色、端口）


    - 实现节点列表显示（按分类）
    - 实现拖拽创建节点功能
    - _Requirements: 7.2_

  - [ ] 5.4 实现节点可视化
    - 创建 FlowGraphNode.gd 脚本（继承 GraphNode）
    - 根据节点类型设置颜色
    - 显示节点名称和图标
    - 显示输入/输出端口
    - 根据端口类型设置端口颜色
    - _Requirements: 7.5, 7.6_

  - [ ] 5.5 实现节点创建逻辑
    - 在 FlowGraphEditor 中实现 create_node() 方法
    - 从节点库元数据创建 FlowNode 实例
    - 创建对应的 FlowGraphNode UI
    - 添加到 GraphEdit 和 FlowGraph
    - _Requirements: 7.2_

  - [ ] 5.6 实现连接创建逻辑
    - 监听 GraphEdit 的 connection_request 信号
    - 验证端口类型兼容性
    - 创建 Connection 实例
    - 在 GraphEdit 中绘制连接线
    - _Requirements: 7.3, 7.4_

  - [ ] 5.7 实现连接验证
    - 检查类型兼容性
    - 检查数据流是否形成环路（拓扑排序）
    - 检查输入端口是否已连接（单一来源）
    - 显示验证错误（红色高亮）
    - _Requirements: 7.4_

- [ ] 6. 实现执行控制和调试
  - 实现 Run/Stop 按钮
  - 实现执行高亮
  - 实现端口值显示
  - 实现控制台输出
  - _Requirements: 8.1, 8.2, 8.3, 8.4, 8.5_

  - [ ] 6.1 实现 Run/Stop 按钮
    - 在工具栏添加 Run 和 Stop 按钮
    - Run: 触发 Start 事件，开始执行
    - Stop: 清空事件队列，停止执行
    - 更新按钮状态（运行中/已停止）
    - _Requirements: 8.5_

  - [ ] 6.2 实现执行高亮
    - 在 Interpreter 中添加 node_executing 信号
    - 在 FlowGraphEditor 中监听信号
    - 高亮当前执行的节点（改变边框颜色）
    - 执行完成后取消高亮
    - _Requirements: 8.1_

  - [ ] 6.3 实现端口值显示
    - 在 FlowGraphNode 中监听鼠标悬停事件
    - 从 ExecutionContext.port_cache 读取端口值
    - 显示 Tooltip 显示端口当前值
    - _Requirements: 8.2_

  - [ ] 6.4 实现控制台面板
    - 创建 ConsolePanel.gd 脚本
    - 实现 RichTextLabel 显示日志
    - 捕获 print() 输出并显示
    - 显示错误消息（红色）
    - 实现清空按钮
    - _Requirements: 8.3, 8.4_

- [ ] 7. 实现图的保存和加载
  - 实现 JSON 序列化
  - 实现文件保存和加载
  - 实现图验证
  - _Requirements: 9.1, 9.2, 9.3, 9.4, 9.5_

  - [ ] 7.1 实现 JSON 序列化
    - 在 FlowGraph 中完善 to_dict() 方法
    - 在 FlowGraph 中完善 from_dict() 方法
    - 包含版本号、元数据、节点、连接、变量
    - 添加单元测试验证序列化正确性
    - _Requirements: 9.1, 9.4_

  - [ ] 7.2 实现文件保存
    - 在 FlowGraphEditor 中添加 save_graph() 方法
    - 使用 FileDialog 选择保存位置
    - 将 FlowGraph 序列化为 JSON
    - 保存为 .tflow 文件
    - _Requirements: 9.1_

  - [ ] 7.3 实现文件加载
    - 在 FlowGraphEditor 中添加 load_graph() 方法
    - 使用 FileDialog 选择文件
    - 从 JSON 反序列化 FlowGraph
    - 重建 UI（节点和连接）
    - _Requirements: 9.2, 9.4_

  - [ ] 7.4 实现图验证
    - 在加载时验证 JSON 格式
    - 验证节点类型是否存在
    - 验证连接的节点和端口是否存在
    - 显示验证错误并拒绝加载
    - _Requirements: 9.3, 9.5_

- [ ] 8. 创建示例程序
  - 创建 4 个示例程序的 .tflow 文件
  - 添加示例加载菜单
  - _Requirements: 10.1, 10.2, 10.3, 10.4, 10.5_

  - [ ] 8.1 创建 Hello World 示例
    - Start -> Print("Hello, World!")
    - 保存为 examples/hello_world.tflow
    - _Requirements: 10.1, 10.5_

  - [ ] 8.2 创建 Counter 示例
    - Start -> Variable(Set, counter=0) -> Print(counter) -> Variable(Set, counter=counter+1)
    - 保存为 examples/counter.tflow
    - _Requirements: 10.2, 10.5_

  - [ ] 8.3 创建 Conditional 示例
    - Start -> Variable(Set, x=5) -> If(x>3) -> Print("Big") / Print("Small")
    - 保存为 examples/conditional.tflow
    - _Requirements: 10.3, 10.5_

  - [ ] 8.4 创建 Calculator 示例
    - Start -> Constant(10) + Constant(20) -> Print(result)
    - 保存为 examples/calculator.tflow
    - _Requirements: 10.4, 10.5_

  - [ ] 8.5 添加示例加载菜单
    - 在工具栏添加 "Examples" 菜单
    - 列出所有示例
    - 点击加载对应的示例文件
    - _Requirements: 10.5_

- [ ] 9. 测试和验证
  - 编写单元测试
  - 进行集成测试
  - 性能测试
  - _Requirements: All_

  - [ ] 9.1 单元测试
    - 测试 FlowType 类型转换
    - 测试 Port 兼容性检查
    - 测试 FlowGraph 序列化
    - 测试各个 NodeExecutor
    - 使用 GDScript 的 GUT 测试框架

  - [ ] 9.2 集成测试
    - 测试完整的 Hello World 程序执行
    - 测试变量读写
    - 测试条件分支
    - 测试错误处理

  - [ ] 9.3 性能测试
    - 创建 1000 节点的测试图
    - 测量执行时间（应 < 16ms）
    - 测量内存占用（应 < 100MB）
    - 测量启动时间（应 < 1s）

- [ ] 10. 文档和打包
  - 编写用户文档
  - 创建教程
  - 打包为 Godot 插件
  - _Requirements: All_

  - [ ] 10.1 编写 README
    - 项目介绍
    - 安装说明
    - 快速开始
    - 节点参考

  - [ ] 10.2 创建快速教程
    - 5 分钟入门教程
    - 创建第一个程序
    - 使用变量和条件

  - [ ] 10.3 打包为插件
    - 创建 plugin.cfg
    - 整理文件结构
    - 测试插件安装
