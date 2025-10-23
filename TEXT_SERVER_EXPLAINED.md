# Godot文本服务器详解

## 1️⃣ 类注册机制

### GDREGISTER_CLASS 宏
虽然代码中使用了 `GDREGISTER_CLASS`，但这实际上是 `GDCLASS` 宏的一部分：

```cpp
// modules/text_server_adv/text_server_adv.h
class TextServerAdvanced : public TextServerExtension {
    GDCLASS(TextServerAdvanced, TextServerExtension);  // 👈 这里声明类
    // ...
};

// modules/text_server_adv/register_types.cpp
void initialize_text_server_adv_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SERVERS) {
        return;
    }

    GDREGISTER_CLASS(TextServerAdvanced);  // 👈 这里注册到ClassDB
    
    // 创建实例并添加到TextServerManager
    TextServerManager *tsman = TextServerManager::get_singleton();
    if (tsman) {
        Ref<TextServerAdvanced> ts;
        ts.instantiate();
        tsman->add_interface(ts);  // 👈 注册为可用的文本服务器
    }
}
```

### 注册流程

```
启动时
  ↓
main/main.cpp:2881
  tsman = memnew(TextServerManager);  // 创建管理器
  ↓
initialize_modules(MODULE_INITIALIZATION_LEVEL_SERVERS)
  ↓
各模块的 initialize_xxx_module() 被调用
  ├─ initialize_text_server_fb_module()
  │    └─ tsman->add_interface(TextServerFallback实例)
  └─ initialize_text_server_adv_module()
       └─ tsman->add_interface(TextServerAdvanced实例)
  ↓
main/main.cpp:3345
  选择功能最多的文本服务器作为主服务器
  TextServerManager::get_singleton()->set_primary_interface(ts);
```

---

## 2️⃣ Android平台使用哪个文本服务器？

### 默认选择逻辑（main/main.cpp:3345-3358）

```cpp
// 如果用户没有指定，使用功能最多的
int max_features = 0;
for (int i = 0; i < TextServerManager::get_singleton()->get_interface_count(); i++) {
    uint32_t features = TextServerManager::get_singleton()->get_interface(i)->get_features();
    
    int feature_number = 0;
    while (features) {
        feature_number += features & 1;
        features = features >> 1;
    }
    if (feature_number >= max_features) {
        max_features = feature_number;
        text_driver_idx = i;
    }
}
```

### Android上的实际情况

**默认：TextServerAdvanced** ✅

原因：
1. Android编译时**两个模块都会被编译**（除非手动禁用）
2. TextServerAdvanced的功能数量更多：
   ```cpp
   // text_server_adv 支持的功能
   FEATURE_SIMPLE_LAYOUT
   FEATURE_BIDI_LAYOUT          // ✅ 双向文本
   FEATURE_VERTICAL_LAYOUT      // ✅ 垂直排版
   FEATURE_SHAPING              // ✅ 复杂脚本
   FEATURE_KASHIDA_JUSTIFICATION // ✅ 阿拉伯语对齐
   FEATURE_BREAK_ITERATORS      // ✅ 智能断行
   FEATURE_FONT_BITMAP
   FEATURE_FONT_DYNAMIC
   FEATURE_FONT_MSDF            // ✅ MSDF支持
   // ... 更多功能
   
   // text_server_fb 支持的功能
   FEATURE_SIMPLE_LAYOUT
   FEATURE_FONT_BITMAP
   FEATURE_FONT_DYNAMIC
   FEATURE_FONT_MSDF (如果编译了msdfgen模块)
   // 仅基础功能
   ```

3. 自动选择功能最多的 → **TextServerAdvanced**

### 手动指定（可选）

在 `project.godot` 中：
```ini
[internationalization]
rendering/text_driver = "TextServerFallback"  # 强制使用FB版本
```

或在代码中：
```gdscript
# 在游戏启动时
ProjectSettings.set_setting("internationalization/rendering/text_driver", "TextServerFallback")
```

---

## 3️⃣ TextServerFallback的渲染质量

### 功能对比表

| 功能 | TextServerFallback | TextServerAdvanced |
|------|-------------------|-------------------|
| **基础渲染** | ✅ | ✅ |
| FreeType字体 | ✅ | ✅ |
| 抗锯齿 | ✅ Gray/LCD | ✅ Gray/LCD |
| 亚像素定位 | ✅ | ✅ |
| Oversampling | ✅ | ✅ |
| MSDF | ✅ (需msdfgen) | ✅ |
| Hinting | ✅ | ✅ |
| Mipmap | ✅ | ✅ |
| **高级功能** | ❌ | ✅ |
| BiDi（双向文本） | ❌ | ✅ HarfBuzz+ICU |
| 复杂脚本（阿拉伯语、印地语等） | ❌ | ✅ HarfBuzz |
| 连字（Ligatures） | ❌ | ✅ |
| 字距调整（Kerning） | ✅ 基础 | ✅ 高级 |
| OpenType特性 | ❌ | ✅ |
| 垂直排版 | ❌ | ✅ |

