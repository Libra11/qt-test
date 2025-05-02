# Qt IME 选择器项目

这是一个基于 Qt 的输入法选择器项目，提供了系统级的输入法管理功能。

## 项目结构

```
项目根目录/
├── src/                    # 源代码文件
├── include/               # 头文件
├── ui/                    # UI文件
└── helpers/              # 辅助类文件
```

## 功能特性

- 系统输入法管理
- 内容保护
- 系统控制
- 管理员权限管理
- Hosts 文件管理

## 开发环境

- Qt 5.x 或更高版本
- C++11 或更高版本
- Windows 操作系统

## 构建说明

1. 确保已安装 Qt 开发环境
2. 使用 Qt Creator 打开项目文件（test.pro）
3. 运行 qmake
4. 构建项目

## 依赖项

- Qt Core
- Qt GUI
- Qt Widgets
- Windows API (Imm32, Ole32, Uuid, Oleaut32, User32)
