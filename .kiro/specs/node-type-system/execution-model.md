# TinaFlowStudio 执行模型规范

## 概述

本文档定义 TinaFlowStudio 节点图的执行语义、时序模型、调度策略和确定性保证。这是实现可预测、可调试的可视化编程系统的核心规范。

---

## 1. 执行模型基础

### 1.1 执行上下文（Execution Context）

每个节点图的执行都在一个独立的执行上下文中进行：

```
ExecutionContext {
    graph: FlowGraph              // 当前执行的图
    variables: Map<String, Value> // 变量存储
    event_queue: EventQueue       // 事件队列
    execution_stack: Stack<Node>  // 执行栈（用于调试）
    state: ExecutionState         // 执行状态
    frame_time: float             // 当前帧时间
}

ExecutionState = Running | Paused | Stopped | Error
```

### 1.2 执行流（Execution Flow）

- **执行流**是一个特殊的信号，从一个节点传递到另一个节点
- 执行流通过白色的"执行端口"连接
- 执行流是**单向**的，从输出端口流向输入端口
- 执行流是**即时**的，不携带数据

### 1.3 数据流（Data Flow）

- **数据流**通过彩色的"数据端口"连接
- 数据流是**惰性求值**的：只有当节点执行时才会读取输入端口的值
- 数据流是**无状态**的：每次读取都会重新计算
- 数据流支持**缓存**：同一帧内多次读取同一端口返回相同值

---

## 2. 事件驱动模型

### 2.1 事件队列

TinaFlowStudio 使用事件驱动模型：

```
EventQueue {
    events: PriorityQueue<Event>  // 按优先级和时间排序
    
    push(event: Event)
    pop() -> Event?
    clear()
}

Event {
    type: EventType               // 事件类型
    priority: int                 // 优先级（0-100，越大越优先）
    timestamp: float              // 触发时间
    data: Map<String, Value>      // 事件数据
    source_node: Node?            // 触发节点
}

EventType = Start | KeyPress | KeyRelease | Timer | Custom
```

### 2.2 事件优先级

| 优先级 | 事件类型 | 说明 |
|--------|---------|------|
| 100 | Start | 程序启动事件，最高优先级 |
| 80 | KeyPress/KeyRelease | 用户输入事件 |
| 60 | Timer | 定时器事件 |
| 40 | Custom | 自定义事件 |

### 2.3 事件处理流程

```
每帧执行：
1. 从事件队列中取出所有当前帧应该触发的事件
2. 按优先级排序
3. 依次处理每个事件：
   a. 找到监听该事件的节点
   b. 触发节点的执行流输出端口
   c. 沿着执行流传播，执行后续节点
4. 清理已完成的事件
```

---

## 3. 节点执行语义

### 3.1 节点执行生命周期

```
节点执行流程：
1. 接收执行流信号（从输入执行端口）
2. 读取所有输入数据端口的值
3. 执行节点逻辑
4. 写入输出数据端口的值
5. 触发输出执行端口（如果有）
```

### 3.2 执行流传播规则

#### 单一输出（如：Print、变量赋值）
```
[节点A] --执行--> [节点B]
```
- 节点 A 执行完成后，立即触发节点 B

#### 条件分支（If/Else）
```
         ┌--True--> [节点B]
[节点A] --|
         └--False--> [节点C]
```
- 节点 A 执行完成后，根据条件选择一个分支触发
- **只有一个分支会被触发**

#### 序列（Sequence）
```
         ┌--Then1--> [节点B]
[节点A] --├--Then2--> [节点C]
         └--Then3--> [节点D]
```
- 节点 A 执行完成后，**按顺序**触发所有输出
- 执行顺序：Then1 → Then2 → Then3
- 每个分支执行完成后才执行下一个

#### 并行（未来功能）
```
         ┌==Parallel1==> [节点B]
[节点A] ==╪==Parallel2==> [节点C]
         └==Parallel3==> [节点D]
```
- 节点 A 执行完成后，**同时**触发所有输出
- 各分支独立执行，互不阻塞

### 3.3 循环执行

循环节点的特殊处理：

```
Loop {
    输入: 执行流, 次数N
    输出: 循环体执行流, 索引, 完成执行流
    
    执行逻辑:
    1. 接收执行流
    2. for i in 0..N:
       a. 设置索引输出 = i
       b. 触发循环体执行流
       c. 等待循环体执行完成
    3. 触发完成执行流
}
```

**重要**：循环体内的执行是**同步**的，每次迭代必须完成后才能进入下一次。

---

## 4. 时间与等待

### 4.1 帧时间模型

TinaFlowStudio 与 Godot 的主循环集成：

