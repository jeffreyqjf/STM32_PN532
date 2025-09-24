# STM32_PN532
[中文](README.md) | English

Based on **STM32**, this project provides a complete **PN532 NFC/RFID** driver and engineering implementation.  
It offers full low-level drivers, protocol encapsulation, and application examples to enable NFC functions such as card detection and UID reading on STM32 platforms with an easy-to-use interface.

---

## 📂 Project Structure
```text
STM32_PN532/ 
├── Library/                         # Low-level hardware drivers and common library files
├── Module/                          # Functional implementation of the project
├── Listings/                        # Code and test programs
├── Start/                           # Startup files and system initialization
├── System/                          # System-level modules
├── User/                            # User application layer
├── Project.uvprojx                  # Keil µVision project file
├── Project.uvoptx                   # Keil project configuration
├── ProgramLogs.txt                  # Debug logs
├── mcuisp.exe                       # Serial flashing tool
├── mcuispConfig.ini                 # Flashing tool configuration
└── pemicro_connection_settings.ini  # Flashing tool configuration
```

---

## ✨ Features
- **Multi-interface support**: Communication with PN532 via UART/I²C  
- **Card compatibility**: ISO14443A (MIFARE Classic/Ultralight, etc.)  
- **Basic operations**: Card detection, UID reading, Flash data read/write, OLED display  
- **Modular design**: Driver layer and application layer are separated for easy porting and expansion  

---

## 💡 Hardware & Environment
- **MCU Platform**: STM32 series (example: STMF103C8T6)  
- **NFC Module**: PN532 module (supports I²C/SPI/UART interfaces)  
- **Development Environment**: Keil µVision5  
- **Power Supply**: The PCB used in this example supports 9V battery input and Micro-USB input; the MCU requires 3.3V power  

---

## 🚀 Quick Start
1. **Clone the repository**
   ```bash
   git clone https://github.com/jeffreyqjf/STM32_PN532.git

2. **Open the Project**

Use **Keil µVision** to open the project file **`Project.uvprojx`**.

3. **Hardware Connections**

Refer to **[HARDWARE.md](./HARDWARE.md)** and related PCB documents for detailed wiring information.

4. **Build and Flash**

- Compile the project in **Keil**  
- Download the firmware to the **STM32 development board** and run the program

---

## 📚 API Reference

Please refer to the source code and comments in the **`Module/`** directory for more details.

---

## ⚠️ Known Issues

- Only the basic framework is provided  
- **P2P** and **card emulation** functions are still under development  
- Array overflow issues caused by excessively long keyboard input remain unresolved

---

## 📜 License

- No specific license is provided for this project  
- For commercial use or secondary development, please contact the author or add your own license

---

## 🔗 References

- [PN532 Datasheet](https://www.nxp.com/docs/en/user-guide/141520.pdf)  
- STM32 Official Reference Manual  
- OLED code reference from **BiliBili - Jiang Xie Technology**

