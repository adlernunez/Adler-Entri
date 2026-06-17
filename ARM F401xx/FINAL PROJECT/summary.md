# STM32 Bare-Metal Driver Development Portfolio
**Platform:** STM32 Nucleo  
**Architecture:** ARM Cortex-M4  
**Framework:** Bare-Metal C 

## Executive Summary
This portfolio showcases a complete, ground-up implementation of essential embedded communication protocols and hardware abstraction layers. By bypassing standard manufacturer libraries (like STM32 HAL), these projects demonstrate a deep understanding of microcontroller memory maps, bus matrices, clock configurations, and register-level hardware manipulation. 

The resulting custom drivers abstract complex bitwise operations into clean, reusable, and human-readable APIs, culminating in a robust system capable of real-time input processing, visual rendering, and multi-device networking.

---

## Project Modules Overview

### 1. GPIO: Interactive Reflex Game & Hardware Foundation
* **Protocol:** General Purpose Input/Output (GPIO)
* **Hardware:** Internal LED (PA5), User Button (PC13)
* **Technical Achievement:** Developed the foundational driver to manage the AHB1 bus, abstracting pin modes (Input, Output, Alternate Function), output types (Push-Pull/Open-Drain), and speeds. Implemented a software-debounced reflex game that dynamically scales in difficulty, proving the reliability of the input polling logic.

### 2. USART: Asynchronous Serial Telemetry
* **Protocol:** Universal Synchronous/Asynchronous Receiver-Transmitter (USART)
* **Hardware:** ST-Link Virtual COM Port (PA2/PA3)
* **Technical Achievement:** Built an asynchronous driver capable of fractional baud rate generation (e.g., 9600 baud) derived from the APB clock. Implemented strict hardware flag polling (`TXE`, `TC`) to safely transmit formatted string buffers to a PC terminal without dropping characters or overflowing memory.

### 3. I2C: OLED Graphical Interface
* **Protocol:** Inter-Integrated Circuit (I2C)
* **Hardware:** SSD1306 128x64 OLED Display
* **Technical Achievement:** Navigated the unforgiving I2C hardware state machine (START -> Address -> ACK -> Data -> STOP) to control an external display. Engineered safety timeout mechanisms to prevent CPU deadlocks during hardware disconnections, and implemented a custom ASCII font dictionary to render text onto a purely graphical pixel array.

### 4. SPI: Master-Slave Device Networking
* **Protocol:** Serial Peripheral Interface (SPI)
* **Hardware:** STM32 (Master) to Arduino Uno (Slave)
* **Technical Achievement:** Established a high-speed, full-duplex communication bridge. Managed Clock Polarity (CPOL) and Phase (CPHA) synchronization, Shift Register timing, and Software Slave Management (SSM) to successfully offload data payloads to a secondary microcontroller operating on a different logic level.

---

## System Integration & Architecture
While each driver was developed and tested as a standalone module, they share a unified architectural philosophy:
1. **Handle & Config Structures:** Every peripheral is managed via a `Handle_t` structure, decoupling the hardware address from the user's software configuration.
2. **Clock Gating:** Strict adherence to ARM power-saving principles, ensuring peripheral clocks are only enabled when explicitly required via custom RCC macros.
3. **Safety & Stability:** Built-in safeguards against common hardware traps, such as SPI Overrun errors (`OVR`), I2C clock stretching freezes, and mechanical switch bouncing.

## Core Engineering Competencies Demonstrated
* **Datasheet Navigation:** Translating complex Silicon Reference Manuals (RM0390/RM0368) into actionable C code.
* **Bitwise Arithmetic:** Safely masking, clearing, and setting specific 32-bit register fields.
* **Hardware Synchronization:** Managing strict timing requirements and hardware status flags to synchronize CPU execution with slower external peripherals.
* **API Design:** Creating intuitive software interfaces that hide underlying hardware complexity from the end-user application layer.
