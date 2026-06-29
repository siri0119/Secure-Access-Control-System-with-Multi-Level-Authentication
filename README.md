# 🔒 Secure Access Control System with Multi-Level Authentication

An advanced, high-security embedded access control application developed for the **ARM7 LPC2148 Microcontroller**. This system implements a strict **Three-Factor Authentication (3FA)** defense pipeline to eliminate unauthorized entry, combining knowledge-based credentials with biometric verification.

---

## 👁️ System Architecture & Data Flow

### Visual Block Diagram
The following layout illustrates how the LPC2148 acts as the central brain, bridging hardware inputs, external memory, and physical actuators:

```text
  ┌─────────────────┐       ┌────────────────────────┐       ┌────────────────┐
  │   4x4 Matrix    │──────>│                        │──────>│ 16x2 Character │
  │     Keypad      │       │                        │       │  LCD Display   │
  └─────────────────┘       │    LPC2148 (ARM7)      │       └────────────────┘
  ┌─────────────────┐       │                        │       ┌────────────────┐
  │ Push Button Switch│──EINT0─>│  • Master Controller   │──────>│  L293D Driver  │
  │ (Admin Trigger) │       │  • Verification Engine │       │   & DC Motor   │
  └─────────────────┘       │                        │       └────────────────┘
  ┌─────────────────┐       │                        │               │
  │ R305 Fingerprint│<─UART0─>│                        │               ▼
  │  Sensor Module  │       │                        │        [ Door Actuator ]
  └─────────────────┘       └────────────────────────┘
                                      ▲    │
                                     I2C  I2C
                                      │    ▼
                            ┌────────────────────────┐
                            │    AT24C256 EEPROM     │
                            │ (Secure Password Vault)│
                            └────────────────────────┘
Images/
│── banner.png
│── architecture.png
│── block_diagram.png
│── authentication_flow.png
│── sequence_diagram.png
│── state_diagram.png
│── data_flow.png
│── use_case.png
│── hardware_setup.png
│── fingerprint_module.png
│── lcd_output.png
│── project_demo.gif
