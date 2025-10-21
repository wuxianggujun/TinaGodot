# 依赖清理报告（TinaGodot）

> **最后更新**: 2025-10-21
> **状态**: 第四阶段完成

本次操作依据项目定位（UI/2D 精简版）、SCons 默认禁用项与文档模块矩阵，对"默认关闭/明确不需要"的功能模块及对应 thirdparty 依赖进行了物理删除，以缩减源码体积并避免误启用。

⚠️ **注意**：已删除模块将无法通过构建开关再次启用；如需恢复，请从上游或历史提交还原后，重新运行生成脚本（如 `gen_vsproj.*`）与 SCons 构建。

---

## 📋 第一阶段：3D功能删除 ✅（已完成）

### 删除的第三方库（~200MB）
- thirdparty/jolt_physics/ (3D物理引擎)
- thirdparty/openxr/ (XR支持)
- thirdparty/vhacd/ (凸包分解)
- thirdparty/embree/ (光线追踪)

### 删除的模块
- modules/fbx/ (FBX导入)
- modules/vhacd/ (凸包分解模块)

**体积减少**: ~258MB

---

## 📋 第二阶段：网络/导航模块删除 ✅（已完成）

### 已删除的模块（modules/）

**网络相关**（5个模块）：
- ✅ enet - ENet网络库
- ✅ multiplayer - 多人游戏框架
- ✅ websocket - WebSocket支持
- ✅ upnp - UPnP端口映射
- ✅ mbedtls - TLS/SSL加密

**导航相关**（1个模块）：
- ✅ navigation_2d - 2D寻路和避障

**3D工具**（2个模块）：
- ✅ meshoptimizer - 网格优化
- ✅ xatlas_unwrap - UV展开

**其他**（2个模块）：
- ✅ noise - 噪声生成（包含NoiseTexture3D）
- ✅ jsonrpc - JSON远程调用

**已完成**: 10个模块，约7.7MB

### 已清理的第三方依赖（thirdparty/）✅

**网络相关**（3个依赖）：
- ✅ enet - ENet网络库实现 (0.2MB) - 已删除
- ✅ miniupnpc - UPnP端口映射库 (0.3MB) - 已删除
- ✅ wslay - WebSocket实现 (0.1MB) - 已删除

**注意**：`thirdparty/mbedtls/` 核心加密库**必须保留** (7.7MB)
- `modules/mbedtls/` 模块已删除（TLS/SSL网络加密）
- `thirdparty/mbedtls/` 保留（核心加密：AES、MD5、SHA256等）
- 原因：`core/crypto/` 依赖这些基础加密功能进行资源hash、数据加密等

**导航相关**（2个依赖）：
- ✅ recastnavigation - 寻路网格 (0.7MB) - 已删除
- ✅ rvo2 - 动态避障算法 (0.8MB) - 已删除

**3D工具**（2个依赖）：
- ✅ meshoptimizer - 网格优化库 (0.5MB) - 已删除
- ✅ xatlas - UV展开库 (0.3MB) - 已删除

**额外清理**：
- ✅ amd-fsr/amd-fsr2 - AMD FSR缩放技术 - 已删除
- ✅ certs - CA证书包 - 已删除
- ✅ doctest - 测试框架 - 已删除

**已清理体积**: ~10.8MB

**提交信息**：
```bash
git commit b3899885e9
chore(purge-thirdparty): 移除mbedtls加密库和其他网络依赖
1139 files changed, 602 insertions(+), 600708 deletions(-)
```

---

## 📋 第三阶段：音频系统完全删除 ✅（已完成 - 2025-10-20）

**删除的音频代码**（123个文件）：

**Scene层**：
- scene/audio/ - 音频播放器场景节点（6个文件）
- scene/2d/audio_*.* - 2D音频监听器和播放器（5个文件）

