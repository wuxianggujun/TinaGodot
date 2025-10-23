# 文档快速索引

> 快速找到你需要的文档

## 🚀 快速开始

| 我想... | 推荐文档 | 时间 |
|---------|---------|------|
| 快速上手文本渲染 | [学习路径A](./text_rendering/LEARNING_PATH.md#路径a应用开发者快速入门) | 3小时 |
| 深入理解渲染原理 | [学习路径B](./text_rendering/LEARNING_PATH.md#路径b引擎开发者深入理解) | 7小时 |
| 完整掌握文本系统 | [学习路径C](./text_rendering/LEARNING_PATH.md#路径c系统学习完整掌握) | 12小时 |
| 查找API和代码位置 | [快速参考](./text_rendering/QUICK_REFERENCE.md) | 随时 |
| 解决具体问题 | [常见问题](#常见问题快速跳转) | 5分钟 |

---

## 📚 文档导航

### 总览文档
- [文档中心首页](./README.md)
- [文本渲染学习指南](./text_rendering/README.md)
- [学习路线图](./text_rendering/LEARNING_PATH.md) ⭐
- [快速参考卡片](./text_rendering/QUICK_REFERENCE.md) ⭐

### 基础教程
- [01-文本渲染基础概念](./text_rendering/01-文本渲染基础概念.md)
- [02-高精度渲染技术](./text_rendering/02-高精度渲染技术.md)
- 03-性能优化技术（待完成）

### 架构文档
- 04-Godot文本渲染架构总览（待完成）
- 05-TextServer接口层（待完成）
- 06-Font资源层（待完成）
- 07-UI节点层（待完成）

### 底层实现
- 08-TextServerAdvanced实现解析（待完成）
- [09-字形渲染核心代码](./text_rendering/09-字形渲染核心代码-参考.md) ⭐ 最重要
- 10-字形缓存与纹理管理（待完成）
- 11-文本排版引擎（待完成）

### 实战应用
- 12-调试文本渲染（待完成）
- 13-自定义文本渲染器（待完成）
- 14-性能优化实战（待完成）
- 15-跨平台适配（待完成）

### 参考资料
- 16-API参考手册（待完成）
- 17-源码导航（待完成）
- 18-常见问题FAQ（待完成）

---

## 🔍 按主题查找

### 概念理解
- [什么是字形？](./text_rendering/01-文本渲染基础概念.md#2-字符-vs-字形)
- [什么是光栅化？](./text_rendering/01-文本渲染基础概念.md#3-光栅化rasterization)
- [什么是抗锯齿？](./text_rendering/01-文本渲染基础概念.md#4-抗锯齿技术)

### 技术细节
- [亚像素定位原理](./text_rendering/02-高精度渲染技术.md#1-亚像素定位subpixel-positioning)
- [LCD抗锯齿原理](./text_rendering/02-高精度渲染技术.md#2-lcd亚像素抗锯齿)
- [MSDF技术](./text_rendering/02-高精度渲染技术.md#3-msdf多通道有向距离场)
- [Hinting原理](./text_rendering/02-高精度渲染技术.md#4-hinting字体微调)
- [Oversampling](./text_rendering/02-高精度渲染技术.md#5-oversampling过采样)

### 代码实现
- [核心渲染函数](./text_rendering/QUICK_REFERENCE.md#-关键代码位置)
- [字形缓存](./text_rendering/09-字形渲染核心代码-参考.md)
- [纹理管理](./text_rendering/09-字形渲染核心代码-参考.md)

### API使用
- [字体配置](./text_rendering/QUICK_REFERENCE.md#字体配置)
- [文本绘制](./text_rendering/QUICK_REFERENCE.md#文本绘制)
- [字体度量](./text_rendering/QUICK_REFERENCE.md#字体度量)

### 调试技巧
- [断点位置](./text_rendering/QUICK_REFERENCE.md#断点位置)
- [性能分析](./text_rendering/QUICK_REFERENCE.md#性能分析)
- [问题诊断](./text_rendering/QUICK_REFERENCE.md#常见问题诊断)

### 最佳实践
- [小字号配置](./text_rendering/QUICK_REFERENCE.md#小字号16px)
- [中字号配置](./text_rendering/QUICK_REFERENCE.md#中字号16-32px)
- [大字号配置](./text_rendering/QUICK_REFERENCE.md#大字号32px)
- [性能优化](./text_rendering/QUICK_REFERENCE.md#-性能优化清单)

---

## 🎯 按角色查找

### 游戏开发者
**推荐阅读**：
1. [01-文本渲染基础概念](./text_rendering/01-文本渲染基础概念.md)
2. [02-高精度渲染技术](./text_rendering/02-高精度渲染技术.md)
3. [快速参考](./text_rendering/QUICK_REFERENCE.md)

**关注重点**：
- 如何配置高质量字体
- 如何解决文本模糊
- 如何实现文本特效

### UI设计师
**推荐阅读**：
1. [01-文本渲染基础概念](./text_rendering/01-文本渲染基础概念.md)
2. [02-高精度渲染技术](./text_rendering/02-高精度渲染技术.md)（前半部分）
3. [快速参考-最佳实践](./text_rendering/QUICK_REFERENCE.md#-最佳实践)

**关注重点**：
- 抗锯齿模式选择
- 字号和清晰度关系
- 不同场景的配置

### 引擎开发者
**推荐阅读**：
1. 全部基础教程（01-03）
2. 全部架构文档（04-07）
3. 全部底层实现（08-11）⭐
4. [快速参考](./text_rendering/QUICK_REFERENCE.md)

**关注重点**：
- 渲染流程和调用链
- 核心算法实现
- 性能优化技巧
- 源码结构

### 性能优化工程师
**推荐阅读**：
1. [02-高精度渲染技术](./text_rendering/02-高精度渲染技术.md)
2. 03-性能优化技术（待完成）
3. [09-字形渲染核心代码](./text_rendering/09-字形渲染核心代码-参考.md)
4. 14-性能优化实战（待完成）

**关注重点**：
- 缓存策略
- 批量渲染
- 内存优化
- GPU优化

---

## ❓ 常见问题快速跳转

### 文本显示问题
- [文本模糊怎么办？](./text_rendering/QUICK_REFERENCE.md#常见问题诊断)
- [文本有锯齿？](./text_rendering/01-文本渲染基础概念.md#4-抗锯齿技术)
- [文本有彩色边缘？](./text_rendering/QUICK_REFERENCE.md#常见问题诊断)
- [文本间距不均？](./text_rendering/02-高精度渲染技术.md#1-亚像素定位subpixel-positioning)

### 性能问题
- [渲染很慢？](./text_rendering/QUICK_REFERENCE.md#常见问题诊断)
- [内存占用高？](./text_rendering/QUICK_REFERENCE.md#-性能优化清单)
- [缓存命中率低？](./text_rendering/01-文本渲染基础概念.md#6-字形缓存)

### 配置问题
- [如何启用LCD抗锯齿？](./text_rendering/QUICK_REFERENCE.md#字体配置)
- [如何使用MSDF？](./text_rendering/QUICK_REFERENCE.md#字体配置)
- [如何设置Oversampling？](./text_rendering/QUICK_REFERENCE.md#字体配置)

### 开发问题
- [如何调试文本渲染？](./text_rendering/QUICK_REFERENCE.md#断点位置)
- [如何分析性能？](./text_rendering/QUICK_REFERENCE.md#性能分析)
- [如何实现自定义渲染器？](./text_rendering/README.md#第四部分实战应用进阶)

---

## 🛠️ 工具和资源

### 示例代码
- `examples/text_rendering/` - 文本渲染示例
- `examples/custom_renderer/` - 自定义渲染器

### 工具脚本
- `tools/font_analyzer.gd` - 字体分析
- `tools/render_debugger.gd` - 渲染调试
- `tools/performance_profiler.gd` - 性能分析

### 参考实现
- `reference_impl/minimal_text_server.cpp` - 最小TextServer
- `reference_impl/simple_text_renderer.cpp` - 简化渲染器

---

## 📖 推荐学习顺序

### 新手（第一次学习）
```
1. 阅读学习路线图
   → docs/text_rendering/LEARNING_PATH.md

2. 选择路径A（快速入门）
   → 01 → 02 → 04 → 06 → 07 → 12

3. 收藏快速参考
   → docs/text_rendering/QUICK_REFERENCE.md
```

### 进阶（想深入理解）
```
1. 完成新手路径

2. 选择路径B（深入理解）
   → 03 → 05 → 08 → 09 ⭐ → 10 → 11

3. 实践项目
   → 实现自定义功能
```

### 高级（想完整掌握）
```
1. 完成进阶路径

2. 选择路径C（完整掌握）
   → 13 → 14 → 15 → 16 → 17 → 18

3. 贡献代码
   → 成为Godot贡献者
```

---

## 🔖 书签推荐

建议将以下页面加入浏览器书签：

1. **[学习路线图](./text_rendering/LEARNING_PATH.md)** - 规划学习
2. **[快速参考](./text_rendering/QUICK_REFERENCE.md)** - 随时查阅
3. **[本索引页](./INDEX.md)** - 快速导航

---

## 📊 学习进度

你可以在这里追踪学习进度：

- [ ] 已阅读学习路线图
- [ ] 已选择学习路径
- [ ] 已完成基础教程（01-03）
- [ ] 已完成架构文档（04-07）
- [ ] 已完成底层实现（08-11）
- [ ] 已完成实战应用（12-15）
- [ ] 已完成参考资料（16-18）

---

## 💡 学习提示

1. **不要跳过基础** - 即使你有经验
2. **边学边实践** - 每章都有练习
3. **使用快速参考** - 随时查阅API
4. **提出问题** - 深入思考
5. **分享经验** - 帮助他人

---

**开始学习** → [文本渲染学习指南](./text_rendering/README.md)

**快速查阅** → [快速参考卡片](./text_rendering/QUICK_REFERENCE.md)

**选择路径** → [学习路线图](./text_rendering/LEARNING_PATH.md)
