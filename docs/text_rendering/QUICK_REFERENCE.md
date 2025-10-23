# 文本渲染快速参考卡片

> 快速查找关键概念、API和代码位置

## 🎯 核心概念速查

### 字形 vs 字符
```
字符（Character）: Unicode码点，如 'A' (U+0041)
字形（Glyph）: 视觉表现，一个字符可能有多个字形
```

### 抗锯齿模式
```gdscript
FONT_ANTIALIASING_NONE  # 无抗锯齿
FONT_ANTIALIASING_GRAY  # 灰度抗锯齿（通用）
FONT_ANTIALIASING_LCD   # LCD亚像素（最清晰）
```

### 亚像素定位
```gdscript
SUBPIXEL_POSITIONING_DISABLED      # 禁用
SUBPIXEL_POSITIONING_AUTO          # 自动（推荐）
SUBPIXEL_POSITIONING_ONE_HALF      # 1/2像素精度
SUBPIXEL_POSITIONING_ONE_QUARTER   # 1/4像素精度（最精确）
```

### Hinting
```gdscript
HINTING_NONE    # 无微调（大字号）
HINTING_LIGHT   # 轻度微调（推荐）
HINTING_NORMAL  # 标准微调（小字号）
```

---

## 📍 关键代码位置

### 核心渲染函数
```
modules/text_server_adv/text_server_adv.cpp:3931
→ TextServerAdvanced::_font_draw_glyph()
  最重要的函数！每个字形的实际渲染

modules/text_server_adv/text_server_adv.cpp:2500
→ TextServerAdvanced::_ensure_glyph()
  字形缓存和光栅化

modules/text_server_adv/text_server_adv.cpp:5000
→ TextServerAdvanced::_shaped_text_shape()
  文本排版和字形选择
```

### 高层API
```
scene/resources/font.cpp:349
→ Font::draw_string()
  字符串绘制入口

scene/resources/text_line.cpp:408
→ TextLine::draw()
  单行文本绘制

scene/gui/label.cpp:700
→ Label::_notification(NOTIFICATION_DRAW)
  Label控件绘制
```

### 服务层
```
servers/text/text_server.cpp:1727
→ TextServer::shaped_text_draw()
  遍历字形并调用font_draw_glyph

servers/text/text_server.h:430
→ TextServer::font_draw_glyph()
  虚函数接口定义
```

---

## 🔧 常用API

### 字体配置
```gdscript
var font = FontFile.new()
font.load_dynamic_font("res://fonts/MyFont.ttf")

# 抗锯齿
font.antialiasing = TextServer.FONT_ANTIALIASING_LCD

# 亚像素定位
font.subpixel_positioning = TextServer.SUBPIXEL_POSITIONING_ONE_QUARTER

# Hinting
font.hinting = TextServer.HINTING_LIGHT

# Oversampling
font.oversampling = 1.5

# MSDF
font.multichannel_signed_distance_field = true
font.msdf_pixel_range = 16
font.msdf_size = 48

# Mipmap
font.generate_mipmaps = true
```

### 文本绘制
```gdscript
# 方法1：使用Label
var label = Label.new()
label.text = "Hello World"
label.add_theme_font_override("font", font)
label.add_theme_font_size_override("font_size", 16)

# 方法2：使用Font.draw_string
font.draw_string(
    get_canvas_item(),           # canvas
    Vector2(100, 100),           # position
    "Hello World",               # text
    HORIZONTAL_ALIGNMENT_LEFT,   # alignment
    -1,                          # width
    16,                          # font_size
    Color.WHITE                  # color
)

# 方法3：使用TextLine
var line = TextLine.new()
line.add_string("Hello World", font, 16)
line.draw(get_canvas_item(), Vector2(100, 100), Color.WHITE)
```

### 字体度量
```gdscript
var height = font.get_height(16)
var ascent = font.get_ascent(16)
var descent = font.get_descent(16)
var string_size = font.get_string_size("Hello", HORIZONTAL_ALIGNMENT_LEFT, -1, 16)
```

---

## 🐛 调试技巧

### 断点位置
```
最常用的3个断点：
1. modules/text_server_adv/text_server_adv.cpp:3931
   → _font_draw_glyph (看每个字形如何渲染)

2. scene/resources/font.cpp:349
   → Font::draw_string (看字符串绘制入口)

3. scene/gui/label.cpp:700
   → Label::_notification (看Label如何触发绘制)
```

### 性能分析
```gdscript
# 检查缓存命中率
var ts = TextServerManager.get_singleton().get_primary_interface()
print("Cache stats: ", ts.get_stats())

# 检查纹理数量
print("Textures: ", font.get_texture_count(Vector2i(16, 0)))

# 测量渲染时间
var start = Time.get_ticks_usec()
font.draw_string(...)
var elapsed = Time.get_ticks_usec() - start
print("Render time: ", elapsed, " μs")
```

### 常见问题诊断
```gdscript
# 文本模糊？
print("Oversampling: ", font.oversampling)  # 应该 >= 1.0
print("Subpixel: ", font.subpixel_positioning)  # 不应该是DISABLED
print("Antialiasing: ", font.antialiasing)  # 不应该是NONE

# 性能差？
print("Oversampling: ", font.oversampling)  # 不应该 > 2.0
print("MSDF: ", font.multichannel_signed_distance_field)  # 大字号应该用MSDF

# 彩色边缘？
if font.antialiasing == TextServer.FONT_ANTIALIASING_LCD:
    print("LCD on dark background causes color fringing")
    font.antialiasing = TextServer.FONT_ANTIALIASING_GRAY
```

---

## 📊 性能优化清单

### ✅ 应该做的
- 使用字形缓存（自动）
- 小字号用LCD抗锯齿
- 大字号用MSDF
- 合理设置oversampling（1.0-2.0）
- 启用mipmap（动态缩放时）

### ❌ 不应该做的
- 频繁改变字号
- 过高的oversampling（>2.0）
- 在深色背景上用LCD
- 禁用亚像素定位
- 对MSDF字体使用oversampling

---

## 🎨 最佳实践

### 小字号（≤16px）
```gdscript
font.antialiasing = TextServer.FONT_ANTIALIASING_LCD
font.subpixel_positioning = TextServer.SUBPIXEL_POSITIONING_ONE_QUARTER
font.hinting = TextServer.HINTING_NORMAL
font.oversampling = 1.0
```

### 中字号（16-32px）
```gdscript
font.antialiasing = TextServer.FONT_ANTIALIASING_GRAY
font.subpixel_positioning = TextServer.SUBPIXEL_POSITIONING_ONE_HALF
font.hinting = TextServer.HINTING_LIGHT
font.oversampling = 1.5
```

### 大字号（>32px）
```gdscript
# 方案1：MSDF
font.multichannel_signed_distance_field = true
font.msdf_pixel_range = 16
font.msdf_size = 48

# 方案2：高倍过采样
font.antialiasing = TextServer.FONT_ANTIALIASING_GRAY
font.subpixel_positioning = TextServer.SUBPIXEL_POSITIONING_DISABLED
font.hinting = TextServer.HINTING_NONE
font.oversampling = 2.0
```

---

## 📚 延伸阅读

- [01-文本渲染基础概念](./01-文本渲染基础概念.md)
- [02-高精度渲染技术](./02-高精度渲染技术.md)
- [09-字形渲染核心代码](./09-字形渲染核心代码.md) ⭐
- [12-调试文本渲染](./12-调试文本渲染.md)
- [14-性能优化实战](./14-性能优化实战.md)

---

**提示**：将此页面加入书签，随时查阅！
