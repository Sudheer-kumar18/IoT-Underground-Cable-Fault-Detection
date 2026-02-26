# ⚡ IoT-Based Underground Cable Fault Detection System

## 📌 Overview
This project presents an IoT-based system for detecting and locating underground cable faults in real-time. It improves reliability of power distribution by providing early fault detection and instant alerts.

---

## 🚀 Features
- Real-time fault detection
- Fault classification (Line 1, Line 2, Line 3)
- Distance calculation of fault location
- IoT monitoring using Blynk / ThingSpeak
- GPS-based fault location tracking
- Alert system (LCD + Mobile Notification)

---

## 🛠️ Technologies Used
- Arduino / NodeMCU (ESP8266)
- IoT Platform (Blynk / ThingSpeak)
- Sensors (Voltage sensors, switches)
- GPS Module (Neo-6M)
- Embedded C

---

## ⚙️ System Architecture
![Architecture](docs/architecture.png)

---

## 🔌 Hardware Components
- Arduino Uno / NodeMCU
- ESP8266 WiFi Module
- Voltage Sensors
- Resistors
- GPS Module
- LCD Display
- Power Supply (LM7808 Regulator)

---

## 📡 Working Principle
1. System continuously monitors voltage across cables
2. Fault is detected when voltage drops or changes
3. Distance is calculated using resistance method
4. GPS provides exact location
5. Data is sent to IoT dashboard
6. Alerts are generated instantly

---

## 📊 Output
- Fault Type Displayed
- Distance of Fault
- GPS Coordinates
- Cloud Dashboard Visualization

---

## 📷 Project Demo
![Prototype](images/prototype.jpg)

---

## 🔮 Future Enhancements
- Machine Learning-based fault prediction
- Mobile App Integration
- Automatic fault repair system
- AI-based anomaly detection

---

## 👨‍💻 Author
SudheerKumar Malakanti

---

## 📜 License
This project is licensed under the MIT License.
