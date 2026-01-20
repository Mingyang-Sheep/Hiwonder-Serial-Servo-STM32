# Hiwonder/Lobot Serial Bus Servo Driver for STM32
# 幻尔（Lobot）串行总线舵机 STM32 驱动与协议详解

![Platform](https://img.shields.io/badge/Platform-STM32-blue) ![Language](https://img.shields.io/badge/Language-C%2F%2FC%2B%2B-green) ![License](https://img.shields.io/badge/License-MIT-yellow)

[🇺🇸 English Version](#-english-version) | [🇨🇳 中文版本](#-中文版本)

---

<a name="-english-version"></a>
## 🇺🇸 English Version

### Introduction
This repository contains the driver implementation and protocol analysis for **Hiwonder** serial bus servos based on **STM32F103**. 

While the official demo code primarily covers the **Position Mode** (angle control), it often lacks support for **Motor Mode** (continuous rotation/wheel mode). This project fills that gap by providing:
1.  A detailed explanation of the underlying serial communication protocol.
2.  Implementation of the **Motor Mode** driver to make the servo rotate continuously like a DC motor.
3.  Standard **Position Mode** driver implementation.

### Hardware Requirements
* **MCU**: STM32F103 (or compatible STM32 series)
* **Servo**: Hiwonder/Lobot Serial Bus Servos (e.g., LX-16A, LX-15D, etc.)
* **Driver Board**: 
    * **TTL Debug Board** (Recommended): For direct protocol development (transparent transmission).
    * **Bus Servo Controller**: Can be used, but requires understanding its specific secondary development protocol.

### Features
- [x] **Position Mode**: Control the servo to rotate to a specific angle within a specific time.
- [x] **Motor Mode**: Control the servo to rotate continuously at a specific speed (Clockwise/Counter-clockwise).
- [x] **Protocol Parsing**: Raw byte construction for serial packets including Checksum calculation.

### Wiring (TTL Debug Board)
| STM32 Pin | TTL Board Pin | Description |
| :--- | :--- | :--- |
| PA9 (TX) | RXD | Serial Transmit |
| PA10 (RX)| TXD | Serial Receive |
| GND | GND | Ground |
| - | VCC | Servo Power Supply (6V-8.4V) |

### Key Code Snippet: Motor Mode
The following function demonstrates how to switch the servo to Motor Mode using the raw protocol:

```c
/**
 * @brief  Set Servo to Motor Mode (Continuous Rotation)
 * @param  servoID: The ID of the servo (0-253)
 * @param  rotationMode: 0 = Stop/Position Mode, 1 = Speed Control Mode
 * @param  speed: Rotation speed (-1000 to 1000). Negative for reverse.
 */
void setServoMotorMode(uint8_t servoID, uint8_t rotationMode, int16_t speed)
{
    uint16_t speedComplement;
    uint8_t speedLow, speedHigh;
    uint8_t length, checksum;
    uint16_t sum;

    if (servoID > 31) return;

    // Handle speed value (Two's complement for negative values)
    speedComplement = (uint16_t)speed;
    speedLow = speedComplement & 0xFF;
    speedHigh = (speedComplement >> 8) & 0xFF;

    // Packet Length = Parameters count + 1 (CMD)
    length = 7; 

    // Checksum Calculation: sum(ID + Length + Cmd + Params)
    // Cmd for Motor Mode is 29 (0x1D)
    sum = servoID + length + 0x1D + 1 + rotationMode + speedLow + speedHigh;
    checksum = ~(sum & 0xFF);

    // Send Buffer Construction (Example)
    // LobotTxBuf[0] = 0x55; // Header
    // LobotTxBuf[1] = 0x55; // Header
    // ... (See source code for full implementation)
}

```
---

<a name="-中文版本"></a>

## 🇨🇳 中文版本

### 简介

本项目提供了基于 **STM32F103** 的 **幻尔（Hiwonder）** 串行总线舵机驱动实现及底层协议解析。

官方提供的驱动例程主要针对 **位置模式**（角度控制），往往缺少 **电机模式**（轮式模式/连续旋转）的实现代码。本项目旨在补充这一缺失，提供：

1. 底层串口通信协议的详细解析。
2. **电机模式** 驱动代码实现，使舵机能像直流电机一样连续旋转。
3. 标准的 **位置模式** 驱动实现。

### 硬件需求

* **MCU**: STM32F103 (或兼容的 STM32 系列)
* **舵机**: 幻尔/Lobot 串行总线舵机 (如 LX-16A, LX-15D 等)
* **驱动板**:
* **TTL 调试板** (推荐): 用于透传开发，直接发送协议数据包。
* **总线舵机控制器**: 可以使用，但需要配合其特定的二次开发库。



### 功能特性

* [x] **位置模式**: 控制舵机在指定时间内转动到指定角度。
* [x] **电机模式**: 控制舵机以指定速度连续旋转（顺时针/逆时针）。
* [x] **协议解析**: 实现了包含校验和计算在内的原始数据包构建。

### 接线说明 (使用 TTL 调试板)

| STM32 引脚 | TTL 调试板引脚 | 说明 |
| --- | --- | --- |
| PA9 (TX) | RXD | 串口发送 |
| PA10 (RX) | TXD | 串口接收 |
| GND | GND | 共地 |
| - | VCC | 舵机供电 (6V-8.4V) |

### 核心代码：电机模式

以下函数展示了如何通过构建底层协议数据包将舵机切换至电机模式：

```c
/**
 * @brief  设置舵机为电机模式 (连续旋转)
 * @param  servoID: 舵机 ID (0-253)
 * @param  rotationMode: 旋转模式参数 (通常为 1)
 * @param  speed: 旋转速度 (-1000 到 1000). 负数代表反转.
 */
void setServoMotorMode(uint8_t servoID, uint8_t rotationMode, int16_t speed)
{
    uint16_t speedComplement;
    uint8_t speedLow, speedHigh;
    uint8_t length, checksum;
    uint16_t sum;

    if (servoID > 31) return;

    // 处理速度值（负数转换为补码，并拆分高低八位）
    speedComplement = (uint16_t)speed;
    speedLow = speedComplement & 0xFF;
    speedHigh = (speedComplement >> 8) & 0xFF;

    // 数据长度计算
    // 协议格式：0x55 0x55 ID Length Cmd P1 P2... Checksum
    length = 7; 

    // 校验和计算
    // 校验和 = ~(ID + Length + Cmd + Prm1 + ... PrmN)
    // 0x1D 是电机模式的指令代码 (Decimal 29)
    sum = servoID + length + 0x1D + 1 + rotationMode + speedLow + speedHigh;
    checksum = ~(sum & 0xFF);

    // 发送数据包 (此处为伪代码，请调用你的串口发送函数)
    // LobotTxBuf[0] = 0x55; // 帧头
    // ... 详细实现请参考仓库源码
}

```
---

## ⚠️ Disclaimer / 免责声明

This code is for learning and reference purposes. Please ensure your hardware wiring is correct before testing to avoid damaging the servo or the controller.
本代码仅供学习参考。在测试前请务必确保硬件接线正确，以免损坏舵机或控制器。
