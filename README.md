# STM32 Fly-By-Wire Telemetry & Closed-Loop Control System

![C](https://img.shields.io/badge/Language-C11-blue.svg)
![Platform](https://img.shields.io/badge/Platform-STM32F446RE%20(ARM%20Cortex--M4)-brightgreen.svg)
![RTOS](https://img.shields.io/badge/RTOS-FreeRTOS%20v2-orange.svg)
![Architecture](https://img.shields.io/badge/Architecture-BSP%20%2F%20FSM%20%2F%20Decoupled-purple.svg)

A robust, real-time embedded firmware architecture built on an **STM32F446RE (Nucleo-64)** platform. The system implements a deterministic, multi-tasking closed-loop stabilization platform driven by an **MPU6500 6-DOF IMU** and an **SG90 Micro Servo**, coupled with a deterministic **UART-based telemetry pipeline** using a Finite State Machine (FSM) packet parser and an $O(1)$ function pointer dispatcher.

Designed with clean architecture principles, Board Support Package (BSP) decoupling, static memory encapsulation, and strict thread-safe data handling.

---

## Key Architectural Highlights

* **Clean Architecture & Hardware Abstraction (BSP):** High-level application logic (Dispatcher, CLI, FSM) is decoupled from ST's Hardware Abstraction Layer (HAL). UART transmission and peripheral I/O are routed through dedicated BSP drivers.
* **Deterministic FSM Packet Parser:** Robust byte-by-byte frame parser capable of processing custom telemetry frames while isolating payload extraction and verifying frame integrity via XOR checksums.
* **$O(1)$ Function Pointer Command Dispatcher:** Eliminates branch mispredictions and unbounded `switch-case` evaluation latency by routing validated command IDs to handlers via a direct lookup jump table.
* **Data Encapsulation & Thread Safety:** Mutex-shielded access (`osMutex`) guarantees non-blocking read/write synchronization between the high-frequency sensor acquisition task and real-time UART telemetry serialization.
* **Zero Global Pollution:** Replaced arbitrary `extern` declarations with dedicated thread-safe Getter APIs (`telemetry_api.h`) and translation-unit-scoped (`static`) allocations in compliance with MISRA-C principles.

---

## Hardware Configuration & Pinout

| Peripheral | Component | Pin / Bus | Interface / Configuration |
| :--- | :--- | :--- | :--- |
| **MCU** | STM32F446RET6 | Nucleo-64 | ARM Cortex-M4 @ 180 MHz |
| **IMU** | MPU6500 | `PB8` (SCL), `PB9` (SDA) | I2C1 (Fast Mode 400 kHz) |
| **Servo Actuator** | SG90 Micro Servo | `PA6` | TIM3 Channel 1 (PWM @ 50 Hz, 20 ms Period) |
| **Telemetry Interface** | On-Board ST-LINK VCP | `PA2` (TX), `PA3` (RX) | USART2 (115200 bps, 8N1) |

---

## Protocol Specification

Binary packets follow a deterministic framing structure:

```text
+--------------+---------------+------------+------------------+--------------+
| Header (1B)  |  Length (1B)  |  CmdID (1B)| Payload (0-128B) | Checksum (1B)|
+--------------+---------------+------------+------------------+--------------+
|     0xAA     | Len (Cmd+Data)|  0x01-0xFF |  Variable Data   | Length ^ Cmd |
+--------------+---------------+------------+------------------+--------------+
