# SenseAR Effects特效引擎iOS SDK集成Demo

本项目是**商汤科技**提供的[**特效引擎 SDK**](https://sensear.softsugar.com/) iOS 集成演示工程，旨在帮助开发者快速了解如何在iOS项目中集成与使用我们的特效引擎SDK。您可以通过运行本 Demo，直观体验SDK提供的各类人脸特效、贴纸、美颜、滤镜等功能。

---

## 功能亮点

- 人脸检测与跟踪
- 美颜效果（磨皮、美白、瘦脸等）
- AR 贴纸/道具实时叠加
- 实时滤镜效果
- 视频流处理与渲染
- 高性能渲染支持（基于 OpenGL ES/Metal）

---

## 环境要求

- Xcode 8 及以上
- iOS 9.0 及以上
- 真机运行（部分功能依赖相机）

---

## 运行Demo

- clone工程到本地
- 将从商汤商务渠道获取的license文件放入工程（需要将名字改为"SENSEME.lic"）
- 将工程的Bundle Identifier修改为与上述license绑定的Bundle Identifier
- 完成工程编译及App在测试机的安装，运行Demo

> 请[**提交免费试用申请**](https://sensear.softsugar.com/)，或**联系商务**（Tel: 181-1640-5190）获取测试license。

---

## SDK 集成说明

本项目已经完成对SDK的集成，您无需单独引入SDK依赖。

如果您需要在自己的项目中引入SDK，请参考如下步骤：

1. 导入SDK
导入SenseME Effects iOS SDK头文件、静态库文件（libst_mobile.a）。

2. 添加链接库
SenseME Effects依赖C++，在TARGETS -> Build Settings -> Linking -> Other LinkerFlags中添加`-lc++`。

3. 关闭Bitcode
SenseMe Effects不支持Bitcode，将TARGETS -> Build Settings -> Build Options -> Enable Bitcode 设置为 `NO`。

4. 导入配套的Objective-C封装
将Wrapper文件夹导入工程。

> [详细接入文档](./SenseMeEffects/st_mobile_sdk/docs/SenseAR集成文档.md)

---

## 反馈

- 如果您在使用过程中有遇到什么问题，欢迎提交 [**issue**](https://github.com/SoftSugar-Inc/effects-ios/issues)。
- 我们真诚地感谢您的贡献，欢迎通过 GitHub 的 fork 和 pull request 流程来提交代码。代码风格请参考[**Coding Guidelines for Cocoa**](https://developer.apple.com/library/archive/documentation/Cocoa/Conceptual/CodingGuidelines/CodingGuidelines.html)
