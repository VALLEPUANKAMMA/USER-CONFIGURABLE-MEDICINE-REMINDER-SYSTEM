# USER-CONFIGURABLE MEDICINE REMINDER SYSTEM

## 📌 Aim

To develop a User-Configurable Medicine Reminder System that allows users to set medicine timings and receive automatic alerts at the scheduled time.

---

## 🎯 Objectives

- Display RTC time on LCD
- Allow user configuration of medicine timings
- Monitor schedules in real time
- Trigger alert when medicine time occurs

---

## 🛠 Hardware Requirements

- LPC2148 Microcontroller
- 16x2 LCD Display
- 4x4 Matrix Keypad
- Buzzer
- Switches
- USB-UART Converter / DB-9 Cable

---

## 💻 Software Requirements

- Embedded C Programming
- Flash Magic

---

## 📷 Project Images

### 🔌 Hardware Setup
---
<img width="873" height="760" alt="Screenshot 2026-05-27 142237" src="https://github.com/user-attachments/assets/331543bf-490a-478f-bfa1-c9265282b4a3" />

Complete hardware connection of the medicine reminder system including LPC2148, LCD, keypad, RTC, and buzzer.
---

### 🖥 LCD Output Display

<img width="253" height="206" alt="Screenshot 2026-05-27 143145" src="https://github.com/user-attachments/assets/406d629d-86e2-439f-9e0d-28e9d591b81e" />

LCD displaying current RTC time and system status.
---


### 🔔 Medicine Alert System


<img width="1537" height="1023" alt="image" src="https://github.com/user-attachments/assets/d59f5698-7c1b-48c9-bbce-d28246830b5e" />

LCD showing medicine reminder message: `TAKE MEDICINE NOW` with buzzer alert activation.

---

---

## ⚙️ Working Principle

### 1. System Configuration

- Press Switch-1 to enter setup mode
- Configure:
  - RTC Time
  - Medicine Schedule
- Timings are entered using keypad and stored in memory

### 2. Clock Mode

- If no medicine timing is stored:
  - LCD displays current date and time
  - No buzzer alert is generated

### 3. Real-Time Monitoring

- Controller continuously compares RTC time with stored schedules

### 4. Alert Generation

When medicine time matches:
- LCD displays: `Take Medicine Now`
- Buzzer turns ON/OFF periodically
- 1-minute acknowledgment timer starts

### 5. User Acknowledgment

- Press Switch-2 to stop buzzer
- Reminder gets cleared
- If not pressed within 1 minute:
  - Alert automatically stops

---

## 🔄 Software Flow

1. Initialize RTC, LCD, keypad, buzzer, timer, and interrupts
2. Display current date and time on LCD
3. Detect Switch-1 interrupt for schedule setup
4. Store medicine timing in memory
5. Compare RTC time with schedules continuously
6. Trigger buzzer alert when time matches
7. Wait for acknowledgment using Switch-2
8. Continue monitoring next medicine schedule

---

## ✨ Features

- Real-time clock display
- User configurable medicine schedules
- Automatic medicine reminders
- LCD notification system
- Interrupt-based operation
- Simple and user-friendly design
- RTC-based accurate timing system
- Audible buzzer alert mechanism

---

## 🔮 Future Scope

- Mobile app integration using Bluetooth/Wi-Fi
- SMS notification system for reminders
- Voice-based medicine alerts
- Cloud-based medicine schedule storage
- Multiple user profile support
- Battery backup for power failure situations
- IoT-based healthcare monitoring system

---

## 🚀 Applications

- Elderly patient medicine reminders
- Home healthcare systems
- Hospital monitoring systems
- Personal medication management

---

## 🧰 Technologies Used

- Embedded C
- LPC2148 ARM7 Microcontroller
- RTC Interfacing
- LCD Interfacing
- Keypad Interfacing
- External Interrupts

---

## 📖 Conclusion

The User-Configurable Medicine Reminder System helps users take medicines on time using RTC-based scheduling and automatic alerts. The project demonstrates important embedded system concepts such as interrupt handling, RTC interfacing, LCD display control, and keypad interaction.