### 渲染质量评估

#### ✅ **对于英文/简单文本：几乎相同**

```cpp
// text_server_fb/text_server_fb.cpp
// FB版本使用相同的FreeType渲染管线
void TextServerFallback::_font_draw_glyph(...) {
    // 1. 相同的oversampling
    // 2. 相同的亚像素定位
    // 3. 相同的LCD抗锯齿
    // 4. 相同的MSDF支持
    
    if (fd->msdf) {
        RenderingServer::get_singleton()->canvas_item_add_msdf_texture_rect_region(...);
    } else if (lcd_aa) {
        RenderingServer::get_singleton()->canvas_item_add_lcd_texture_rect_region(...);
    } else {
        RenderingServer::get_singleton()->canvas_item_add_texture_rect_region(...);
    }
}
```

**结论**：对于英文、数字、简单符号，**视觉效果完全一样**！

#### ⚠️ **对于复杂文本：有明显差异**

1. **阿拉伯语/希伯来语**
   - FB: 字符独立显示，不连接 ❌
   - Adv: 正确的连字和形状变化 ✅

2. **印地语/泰语等**
   - FB: 字符堆叠错误 ❌
   - Adv: 正确的复杂脚本排版 ✅

3. **连字（如 fi → ﬁ）**
   - FB: 分开显示 f i ❌
   - Adv: 显示连字 ﬁ ✅

4. **Emoji/彩色字体**
   - FB: 基础支持 ⚠️
   - Adv: 完整支持 ✅

### 性能对比

| 指标 | TextServerFallback | TextServerAdvanced |
|------|-------------------|-------------------|
| 二进制大小 | ~500KB | ~3MB (含HarfBuzz+ICU) |
| 内存占用 | 低 | 中等 |
| 排版速度 | 快 | 稍慢（复杂脚本） |
| 启动时间 | 快 | 稍慢 |

### 实际测试示例

```gdscript
# 测试代码
extends Label

func _ready():
    # 英文测试
    text = "Hello World 123"
    # FB和Adv: 完全相同 ✅
    
    # 连字测试
    text = "office difficult"
    # FB: office difficult (分开)
    # Adv: oﬃce diﬃcult (连字) ✅
    
    # 阿拉伯语测试
    text = "مرحبا بك"
    # FB: 字符反向且不连接 ❌
    # Adv: 正确的从右到左连写 ✅
    
    # Emoji测试
    text = "Hello 👋 World 🌍"
    # FB: 基础显示 ⚠️
    # Adv: 完整彩色显示 ✅
```

---

## 🎯 使用建议

### 使用 TextServerFallback 的场景

1. **纯英文游戏**（无需国际化）
2. **嵌入式设备**（内存/存储受限）
3. **简单UI**（只有数字和基本符号）
4. **追求最小体积**

### 使用 TextServerAdvanced 的场景

1. **多语言游戏**（特别是亚洲/中东语言）
2. **需要高质量排版**（连字、字距等）
3. **复杂文本编辑器**
4. **Android/iOS发布**（默认选择）

### Android开发建议

**推荐：保持默认（TextServerAdvanced）**

原因：
- Android设备性能足够
- 用户可能切换系统语言
- Google Play要求支持多语言
- 体积增加（~2.5MB）可接受

如果确实需要减小APK体积：
```python
# platform/android/detect.py 或 custom.py
env.module_add_dependencies("text_server_adv", [], False)  # 禁用Adv
```

---

## 📊 总结

| 方面 | 结论 |
|------|------|
| **注册机制** | 模块化，通过TextServerManager管理 |
| **Android默认** | TextServerAdvanced（功能最多） |
| **FB渲染质量** | 英文/简单文本：与Adv相同 ✅<br>复杂文本：明显不足 ❌ |
| **推荐选择** | 除非有特殊需求，使用默认的Adv |

**关键点**：TextServerFallback的"Fallback"不是指"渲染质量降级"，而是指"功能简化"。对于基础的英文文本，它的渲染质量和Advanced版本**完全一样**，因为它们使用相同的FreeType渲染管线和相同的清晰度技术（亚像素定位、LCD抗锯齿、MSDF等）。
