# TinaFlowStudio 高级类型系统与序列化规范

## 概述

本文档定义 TinaFlowStudio 的完整类型系统、类型推断规则、序列化格式和版本管理策略。

---

## 1. 完整类型系统

### 1.1 基础类型（Primitive Types）

| 类型 | 说明 | 范围/格式 | 默认值 |
|------|------|----------|--------|
| `Int` | 32位整数 | -2147483648 ~ 2147483647 | 0 |
| `Float` | 64位浮点数 | IEEE 754 | 0.0 |
| `String` | UTF-8 字符串 | 无限制 | "" |
| `Bool` | 布尔值 | true/false | false |

### 1.2 复合类型（Composite Types）

#### Array（数组）
```
Array<T>
- 元素类型: T
- 可变长度
- 索引从 0 开始
- 示例: Array<Int> = [1, 2, 3]
```

#### Dictionary（字典）
```
Dictionary<K, V>
- 键类型: K (必须是 String 或 Int)
- 值类型: V
- 无序
- 示例: Dictionary<String, Int> = {"a": 1, "b": 2}
```

### 1.3 向量类型（Vector Types）

| 类型 | 说明 | 组成 | 默认值 |
|------|------|------|--------|
| `Vector2` | 2D 向量 | (x: Float, y: Float) | (0.0, 0.0) |
| `Vector3` | 3D 向量 | (x: Float, y: Float, z: Float) | (0.0, 0.0, 0.0) |
| `Color` | 颜色 | (r: Float, g: Float, b: Float, a: Float) | (1.0, 1.0, 1.0, 1.0) |

### 1.4 资源类型（Resource Types）

| 类型 | 说明 | Godot 类型 |
|------|------|-----------|
| `Texture` | 纹理资源 | Texture2D |
| `Scene` | 场景资源 | PackedScene |
| `Audio` | 音频资源 | AudioStream |
| `Font` | 字体资源 | Font |

### 1.5 对象引用类型（Object Reference Types）

| 类型 | 说明 | Godot 类型 |
|------|------|-----------|
| `Node` | 节点引用 | Node |
| `Node2D` | 2D 节点引用 | Node2D |
| `Sprite` | 精灵引用 | Sprite2D |
| `Label` | 标签引用 | Label |

### 1.6 特殊类型（Special Types）

#### Null（可空类型）
```
Null<T>
- 表示值可能不存在
- 示例: Null<Int> 可以是 42 或 null
- 访问前必须检查是否为 null
```

#### Option（选项类型）
```
Option<T> = Some(T) | None
- 更安全的可空类型
- 强制检查
- 示例: Option<Int> = Some(42) 或 None
```

#### Union（联合类型）
```
Union<T1, T2, ...>
- 表示多种可能的类型之一
- 示例: Union<Int, String> 可以是 42 或 "hello"
- 使用前需要类型判断
```

#### Any（任意类型）
```
Any
- 可以是任何类型
- 使用时需要显式类型转换
- 尽量避免使用，优先使用 Union
```

#### Execution（执行流）
```
Execution
- 特殊类型，表示控制流
- 不携带数据
- 只能用于执行端口
```

---

## 2. 类型转换规则

### 2.1 隐式转换（Implicit Conversion）

自动进行，无需显式转换节点：

| 源类型 | 目标类型 | 规则 | 示例 |
|--------|---------|------|------|
| Int | Float | 数值提升 | 42 → 42.0 |
| Int | String | 字符串化 | 42 → "42" |
| Float | String | 字符串化 | 3.14 → "3.14" |
| Bool | String | 字符串化 | true → "true" |
| Bool | Int | 布尔转整数 | true → 1, false → 0 |
| T | Null<T> | 包装 | 42 → Some(42) |
| T | Any | 类型擦除 | 42 → Any(42) |

### 2.2 显式转换（Explicit Conversion）

需要使用类型转换节点：

