# 🔐 SECURE ACCESS CONTROL SYSTEM WITH MULTI-LEVEL AUTHENTICATION

<p align="center">
  <img src="https://img.shields.io/badge/Platform-LPC2148-blue" />
  <img src="https://img.shields.io/badge/Language-Embedded_C-green" />
  <img src="https://img.shields.io/badge/Security-Multi_Level_Authentication-red" />
  <img src="https://img.shields.io/badge/Biometric-R305_Fingerprint-orange" />
</p>

> 🔒 Multi-Level Authentication System  
> [cite_start]👤 User ID ➜ 🔑 Password ➜ 👆 Fingerprint Verification [cite: 6]

[cite_start]An embedded security solution developed using the **LPC2148 ARM7 Microcontroller**, **R305 Fingerprint Sensor**, **AT24C256 EEPROM**, and **L293D Motor Driver** to provide secure door access through multi-level authentication[cite: 21, 22, 23, 24, 27, 28].

---

# 📌 Project Overview

[cite_start]The Secure Access Control System is designed to prevent unauthorized access by implementing three layers of authentication[cite: 5].

[cite_start]✅ User ID Verification [cite: 6]

[cite_start]✅ Password Verification [cite: 6]

[cite_start]✅ Fingerprint Authentication [cite: 6]

[cite_start]Only when all security levels are successfully verified is access granted[cite: 7].

The system also provides:
- [cite_start]🔑 Password Management [cite: 56, 57]
- [cite_start]👆 Fingerprint Enrollment [cite: 64, 67]
- [cite_start]❌ Fingerprint Deletion [cite: 64]
- [cite_start]🚪 Automatic Door Lock Control [cite: 90, 91]

---

# 🎯 Objectives

- [cite_start]🔐 Improve access security [cite: 8]
- [cite_start]👤 Verify User ID [cite: 6]
- [cite_start]🔑 Verify Password using EEPROM [cite: 6, 80]
- [cite_start]👆 Authenticate Fingerprint [cite: 6, 83]
- [cite_start]🚪 Control Door Lock Mechanism [cite: 91]
- [cite_start]⚙️ Provide Administrative Controls [cite: 56]
- [cite_start]💾 Secure Credential Storage [cite: 61, 80]

---

# 👁️ System Architecture & Data Flow

### Visual Block Diagram
The following layout illustrates how the LPC2148 acts as the central brain, bridging hardware inputs, external memory, and physical actuators:

![System Architecture](ChatGPT_Image_Jun_29_2026_12_47_09_PM.png)

---

# 🔄 Interactive Process Pipelines

### 1️⃣ Main Authentication Pipeline Track
[cite_start]This chart visualizes the linear verification gate requirements used to isolate valid users from malicious entry attempts[cite: 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92]:

![Main Authentication Flowchart](ChatGPT_Image_Jun_29_2026_12_45_11_PM.png)

### 2️⃣ Administrative Control Track (EINT0 Hardware Interrupt)
[cite_start]When the physical setup switch is closed, standard operations pause via an External Interrupt service to load configuration panels[cite: 56]:

![Administrative Control Flowchart](ChatGPT_Image_Jun_29_2026_12_58_56_PM.png)

---

# 🧠 Working Principle

## 1️⃣ System Initialization

<p align="center">
  <img src="IMAGES/loading.png" alt="Loading" width="450">
</p>

### Description
- System powers ON.
- [cite_start]LPC2148 initializes LCD, EEPROM, Keypad and Fingerprint Module[cite: 76].
- [cite_start]User authentication service starts[cite: 77].
- System enters standby mode.

---

## 2️⃣ Registration / Login Menu

<p align="center">
  <img src="IMAGES/entry.png" alt="Registration Login Menu" width="450">
</p>

### Available Options
### 👤 Registration
- Create User Account
- Create Password
- [cite_start]Enroll Fingerprint [cite: 67]

### 🔑 Login
- [cite_start]Authenticate Existing User [cite: 77]
- [cite_start]Access Door Lock System [cite: 90]

---

## 3️⃣ User ID Entry

<p align="center">
  <img src="IMAGES/id.png" alt="User ID Entry" width="450">
</p>

### Description
- [cite_start]User enters ID using keypad[cite: 78].
- [cite_start]ID is validated by LPC2148[cite: 78].
- [cite_start]After successful ID entry, password screen appears[cite: 78].

---

## 4️⃣ Password Entry

### Hidden Password Mode 🔒

<p align="center">
  <img src="IMAGES/hide_pass.png" alt="Hidden Password" width="450">
</p>

### Description
- [cite_start]User enters password using keypad[cite: 79].
- Password characters are hidden using '*' to protect against shoulder surfing attacks.

---

### Show Password Feature 👁️

<p align="center">
  <img src="IMAGES/unhide_pass.png" alt="Show Password" width="450">
</p>

### Description
- User presses the assigned keypad key.
- Password becomes visible.
- User can verify entered password before submission.

### Benefits
✅ Better usability  
✅ Password confirmation  
✅ Improved user experience

---

## 5️⃣ Fingerprint Verification

<p align="center">
  <img src="IMAGES/placefinger.png" alt="Place Finger" width="450">
</p>

