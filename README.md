# STM32_PN532
[English](README_EN.md) | 中文

基于 **STM32** 的 **PN532 NFC/RFID** 驱动与工程。  
本项目提供完整的底层驱动、协议封装以及程序，在 STM32 平台上使用易于交互的用户界面实现了 NFC 功能，包括卡片识别等。

---

## 📂 项目结构
```text
STM32_PN532/ 
├── Library/                         # 底层硬件驱动与通用库文件 
├── Module/                          # 项目功能实现
├── Listings/                        # 代码与测试程序 
├── Start/                           # 启动文件、系统初始化代码 
├── System/                          # 系统使用模块 
├── User/                            # 用户应用层
├── Project.uvprojx                  # Keil µVision 工程文件 
├── Project.uvoptx                   # Keil 项目配置 
├── ProgramLogs.txt                  # 调试日志 
├── mcuisp.exe                       # 串口烧录软件 
├── mcuispConfig.ini                 # 烧录软件配置 
└── pemicro_connection_settings.ini  # 烧录软件配置 
```

---

## ✨ 功能特性
- **多接口支持**：UART/I²C 与 PN532 通信  
- **常见卡型兼容**：ISO14443A (MIFARE Classic/Ultralight 等)  
- **基础操作**：卡片识别、UID 读取、Flash 数据读写、OLED 显示  
- **模块化设计**：驱动层与应用层分离，便于移植与扩展  

---

## 💡 硬件与环境要求
- **MCU 平台**：STM32 系列（本例为 STMF103C8T6）
- **NFC 模块**：PN532 模块（支持 I²C/SPI/UART 接口）
- **开发环境**：Keil µVision5
- **供电**：本例所用 PCB 支持 9V 电池输入与 Micro-USB 输入，芯片需 3.3V 输入

---

## 🚀 快速上手
1. **克隆项目**
   ```bash
   git clone https://github.com/jeffreyqjf/STM32_PN532.git

2. **打开工程**

    使用 **Keil µVision** 打开项目文件 **`Project.uvprojx`**。

3. **硬件连接**

    详情见 [HARDWARE.md](./HARDWARE.md) 与 PCB 相关文件与文档。

4. **编译下载**

- 在 Keil 中编译工程
- 下载固件至 **STM32 开发板** 运行程序

---

## 📚 API 说明

> 请参考 **`Module/`** 目录中的源码与注释。

---

## ⚠️ 已知问题
- 目前仅提供基本框架  
- **P2P** 和 **卡模拟** 功能仍在开发中
- 对于键盘输入过长导致的数组溢出问题没有解决

---

## 📜 许可证
- 本项目未指定 License  
- 如需商用或二次开发，请联系作者或自行添加 License

---

## 🔗 参考资料
- [PN532 数据手册](https://www.nxp.com/docs/en/user-guide/141520.pdf)  
- STM32 官方参考手册
- BiliBili **江协科技** 的 OLED 代码