| 源类型 | 目标类型 | 规则 | 失败处理 |
|--------|---------|------|---------|
| Float | Int | 向下取整 | 3.14 → 3 |
| String | Int | 解析 | "42" → 42, "abc" → 0 (默认值) |
| String | Float | 解析 | "3.14" → 3.14, "abc" → 0.0 |
| String | Bool | 解析 | "true" → true, "false" → false, 其他 → false |
| Any | T | 类型检查 | 成功返回 T，失败返回默认值 |
| Null<T> | T | 解包 | Some(42) → 42, None → 默认值 |

### 2.3 类型兼容性矩阵

```
连接兼容性（输出 → 输入）：

         Int  Float String Bool Vector2 Color Texture Node Any
Int      ✓    ✓     ✓      ✓    ✗       ✗     ✗       ✗    ✓
Float    ✗    ✓     ✓      ✗    ✗       ✗     ✗       ✗    ✓
String   ✗    ✗     ✓      ✗    ✗       ✗     ✗       ✗    ✓
Bool     ✓    ✗     ✓      ✓    ✗       ✗     ✗       ✗    ✓
Vector2  ✗    ✗     ✓      ✗    ✓       ✗     ✗       ✗    ✓
Color    ✗    ✗     ✓      ✗    ✗       ✓     ✗       ✗    ✓
Texture  ✗    ✗     ✗      ✗    ✗       ✗     ✓       ✗    ✓
Node     ✗    ✗     ✗      ✗    ✗       ✗     ✗       ✓    ✓
Any      ✗    ✗     ✗      ✗    ✗       ✗     ✗       ✗    ✓

✓ = 可以直接连接（隐式转换）
✗ = 不能直接连接（需要显式转换节点）
```

---

## 3. 类型推断

### 3.1 端口类型推断

```
规则：
1. 如果端口有显式类型声明，使用声明的类型
2. 如果端口连接到其他端口，推断为连接端口的类型
3. 如果端口有默认值，推断为默认值的类型
4. 否则，推断为 Any
```

### 3.2 变量类型推断

```
规则：
1. 如果变量有显式类型声明，使用声明的类型
2. 如果变量有初始值，推断为初始值的类型
3. 如果变量被赋值，推断为第一次赋值的类型
4. 否则，推断为 Any
```

### 3.3 表达式类型推断

```
运算符类型推断：
- Int + Int → Int
- Int + Float → Float
- Float + Float → Float
- String + Any → String (字符串拼接)
- Int > Int → Bool
- Float > Float → Bool
```

---

## 4. 序列化格式

### 4.1 图文件格式（.tflow）

使用 JSON 格式，扩展名 `.tflow`：

```json
{
  "version": "1.0.0",
  "metadata": {
    "name": "My First Flow",
    "description": "A simple hello world program",
    "author": "User",
    "created_at": "2025-10-22T10:00:00Z",
    "modified_at": "2025-10-22T12:00:00Z"
  },
  "nodes": [
    {
      "id": "node_1",
      "type": "event.start",
      "position": {"x": 100, "y": 100},
      "properties": {}
    },
    {
      "id": "node_2",
      "type": "io.print",
      "position": {"x": 300, "y": 100},
      "properties": {
        "message": {
          "type": "String",
          "value": "Hello, World!"
        }
      }
    }
  ],
  "connections": [
    {
      "id": "conn_1",
      "from_node": "node_1",
      "from_port": "exec_out",
      "to_node": "node_2",
      "to_port": "exec_in"
    }
  ],
  "variables": [
    {
      "name": "counter",
      "type": "Int",
      "initial_value": 0,
      "scope": "global"
    }
  ]
}
```

### 4.2 节点定义格式

```json
{
  "id": "math.add",
  "version": "1.0.0",
  "category": "operation",
  "title": "加法",
  "description": "计算两个数的和",
  "icon": "add",
  "color": "#FFD700",
  "inputs": [
    {
      "name": "A",
      "type": "Float",
      "default": 0.0,
      "required": true,
      "description": "第一个加数"
    },
    {
      "name": "B",
      "type": "Float",
      "default": 0.0,
      "required": true,
      "description": "第二个加数"
    }
  ],
  "outputs": [
    {
      "name": "result",
      "type": "Float",
      "description": "和"
    }
  ],
  "properties": [],
  "execution": {
    "type": "builtin",
    "handler": "math_add"
  }
}
```