**Servers层**：
- servers/audio/ - 音频服务器核心（59个文件）
  - 音频驱动、重采样、音频流
  - 所有音效（Amplify、Chorus、Compressor、Delay、Distortion、EQ、Filter、Limiter、Panner、Phaser、PitchShift、Record、Reverb、SpectrumAnalyzer等）

**Editor层**：
- editor/audio/ - 音频编辑器插件（9个文件）
- editor/import/audio_stream_import_settings.* - 音频导入设置（2个文件）

**文档**：
- doc/classes/Audio*.xml - 所有音频相关API文档（42个文件）

**已删除代码**: 约19,194行

**相关提交**：
- `8ce31bdcae` - 完全删除音频系统
  - 123个文件
  - 19,194行代码

**说明**：TinaGodot作为纯UI框架，不需要音频功能。用户可通过操作系统或外部媒体库处理音频需求。

---

## 📋 第四阶段：FSR删除，SMAA保留 ✅（已完成 - 2025-10-21）

**删除的渲染特性**：

### FSR (FidelityFX Super Resolution) 缩放技术 ❌ 已删除
**删除的代码文件**：
- `servers/rendering/renderer_rd/effects/fsr.h` - FSR1效果实现头文件
- `servers/rendering/renderer_rd/effects/fsr.cpp` - FSR1效果实现
- `servers/rendering/renderer_rd/effects/fsr2.h` - FSR2效果实现头文件
- `servers/rendering/renderer_rd/effects/fsr2.cpp` - FSR2效果实现（约1,200行）
- `servers/rendering/renderer_rd/shaders/effects/fsr2/*` - 所有FSR2 shader文件

**修改的文件**：
- `servers/rendering/renderer_rd/renderer_scene_render_rd.h/.cpp` - 移除FSR初始化和使用代码
- `servers/rendering/renderer_rd/forward_clustered/render_forward_clustered.h/.cpp` - 移除FSR2上下文和处理代码
- `servers/rendering/renderer_rd/effects/SCsub` - 注释掉amd-fsr2的构建配置

**删除的第三方依赖**：
- ❌ `thirdparty/amd-fsr/` - AMD FSR 1.0库（已在第二阶段删除）
- ❌ `thirdparty/amd-fsr2/` - AMD FSR 2.0库（已在第二阶段删除）

**已删除代码**: 约1,500行

**删除原因**：
- FSR和FSR2是AMD的3D场景缩放技术，主要用于提升3D游戏的渲染性能
- 它们通过在较低分辨率渲染后放大到目标分辨率来提升性能
- **对2D UI框架无用**：2D UI直接渲染到目标分辨率，不需要缩放技术

---

### SMAA (Subpixel Morphological Anti-Aliasing) 抗锯齿 ✅ 保留
**保留的代码文件**：
- ✅ `servers/rendering/renderer_rd/effects/smaa.h` - SMAA效果实现头文件（约270行）
- ✅ `servers/rendering/renderer_rd/effects/smaa.cpp` - SMAA效果实现
- ✅ `servers/rendering/renderer_rd/shaders/effects/smaa_*.glsl` - SMAA shader实现

**保留的第三方资源**：
- ✅ `thirdparty/smaa/AreaTex.png` - 边缘区域纹理
- ✅ `thirdparty/smaa/SearchTex.png` - 边缘搜索纹理
- ✅ `thirdparty/smaa/LICENSE.txt` - SMAA许可证

**构建配置**：
- ✅ `servers/rendering/renderer_rd/effects/SCsub` - SMAA纹理生成代码已恢复

**保留原因**：
- SMAA是屏幕空间抗锯齿技术，适用于**2D和3D场景**
- **对2D UI框架有实际价值**：
  - 改善位图UI元素在非整数缩放时的锯齿
  - 优化字体渲染（尤其是小字号和非标准DPI）
  - 平滑Canvas绘制的线条和形状边缘
  - 提升混合2D内容的视觉质量
- 性能开销小，比MSAA轻量，比FXAA清晰
- **用户可选**：通过`ViewportScreenSpaceAA.SMAA`启用