```
每帧（_process(delta)）：
1. 更新 frame_time += delta
2. 处理事件队列
3. 更新所有等待中的 Wait 节点
4. 执行触发的节点
5. 渲染（如果有 2D 图形节点）
```

### 4.2 Wait 节点语义

```
Wait {
    输入: 执行流, 持续时间
    输出: 完成执行流
    
    执行逻辑:
    1. 接收执行流
    2. 记录开始时间 start_time = frame_time
    3. 每帧检查: if frame_time - start_time >= 持续时间
       a. 触发完成执行流
       b. 移除等待状态
    
    状态: Waiting | Completed
}
```

**重要**：Wait 节点是**异步**的，不会阻塞其他执行流。

### 4.3 Timer 事件语义

```
Timer {
    属性: 间隔时间, 是否循环
    输出: 执行流, 计数
    
    执行逻辑:
    1. 初始化: next_trigger_time = frame_time + 间隔时间
    2. 每帧检查: if frame_time >= next_trigger_time
       a. 触发执行流
       b. 计数 += 1
       c. if 循环: next_trigger_time += 间隔时间
          else: 停止计时器
}
```

---

## 5. 确定性保证

### 5.1 确定性原则

**相同的输入 + 相同的事件序列 = 相同的输出**

为了保证确定性：

1. **事件顺序固定**：相同优先级的事件按插入顺序处理
2. **执行顺序固定**：Sequence 节点严格按顺序执行
3. **随机数可控**：提供种子控制的随机数节点
4. **时间可控**：支持固定时间步长模式（用于测试）

### 5.2 非确定性来源

以下情况可能导致非确定性：

1. **真实时间**：使用系统时间而非帧时间
2. **外部输入**：用户输入、网络请求
3. **并行执行**：未来如果支持并行，需要额外的同步机制

### 5.3 调试模式

```
DebugMode {
    fixed_delta: float = 0.016  // 固定时间步长（60 FPS）
    seed: int = 12345           // 随机数种子
    record_events: bool = true  // 记录所有事件
    replay_events: Event[]?     // 重放事件序列
}
```

---

## 6. 错误处理

### 6.1 编译期错误

在图加载/验证时检测：

| 错误类型 | 说明 | 处理 |
|---------|------|------|
| 循环依赖 | 数据流形成环路 | 拒绝加载，高亮环路 |
| 类型不匹配 | 端口类型不兼容 | 拒绝加载，标记连接 |
| 缺少必需输入 | 必需端口未连接 | 警告，使用默认值 |
| 多重连接 | 输入端口连接多个输出 | 拒绝加载，标记端口 |
| 不可达节点 | 节点无法从事件到达 | 警告，灰显节点 |

### 6.2 运行期错误

在节点执行时检测：

| 错误类型 | 说明 | 处理 |
|---------|------|------|
| 除零错误 | 除法/取模的除数为 0 | 返回 NaN，继续执行 |
| 类型转换失败 | 无法转换类型 | 返回默认值，打印警告 |
| 空引用 | 访问不存在的对象 | 跳过操作，打印错误 |
| 数组越界 | 索引超出范围 | 返回 null，打印错误 |
| 超时 | 节点执行时间过长 | 中断执行，打印错误 |

### 6.3 错误恢复策略

```
错误处理流程：
1. 捕获错误
2. 记录错误信息（节点、类型、消息）
3. 根据错误级别决定：
   - Warning: 打印警告，继续执行
   - Error: 打印错误，跳过当前节点，继续执行
   - Fatal: 停止整个图的执行
4. 在 UI 中高亮错误节点
```

---

## 7. 性能与优化

### 7.1 执行优化

1. **惰性求值**：只计算实际需要的数据
2. **缓存**：同一帧内缓存端口值
3. **死代码消除**：不执行不可达节点
4. **常量折叠**：编译期计算常量表达式

### 7.2 性能监控

```
PerformanceMetrics {
    node_execution_time: Map<Node, float>  // 每个节点的执行时间
    node_call_count: Map<Node, int>        // 每个节点的调用次数
    frame_time: float                      // 当前帧总时间
    event_count: int                       // 事件队列大小
}
```

### 7.3 性能限制

| 限制项 | 默认值 | 说明 |
|-------|--------|------|
| 最大节点数 | 10000 | 单个图的最大节点数 |
| 最大执行深度 | 1000 | 防止无限递归 |
| 单帧最大执行时间 | 16ms | 保证 60 FPS |
| 事件队列大小 | 1000 | 防止事件堆积 |

---

## 8. 与 Godot 集成

### 8.1 主循环集成