### 4.3 类型定义格式

```json
{
  "name": "Vector2",
  "kind": "struct",
  "fields": [
    {"name": "x", "type": "Float"},
    {"name": "y", "type": "Float"}
  ],
  "default": {"x": 0.0, "y": 0.0},
  "serialization": {
    "format": "json",
    "example": "{\"x\": 1.0, \"y\": 2.0}"
  }
}
```

---

## 5. 版本管理

### 5.1 版本号规范

使用语义化版本号（Semantic Versioning）：

```
MAJOR.MINOR.PATCH

MAJOR: 不兼容的 API 变更
MINOR: 向后兼容的功能新增
PATCH: 向后兼容的问题修复
```

### 5.2 兼容性策略

| 变更类型 | 版本号变化 | 兼容性 | 处理方式 |
|---------|-----------|--------|---------|
| 新增节点类型 | MINOR | 向后兼容 | 直接加载 |
| 新增端口 | MINOR | 向后兼容 | 使用默认值 |
| 删除节点类型 | MAJOR | 不兼容 | 需要迁移 |
| 删除端口 | MAJOR | 不兼容 | 需要迁移 |
| 修改端口类型 | MAJOR | 不兼容 | 需要迁移 |
| 修改节点行为 | PATCH | 向后兼容 | 直接加载 |

### 5.3 迁移器（Migrator）

```json
{
  "from_version": "1.0.0",
  "to_version": "2.0.0",
  "migrations": [
    {
      "type": "rename_node",
      "old_type": "io.print",
      "new_type": "io.log"
    },
    {
      "type": "rename_port",
      "node_type": "math.add",
      "old_port": "result",
      "new_port": "sum"
    },
    {
      "type": "add_port",
      "node_type": "math.add",
      "port": {
        "name": "precision",
        "type": "Int",
        "default": 2
      }
    },
    {
      "type": "remove_node",
      "old_type": "deprecated.old_node",
      "replacement": "new.better_node",
      "mapping": {
        "old_port_1": "new_port_1",
        "old_port_2": "new_port_2"
      }
    }
  ]
}
```

### 5.4 加载流程

```
加载图文件：
1. 读取 version 字段
2. 检查是否需要迁移
3. 如果需要，应用迁移器
4. 验证图的有效性
5. 加载节点和连接
6. 如果验证失败，显示错误并拒绝加载
```

---

## 6. 验证规则

### 6.1 编译期验证

#### 类型检查
```
对于每个连接：
1. 检查输出端口类型和输入端口类型
2. 如果类型不兼容，报错
3. 如果需要隐式转换，标记转换
```

#### 循环依赖检测
```
算法：拓扑排序
1. 构建数据流依赖图
2. 尝试拓扑排序
3. 如果存在环，报错并高亮环路
```

#### 必需端口检查
```
对于每个节点：
1. 检查所有 required=true 的输入端口
2. 如果未连接且无默认值，警告
```

#### 不可达节点检测
```
算法：从事件节点开始 BFS
1. 从所有事件节点开始广度优先搜索
2. 标记所有可达节点
3. 未标记的节点为不可达，警告
```

### 6.2 运行期验证

#### 空引用检查
```
访问对象引用前：
1. 检查引用是否为 null
2. 如果为 null，跳过操作并打印错误
```

#### 数组越界检查
```
访问数组元素前：
1. 检查索引是否在 [0, length) 范围内
2. 如果越界，返回 null 并打印错误
```

#### 除零检查
```
执行除法/取模前：
1. 检查除数是否为 0
2. 如果为 0，返回 NaN 并打印警告
```

---

## 7. 默认值处理

### 7.1 端口默认值

```
端口默认值优先级：
1. 连接的输出端口值（最高优先级）
2. 节点属性中设置的值
3. 端口定义中的默认值
4. 类型的默认值（最低优先级）
```

### 7.2 类型默认值表