**API支持**：
- `RS::VIEWPORT_SCREEN_SPACE_AA_SMAA` - 枚举值
- `viewport_set_screen_space_aa(RID, ViewportScreenSpaceAA)` - API接口

**相关提交**: 待提交
- 删除FSR1/FSR2相关代码（约1,500行）
- **保留SMAA抗锯齿**作为2D UI的可选渲染优化

---

## 📋 可选模块（计划宏化控制）

以下模块计划通过编译宏控制，暂未删除：

### 扩展图像格式（计划通过 `TINAGODOT_ENABLE_ADVANCED_IMAGE` 控制）
**模块**：
- astcenc - ASTC纹理压缩
- basis_universal - 通用纹理压缩
- bcdec - BC纹理解码
- betsy - 纹理压缩
- cvtt - CVTT纹理压缩
- dds - DDS格式
- etcpak - ETC纹理压缩
- ktx - KTX格式
- hdr - HDR图像格式
- tinyexr - OpenEXR格式

**第三方依赖**：
- thirdparty/astcenc
- thirdparty/basis_universal
- thirdparty/cvtt
- thirdparty/etcpak
- thirdparty/libktx
- thirdparty/tinyexr

**体积**: ~37MB（模块5MB + 依赖32MB）

### 性能分析工具（计划通过 `TINAGODOT_ENABLE_PROFILER` 控制）
**模块**：
- objectdb_profiler - 对象数据库性能分析

**体积**: ~0.5MB

---

## 📊 体积变化总结

| 清理阶段 | 删除内容 | 体积减少 | 状态 |
|---------|---------|---------|------|
| 第一阶段 | 3D功能和相关库 | ~258MB | ✅ 完成 |
| 第二阶段-模块 | 网络/导航/工具模块 | ~7.7MB | ✅ 完成 |
| 第二阶段-依赖 | 对应第三方库 | ~10.8MB | ✅ 完成 |
| 第三阶段-音频 | 音频系统完全删除 | ~600KB（代码） | ✅ 完成 |
| 第四阶段-渲染特性 | FSR/FSR2删除，SMAA保留 | ~1,500行代码 | ✅ 完成 |
| **已完成总计** | | **~277MB + 20,694行代码** | ✅ |
| 可选模块（待宏化） | 扩展图像格式 | ~37MB | 📋 计划中 |

**最新提交**：待提交 (2025-10-21)
- 删除FSR1/FSR2相关代码（约1,500行）
- **保留SMAA抗锯齿**（适用于2D UI渲染）
- 恢复`thirdparty/smaa/`纹理资源
- 清理FSR相关构建配置

**当前状态**：
- thirdparty/ 目录: ~118MB（包含mbedtls核心加密库 + SMAA纹理）
- modules/ 目录: 2.2MB（保留核心模块，mbedtls模块已删除）
- scene/ 目录: 无音频代码、无3D代码
- servers/ 目录: 无音频服务器、无FSR缩放技术、**保留SMAA抗锯齿**

---

## ⚠️ 保留的核心依赖（必需，不可删除）

### 文本渲染系统（~47MB）
- thirdparty/icu4c/ - 国际化和Unicode支持
- thirdparty/freetype/ - 字体渲染引擎
- thirdparty/harfbuzz/ - 文本整形引擎
- thirdparty/graphite/ - 复杂脚本支持
- thirdparty/msdfgen/ - MSDF字体生成
- thirdparty/thorvg/ - SVG渲染
- thirdparty/fonts/ - 内置字体文件

### 图形渲染系统（~28MB）
- thirdparty/vulkan/ - Vulkan图形API
- thirdparty/glslang/ - GLSL着色器编译器
- thirdparty/spirv-cross/ - SPIR-V转换
- thirdparty/spirv-reflect/ - SPIR-V反射
- thirdparty/glad/ - OpenGL加载器
- thirdparty/angle/ - OpenGL ES支持
- thirdparty/volk/ - Vulkan加载器