```gdscript
# FlowGraphExecutor.gd
extends Node

var context: ExecutionContext
var graph: FlowGraph

func _ready():
    # 初始化执行上下文
    context = ExecutionContext.new()
    context.graph = graph
    
    # 触发 Start 事件
    var start_event = Event.new()
    start_event.type = EventType.Start
    start_event.priority = 100
    context.event_queue.push(start_event)

func _process(delta: float):
    # 更新帧时间
    context.frame_time += delta
    
    # 处理事件
    process_events()
    
    # 更新等待节点
    update_wait_nodes()
    
    # 更新定时器
    update_timers()

func _input(event: InputEvent):
    # 转换为 FlowGraph 事件
    if event is InputEventKey:
        var flow_event = Event.new()
        flow_event.type = EventType.KeyPress if event.pressed else EventType.KeyRelease
        flow_event.data["key"] = event.keycode
        context.event_queue.push(flow_event)
```

### 8.2 线程模型

- **主线程执行**：所有节点在 Godot 主线程执行
- **无并发**：当前版本不支持多线程执行
- **异步操作**：Wait 节点通过帧循环实现异步

### 8.3 资源管理

```
资源生命周期：
1. 节点创建时加载资源（Texture、Scene 等）
2. 资源引用计数管理
3. 图销毁时释放所有资源
```

---

## 9. 执行策略：解释执行优先

### 9.1 核心决策：解释执行

**TinaFlowStudio 采用解释执行作为主要执行方式**

理由：
1. **开发体验优先**：支持热重载、即时反馈
2. **调试友好**：完整的断点、单步、变量监视
3. **实现简单**：快速迭代，易于维护
4. **教学导向**：可视化执行过程，帮助理解
5. **灵活性高**：动态修改图，无需重新编译

### 9.2 解释执行实现

#### 核心组件

```gdscript
# FlowGraphInterpreter.gd
class_name FlowGraphInterpreter
extends RefCounted

var context: ExecutionContext
var graph: FlowGraph
var node_executors: Dictionary  # node_type -> NodeExecutor

func execute_node(node: FlowNode):
    # 1. 获取节点执行器
    var executor = node_executors[node.type]
    
    # 2. 读取输入端口值
    var inputs = {}
    for port in node.input_ports:
        inputs[port.name] = read_port_value(node, port)
    
    # 3. 执行节点逻辑
    var outputs = executor.execute(inputs, node.properties)
    
    # 4. 写入输出端口值
    for port_name in outputs:
        write_port_value(node, port_name, outputs[port_name])
    
    # 5. 触发执行流输出
    for exec_port in node.exec_output_ports:
        trigger_execution_flow(node, exec_port)

func read_port_value(node: FlowNode, port: Port) -> Variant:
    # 检查缓存
    if context.port_cache.has(node.id + "." + port.name):
        return context.port_cache[node.id + "." + port.name]
    
    # 查找连接
    var connection = find_input_connection(node, port)
    if connection:
        var source_node = graph.get_node(connection.from_node)
        var value = read_port_value(source_node, connection.from_port)
        context.port_cache[node.id + "." + port.name] = value
        return value
    
    # 使用默认值
    return get_default_value(node, port)
```

#### 节点执行器接口

```gdscript
# NodeExecutor.gd
class_name NodeExecutor
extends RefCounted

# 执行节点逻辑
# @param inputs: Dictionary - 输入端口值 {port_name: value}
# @param properties: Dictionary - 节点属性 {property_name: value}
# @return Dictionary - 输出端口值 {port_name: value}
func execute(inputs: Dictionary, properties: Dictionary) -> Dictionary:
    push_error("NodeExecutor.execute() must be overridden")
    return {}

# 示例：Print 节点执行器
class PrintExecutor extends NodeExecutor:
    func execute(inputs: Dictionary, properties: Dictionary) -> Dictionary:
        var message = inputs.get("message", "")
        print(message)
        return {}

# 示例：Add 节点执行器
class AddExecutor extends NodeExecutor:
    func execute(inputs: Dictionary, properties: Dictionary) -> Dictionary:
        var a = inputs.get("A", 0.0)
        var b = inputs.get("B", 0.0)
        return {"result": a + b}
```

### 9.3 性能优化策略

虽然是解释执行，但仍可以优化：

1. **端口值缓存**：同一帧内缓存端口值，避免重复计算
2. **死代码消除**：不执行不可达节点
3. **常量折叠**：编译期计算常量表达式
4. **执行路径缓存**：缓存常用的执行路径
5. **惰性求值**：只计算实际需要的值