### Description
- [cite_start]User places finger on R305 sensor[cite: 85].
- [cite_start]Fingerprint image is captured[cite: 85].
- [cite_start]Template is generated and matched against stored records[cite: 86, 87].

### Authentication
[cite_start]✅ Match Found → Access Granted [cite: 88, 90]  
[cite_start]❌ Match Not Found → Access Denied [cite: 92]

---

## 6️⃣ Login Success

<p align="center">
  <img src="IMAGES/loginsuccess.png" alt="Login Success" width="450">
</p>

### Description
- [cite_start]User ID Verified [cite: 78]
- [cite_start]Password Verified [cite: 81]
- [cite_start]Fingerprint Verified [cite: 88]

[cite_start]All security levels successfully passed[cite: 90].

---

## 7️⃣ Door Opening

<p align="center">
  <img src="IMAGES/doors_opening.png" alt="Door Opening" width="450">
</p>

### Action
- LPC2148 activates L293D Driver.
- [cite_start]DC Motor rotates forward[cite: 91].
- [cite_start]Door unlocks and opens automatically[cite: 90, 91].

[cite_start]🟢 ACCESS GRANTED [cite: 91]

---

## 8️⃣ Door Closing

<p align="center">
  <img src="IMAGES/doors_closing.png" alt="Door Closing" width="450">
</p>

### Action
- Door remains open for predefined duration.
- [cite_start]DC Motor rotates in reverse direction[cite: 91].
- [cite_start]Door closes automatically[cite: 91].

[cite_start]🔒 SYSTEM LOCKED [cite: 91]

---

## 9️⃣ Administrative Mode (External Interrupt)

### Edit Menu

<p align="center">
  <img src="IMAGES/edit.png" alt="Edit Menu" width="500">
</p>

### Activated By
[cite_start]🔘 **EINT0 Switch** 

### Available Options
[cite_start]🔑 Edit Password [cite: 56]  
[cite_start]👆 Edit Fingerprint [cite: 56]  
♻️ Reset System  
🚪 Exit  

---

## 🔟 Fingerprint Management

<p align="center">
  <img src="IMAGES/finger_edit.png" alt="Fingerprint Management" width="550">
</p>

### Available Functions
### ➕ Enroll
- [cite_start]Capture Fingerprint [cite: 69]
- [cite_start]Generate Template [cite: 72]
- [cite_start]Store Template [cite: 74]

### 🔍 Search
- [cite_start]Verify User Fingerprint [cite: 84]

### ❌ Delete
- [cite_start]Remove Existing Fingerprint [cite: 64]

### 🚪 Exit
- Return to Main Menu

---

# 🛠️ Hardware Requirements

- [cite_start]🧠 LPC2148 ARM7 Microcontroller [cite: 21]
- [cite_start]👆 R305 Fingerprint Sensor [cite: 22]
- [cite_start]💾 AT24C256 EEPROM [cite: 24]
- [cite_start]📺 16×2 LCD [cite: 23]
- [cite_start]⌨️ 4×4 Matrix Keypad [cite: 26]
- [cite_start]🔘 Push Button Switch [cite: 25]
- [cite_start]⚙️ L293D Motor Driver [cite: 27]
- [cite_start]🚪 DC Motor [cite: 28]
- [cite_start]🔌 USB-UART Converter [cite: 29]

---

# 💻 Software Requirements

- [cite_start]📝 Embedded C [cite: 32]
- [cite_start]🛠️ Keil µVision [cite: 33]
- [cite_start]⚡ Flash Magic [cite: 34]

---

# ✨ Features

[cite_start]✅ Multi-Level Authentication [cite: 5]  
[cite_start]✅ Password Protection [cite: 6]  
[cite_start]✅ Fingerprint Verification [cite: 6]  
[cite_start]✅ EEPROM Password Storage [cite: 61]  
[cite_start]✅ Door Lock Automation [cite: 90]  
[cite_start]✅ Password Editing [cite: 57]  
[cite_start]✅ Fingerprint Enrollment [cite: 64]  
[cite_start]✅ Fingerprint Deletion [cite: 64]  
[cite_start]✅ LCD User Interface [cite: 56]  
[cite_start]✅ Secure Access Control [cite: 5]  

---

# 🚀 Future Enhancements

- 🌐 Embedded Linux Integration
- ☁️ Cloud Access Logs
- 📱 Mobile Application Monitoring
- 📡 GSM Notifications
- 🎥 Face Recognition
- 🪪 RFID + Fingerprint Authentication
- 📊 Access Analytics Dashboard

---

# 👨‍💻 Developed By

## Manikanta Karthik Pantham
- [cite_start]Developed Secure Access Control System using LPC2148 ARM7 [cite: 5, 21]
- [cite_start]Implemented EEPROM-Based Password Authentication [cite: 61, 80]
- [cite_start]Integrated R305 Fingerprint Module [cite: 83]
- [cite_start]Developed LCD User Interface [cite: 56]
- [cite_start]Designed Door Lock Control using L293D and DC Motor [cite: 91]
- [cite_start]Programmed using Embedded C [cite: 32]

---

# 📜 License

[cite_start]This project is intended for academic and educational purposes[cite: 1, 39]. Free to use and modify with proper credit.

---
⭐ If you find this project useful, give it a star!
