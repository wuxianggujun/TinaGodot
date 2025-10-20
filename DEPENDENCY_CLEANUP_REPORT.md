# 依赖清理报告（TinaGodot）

> **最后更新**: 2025-10-20
> **状态**: 第二阶段部分完成

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

### 待清理的第三方依赖（thirdparty/）🔄

**网络相关**（4个依赖）：
- 🔄 enet - ENet网络库实现 (0.2MB)
- 🔄 miniupnpc - UPnP端口映射库 (0.3MB)
- 🔄 wslay - WebSocket实现 (0.1MB)
- 🔄 mbedtls - TLS/SSL加密库 (7.7MB)

**导航相关**（2个依赖）：
- 🔄 recastnavigation - 寻路网格 (0.7MB)
- 🔄 rvo2 - 动态避障算法 (0.8MB)

**3D工具**（2个依赖）：
- 🔄 meshoptimizer - 网格优化库 (0.5MB)
- 🔄 xatlas - UV展开库 (0.3MB)

**说明**: 这些依赖库需要运行清理脚本后手动提交：
```bash
# Windows
tools\purge_second_stage.bat

# 或使用 PowerShell
.\tools\purge_second_stage.ps1

# 提交变更
git add -A
git commit -m "chore(purge-thirdparty): 移除网络/导航/3D工具相关依赖"
```

**待清理体积**: ~10.8MB

---

## 📋 可选模块（计划宏化控制）

以下模块计划通过编译宏控制，暂未删除：

### 音频系统（计划通过 `TINAGODOT_ENABLE_AUDIO` 控制）
**模块**：
- ogg - OGG容器格式
- vorbis - Vorbis音频编解码
- minimp3 - MP3音频解码
- interactive_music - 交互式音乐

**第三方依赖**：
- thirdparty/libogg
- thirdparty/libvorbis
- thirdparty/minimp3

**体积**: ~6.4MB（模块3MB + 依赖3.4MB）

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
| 第二阶段-依赖 | 对应第三方库 | ~10.8MB | 🔄 待执行 |
| **已完成总计** | | **~266MB** | |
| 可选模块（待宏化） | 音频+图像+分析 | ~44MB | 📋 计划中 |

**当前状态**：
- thirdparty/ 目录: ~110MB（待执行清理脚本后将减少至~99MB）
- modules/ 目录: 保留核心模块和待宏化模块

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
- thirdparty/amd-fsr/ - AMD FSR缩放
- thirdparty/amd-fsr2/ - AMD FSR 2.0

### 基础图像格式（~7.6MB）
- thirdparty/libpng/ - PNG图像格式
- thirdparty/libjpeg-turbo/ - JPEG图像格式
- thirdparty/libwebp/ - WebP图像格式（可选删除）

### 核心系统库（~15MB）
- thirdparty/zlib/ - ZIP压缩
- thirdparty/minizip/ - ZIP文件处理
- thirdparty/zstd/ - 高效压缩算法
- thirdparty/brotli/ - Brotli压缩
- thirdparty/clipper2/ - 多边形布尔运算
- thirdparty/pcre2/ - 正则表达式库
- thirdparty/misc/ - 杂项工具

### 平台支持库（~10MB）
- thirdparty/sdl/ - 跨平台窗口/输入
- thirdparty/directx_headers/ - Windows DirectX
- thirdparty/d3d12ma/ - DirectX 12内存分配
- thirdparty/linuxbsd_headers/ - Linux头文件
- thirdparty/wayland/ - Linux Wayland支持
- thirdparty/wayland-protocols/ - Wayland协议
- thirdparty/mingw-std-threads/ - MinGW线程支持

**核心依赖总计**: ~110MB（必需保留）

---

## 📝 下一步行动

1. ✅ **已完成**: 物理删除网络/导航模块代码
2. 🔄 **待执行**: 运行第三方依赖清理脚本
3. 📋 **计划中**: 实现音频系统宏控制框架
4. 📋 **计划中**: 实现扩展图像格式宏控制框架
5. 📋 **待评估**: 是否删除libwebp（WebP格式）

---

**报告生成时间**: 2025-10-20
**维护者**: TinaGodot Team