### 基础图像格式（~7.6MB）
- thirdparty/libpng/ - PNG图像格式
- thirdparty/libjpeg-turbo/ - JPEG图像格式
- thirdparty/libwebp/ - WebP图像格式（可选删除）

### 核心系统库（~23MB）
- thirdparty/zlib/ - ZIP压缩
- thirdparty/minizip/ - ZIP文件处理
- thirdparty/zstd/ - 高效压缩算法
- thirdparty/brotli/ - Brotli压缩
- thirdparty/clipper2/ - 多边形布尔运算
- thirdparty/pcre2/ - 正则表达式库
- thirdparty/mbedtls/ - 核心加密库（AES、MD5、SHA256）⚠️ 必需
- thirdparty/misc/ - 杂项工具

### 平台支持库（~10MB）
- thirdparty/sdl/ - 跨平台窗口/输入
- thirdparty/directx_headers/ - Windows DirectX
- thirdparty/d3d12ma/ - DirectX 12内存分配
- thirdparty/linuxbsd_headers/ - Linux头文件
- thirdparty/wayland/ - Linux Wayland支持
- thirdparty/wayland-protocols/ - Wayland协议
- thirdparty/mingw-std-threads/ - MinGW线程支持

**核心依赖总计**: ~118MB（必需保留，包含mbedtls核心加密）

---

## 📝 下一步行动

1. ✅ **已完成**: 物理删除网络/导航模块代码
2. ✅ **已完成**: 删除音频系统（第三阶段）
3. ✅ **已完成**: 删除FSR/FSR2/SMAA渲染特性（第四阶段）
4. 📋 **计划中**: 实现扩展图像格式宏控制框架
5. 📋 **待评估**: 是否删除libwebp（WebP格式）
6. 📋 **待评估**: 继续清理其他3D专用渲染特性

---

**报告生成时间**: 2025-10-21
**维护者**: TinaGodot Team

---

## 变更记录（近期）

- 2025-10-21：**编辑器运行时错误修复** - 修复3D编辑器删除后的遗留问题
  - ✅ 修复 `spatial_editor/tool_select` 快捷键错误（替换为canvas_item_editor快捷键）
  - ✅ 修复 `EDITOR_3D` 按钮索引越界错误（删除相关调用）
  - ✅ 修复 `ScriptServer::get_language(-1)` 索引越界错误（添加边界检查）
  - 提交：`f3fb3e8559` - Fix runtime errors after 3D editor removal

- 2025-10-21：**音频动画轨道编辑器修复** - 解决AudioStream依赖问题
  - ✅ 删除已移除的 `AudioStream` 头文件引用
  - ✅ 将 `Ref<AudioStream>` 替换为 `Ref<Resource>` 保持兼容性
  - ✅ 使用固定默认值（3600秒）替代 `AudioStream::get_length()` 调用
  - 说明：Audio Track功能在UI中保留但不可用（音频系统已完全移除）
  - 提交：待推送 - Fix audio dependencies in animation track editor

- 2025-10-21：**第四阶段完成** - 删除FSR缩放技术，保留SMAA抗锯齿
  - ❌ 删除 `servers/rendering/renderer_rd/effects/fsr.h/.cpp` 和 `fsr2.h/.cpp`（约1,500行）
  - ❌ 删除所有FSR2 shader文件
  - ✅ **保留SMAA抗锯齿**：`smaa.h/.cpp` + shader + `thirdparty/smaa/`纹理
  - 说明：FSR是3D场景缩放技术对2D UI无用；SMAA是通用抗锯齿技术，对2D UI渲染质量有价值

- 2025-10-20：恢复 Android 帧律库 thirdparty/swappy-frame-pacing（含 arm64-v8a/armeabi-v7a/x86/x86_64 的 libswappy_static.a）。
  - 构建 Android 时可通过 `swappy=yes` 启用；默认不启用则不链接该库。
  - Windows 编辑器与导出不受影响。
