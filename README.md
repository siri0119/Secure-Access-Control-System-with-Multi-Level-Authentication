# 🔐 Secure Access Control System using LPC2148 ARM7

<p align="center">

<img src="https://img.shields.io/badge/Embedded%20System-LPC2148-blue?style=for-the-badge"/>
<img src="https://img.shields.io/badge/Language-Embedded%20C-success?style=for-the-badge"/>
<img src="https://img.shields.io/badge/Fingerprint-R305-orange?style=for-the-badge"/>
<img src="https://img.shields.io/badge/EEPROM-AT24C256-red?style=for-the-badge"/>
<img src="https://img.shields.io/badge/Authentication-3%20Level-green?style=for-the-badge"/>
<img src="https://img.shields.io/badge/Status-Completed-brightgreen?style=for-the-badge"/>

</p>

---

# ⭐ Overview

A highly secure **Embedded Access Control System** developed using the **LPC2148 ARM7 Microcontroller** implementing **Three-Level Authentication**.

Unlike conventional password systems, this project verifies

- 👤 User ID
- 🔑 Password
- 👆 Fingerprint

before unlocking the door.

The system stores passwords securely inside an **AT24C256 EEPROM** while fingerprint templates are maintained in the **R305 sensor flash memory**.


# 📷 Complete System Architecture

<p align="center">

![Project Screenshot](system_architecture.png)
</p>

---

# 🎯 Project Objectives

- Prevent unauthorized entry
- Provide Multi-Level Authentication
- Secure Password Storage
- Biometric Authentication
- Automatic Door Lock
- Password Modification
- Fingerprint Enrollment
- Fingerprint Deletion
- Interrupt Based Admin Menu

---

# 🧩 Hardware Components

| Component | Purpose |
|------------|---------|
| LPC2148 ARM7 | Main Controller |
| R305 Fingerprint Sensor | Biometric Authentication |
| AT24C256 EEPROM | Password Storage |
| L293D Driver | Motor Driver |
| DC Motor | Door Lock |
| 16x2 LCD | User Interface |
| 4x4 Matrix Keypad | User Input |
| Push Button | Admin Interrupt |

---

# 🖥 System Block Diagram

<p align="center">

![Project Screenshot](block.png)
</p>

---

# 🔐 Authentication Flow

<p align="center">

![Project Screenshot](Authentication.png)

</p>


---

# 🔑 Admin Mode (External Interrupt)

Admin Mode is triggered through **EINT0**.

Once activated, administrator can

- Change Password
- Enroll Fingerprint
- Delete Fingerprint

---

# 🛠 Admin Flow

<p align="center">

![Project Screenshot](admin.png)

</p>

---

## Password Update Process

```
Press Admin Button
        │
        ▼
Verify Current Password
        │
        ▼
Enter New Password
        │
        ▼
Confirm Password
        │
        ▼
Same ?
 │           │
No          Yes
 │           │
 │      Save EEPROM
 │
Retry
```

---

## Fingerprint Enrollment

```
Select Enroll

↓

Capture Image 1

↓

Capture Image 2

↓

Generate Template

↓

Assign User Index

↓

Store into Sensor Flash

↓

Success
```

---

## Fingerprint Delete

```
Admin

↓

Delete

↓

Enter Finger ID

↓

Delete Template

↓

Success
```

---

# 🔄 Data Flow Diagram

```text
Keypad
   │
   ▼
LPC2148
   │
   ├──────────────► LCD
   │
   ├──────────────► EEPROM
   │
   ├──────────────► Fingerprint Sensor
   │
   └──────────────► Motor Driver
                          │
                          ▼
                      Door Lock
```

---

# 💾 Memory Allocation

| Memory | Usage |
|----------|---------|
| RAM | User Input |
| Flash | Program |
| EEPROM | Password |
| Sensor Flash | Fingerprints |

---

# 🔧 Communication Protocols

| Device | Protocol |
|----------|------------|
| EEPROM | I2C |
| Fingerprint | UART |
| LCD | GPIO |
| Keypad | GPIO |
| Motor Driver | GPIO |

---

# 🔒 Security Layers

```
Layer 1

User ID

↓

Layer 2

Password Verification

↓

Layer 3

Fingerprint Authentication

↓

Door Unlock
```

---

# 🚀 Features

- Multi-Level Authentication
- Fingerprint Authentication
- Password Protection
- EEPROM Storage
- Interrupt Driven Admin Mode
- Fingerprint Enrollment
- Fingerprint Delete
- Automatic Door Control
- LCD Interface
- Embedded C Firmware
- ARM7 Based Security

---

# 📂 Project Folder Structure

```text
Secure-Access-Control-System
│
├── SOURCE CODE
│
├── HEADER FILES
│
├── PROJECT REPORT
│
├── CIRCUIT DIAGRAM
│
├── README.md
│
└── IMAGES
```

---

# 📸 Project Gallery

| System | Authentication |
|---------|----------------|
| ![](IMAGES/system_architecture.png) | ![](IMAGES/authentication_flow.png) |

---

| Admin | Block Diagram |
|-------|----------------|
| ![](IMAGES/admin_flow.png) | ![](IMAGES/block_diagram.jpg) |

---

# 💻 Software Used

- Embedded C
- Keil μVision
- Flash Magic

---

# 🔌 Hardware Used

- LPC2148
- R305
- AT24C256
- LCD
- L293D
- DC Motor
- Matrix Keypad
- Push Button

---

# 👨‍💻 Author

**Palakurla Shirisha Goud**

Bachelor of Technology (Information Technology)

Embedded Systems Engineer

2025 Graduate

---

# 📜 License

This project is intended for educational and academic purposes.

Feel free to fork, modify and improve the project.

---

# ⭐ Support

If you like this project,

⭐ Star this Repository

🍴 Fork it

📢 Share it

```
"Security begins with strong authentication."

```