```gdscript
# 优化示例：常量折叠
func optimize_graph(graph: FlowGraph):
    for node in graph.nodes:
        if node.type == "math.add":
            var a_conn = find_input_connection(node, "A")
            var b_conn = find_input_connection(node, "B")
            
            # 如果两个输入都是常量，直接计算结果
            if is_constant(a_conn) and is_constant(b_conn):
                var result = a_conn.value + b_conn.value
                replace_with_constant(node, result)
```

### 9.4 未来：代码生成支持

**阶段 2（未来版本）**：添加可选的代码生成

```
混合策略：
- 开发期：解释执行 + 完整调试
- 发布期：代码生成 + 性能优化
- 用户可选：根据需求切换
```

#### 代码生成流程

```
1. 分析图结构
2. 生成 GDScript 代码
3. 优化代码（内联、循环展开等）
4. 保存为 .gd 文件
5. Godot 编译为字节码
```

#### 生成代码示例

```gdscript
# 从图生成的 GDScript 代码
extends Node

var counter: int = 0

func _ready():
    # Start 节点
    _node_start()

func _node_start():
    # Print 节点
    print("Hello, World!")
    # 触发下一个节点
    _node_wait()

func _node_wait():
    # Wait 节点
    await get_tree().create_timer(1.0).timeout
    # 触发下一个节点
    _node_print_world()

func _node_print_world():
    print("World!")
```

### 9.5 性能对比（预期）

| 指标 | 解释执行 | 代码生成 |
|------|---------|---------|
| 启动时间 | 快（< 100ms） | 慢（需要生成代码） |
| 执行速度 | 中等（1000 节点/帧） | 快（10000+ 节点/帧） |
| 内存占用 | 中等 | 低 |
| 调试支持 | 完整 | 有限 |
| 热重载 | 支持 | 不支持 |
| 适用场景 | 开发、教学、中小型项目 | 发布、大型项目 |

### 9.6 实现优先级

**MVP（第一版）**：
- ✅ 解释执行引擎
- ✅ 基础节点执行器
- ✅ 事件驱动模型
- ✅ 调试支持（断点、单步）

**第二版**：
- ⏳ 性能优化（缓存、死代码消除）
- ⏳ 更多节点类型
- ⏳ 子图/函数支持

**第三版**：
- ⏳ 可选的代码生成
- ⏳ 性能分析工具
- ⏳ 发布优化

---

## 10. 示例：完整执行流程

### 示例图

```
[Start] --执行--> [Print "Hello"] --执行--> [Wait 1s] --完成--> [Print "World"]
```

### 执行时序

```
帧 0 (t=0.000s):
  1. 触发 Start 事件
  2. 执行 Start 节点
  3. 触发 Print "Hello" 节点
  4. 输出: "Hello"
  5. 触发 Wait 节点
  6. Wait 节点进入等待状态 (target_time = 1.000s)

帧 1-59 (t=0.016s - 0.984s):
  1. 检查 Wait 节点
  2. 未到时间，继续等待

帧 60 (t=1.000s):
  1. 检查 Wait 节点
  2. 时间到达，触发完成执行流
  3. 执行 Print "World" 节点
  4. 输出: "World"
  5. 执行完成
```

---

## 11. 验收标准（MVP）

### 11.1 功能验收

- [ ] 支持 Start 事件触发
- [ ] 支持顺序执行（A → B → C）
- [ ] 支持条件分支（If/Else）
- [ ] 支持循环（固定次数）
- [ ] 支持 Wait 节点（异步等待）
- [ ] 支持变量读写
- [ ] 支持基本运算（加减乘除）

### 11.2 确定性验收

- [ ] 相同输入产生相同输出
- [ ] 事件顺序可预测
- [ ] 支持固定时间步长模式

### 11.3 错误处理验收

- [ ] 检测循环依赖
- [ ] 检测类型不匹配
- [ ] 运行期错误不崩溃
- [ ] 错误信息清晰可读

### 11.4 性能验收

- [ ] 1000 个节点的图可以流畅运行（60 FPS）
- [ ] 单帧执行时间 < 16ms
- [ ] 内存占用合理（< 100MB）

---

## 12. 未来扩展

### 12.1 并行执行

- 支持并行执行流
- 提供同步原语（Join、Barrier）
- 线程安全的变量访问

### 12.2 协程支持

- 支持 yield/await 语义
- 支持异步函数节点
- 支持异步资源加载

### 12.3 子图/函数

- 支持将图封装为可复用的函数节点
- 支持参数传递和返回值
- 支持递归调用

---

## 参考资料

- Unreal Engine Blueprint 执行模型
- Unity Visual Scripting 执行模型
- Node-RED 流程引擎
- Godot GDScript 协程机制