| 类型 | 默认值 |
|------|--------|
| Int | 0 |
| Float | 0.0 |
| String | "" |
| Bool | false |
| Vector2 | (0.0, 0.0) |
| Vector3 | (0.0, 0.0, 0.0) |
| Color | (1.0, 1.0, 1.0, 1.0) |
| Array<T> | [] |
| Dictionary<K,V> | {} |
| Null<T> | None |
| Option<T> | None |
| Any | null |
| Texture | null |
| Node | null |

---

## 8. 扩展类型系统

### 8.1 自定义结构体

```json
{
  "name": "Player",
  "kind": "struct",
  "fields": [
    {"name": "name", "type": "String"},
    {"name": "health", "type": "Int"},
    {"name": "position", "type": "Vector2"}
  ],
  "default": {
    "name": "Player",
    "health": 100,
    "position": {"x": 0.0, "y": 0.0}
  }
}
```

### 8.2 枚举类型

```json
{
  "name": "Direction",
  "kind": "enum",
  "values": [
    {"name": "Up", "value": 0},
    {"name": "Down", "value": 1},
    {"name": "Left", "value": 2},
    {"name": "Right", "value": 3}
  ],
  "default": "Up"
}
```

### 8.3 泛型类型（未来）

```
Array<T>
Dictionary<K, V>
Option<T>
Result<T, E>
```

---

## 9. 验收标准

### 9.1 类型系统验收

- [ ] 支持所有基础类型（Int/Float/String/Bool）
- [ ] 支持向量类型（Vector2/Color）
- [ ] 支持数组和字典
- [ ] 支持可空类型（Null/Option）
- [ ] 支持资源引用（Texture/Scene）
- [ ] 支持对象引用（Node/Node2D）

### 9.2 类型转换验收

- [ ] 隐式转换正确（Int→Float, Bool→Int, Any→String）
- [ ] 显式转换正确（Float→Int, String→Int）
- [ ] 类型不兼容时拒绝连接
- [ ] 转换失败时使用默认值

### 9.3 序列化验收

- [ ] 图可以保存为 JSON 文件
- [ ] 图可以从 JSON 文件加载
- [ ] 保存和加载后图结构不变
- [ ] 支持版本号检查

### 9.4 验证验收

- [ ] 检测循环依赖
- [ ] 检测类型不匹配
- [ ] 检测必需端口未连接
- [ ] 检测不可达节点
- [ ] 运行期空引用检查
- [ ] 运行期数组越界检查

---

## 10. 示例：完整节点定义

```json
{
  "id": "control.loop",
  "version": "1.0.0",
  "category": "control_flow",
  "title": "循环",
  "description": "重复执行一段代码指定次数",
  "icon": "loop",
  "color": "#FF8C00",
  "inputs": [
    {
      "name": "exec_in",
      "type": "Execution",
      "required": true,
      "description": "执行流输入"
    },
    {
      "name": "count",
      "type": "Int",
      "default": 10,
      "required": true,
      "description": "循环次数",
      "validation": {
        "min": 0,
        "max": 10000
      }
    }
  ],
  "outputs": [
    {
      "name": "body",
      "type": "Execution",
      "description": "循环体执行流"
    },
    {
      "name": "index",
      "type": "Int",
      "description": "当前循环索引（从0开始）"
    },
    {
      "name": "complete",
      "type": "Execution",
      "description": "循环完成后的执行流"
    }
  ],
  "properties": [
    {
      "name": "loop_type",
      "type": "Enum",
      "values": ["fixed", "while", "infinite"],
      "default": "fixed",
      "description": "循环类型"
    }
  ],
  "execution": {
    "type": "builtin",
    "handler": "control_loop",
    "async": false
  },
  "documentation": {
    "usage": "用于重复执行一段代码。循环体会执行指定次数，每次执行时index会递增。",
    "examples": [
      {
        "title": "打印1到10",
        "description": "使用循环打印数字1到10"
      }
    ]
  }
}
```

---

## 参考资料

- TypeScript 类型系统
- Rust 类型系统（Option/Result）
- JSON Schema 规范
- Semantic Versioning 规范
- Godot Variant 类型系统
