# IoT-Based-Smart-Pet-Feeder
An IoT-enabled smart pet feeding system developed as a university group project to automate pet feeding through scheduled and remote-controlled operations. The system uses Arduino and ESP32 to provide accurate food dispensing while allowing users to monitor and control feeding through the Blynk IoT platform.

---

## Project Overview

This project was developed to address the challenge of maintaining consistent pet feeding schedules. The system automates food dispensing based on predefined schedules and also allows manual feeding through a mobile application.

The feeder combines sensors, actuators, and wireless communication to provide reliable and accurate food dispensing while reducing the need for constant human supervision.

---

## Features

- Automated scheduled feeding using an RTC module
- Remote feeding through the Blynk mobile application
- Weight-based food portion control using a Load Cell and HX711
- Servo motor controlled food dispensing
- Wi-Fi connectivity via ESP32
- Real-time monitoring and remote operation
- Configurable feeding schedule

---

##  Technologies Used

### Hardware
- Arduino Uno
- ESP32 Wi-Fi Module
- HX711 Load Cell Amplifier
- Load Cell
- Servo Motor
- RTC DS3231 Module
- Breadboard
- Jumper Wires

### Software
- Arduino IDE
- C++
- Blynk IoT Platform
- HX711 Library
- ESP32 Servo Library
- RTClib

---

##  System Workflow

1. The user configures feeding schedules or manually triggers feeding using the Blynk mobile application.
2. The ESP32 receives commands through Wi-Fi.
3. The Arduino checks the RTC for scheduled feeding times.
4. The servo motor dispenses food.
5. The load cell measures the dispensed food weight.
6. Food dispensing stops automatically once the target weight is reached.
7. The system sends status updates through the Blynk platform.

---

##  Project Media

Prototype images, proposal document, and presentation are included in this repository.

---

## Repository Contents

- Arduino source code
- Project proposal
- Project presentation
- Prototype images

---

##  Team Project

This project was developed collaboratively as part of the **IT1040 – Fundamentals of Computing** module at the **Sri Lanka Institute of Information Technology (SLIIT)**.

This repository is shared as part of my academic portfolio to showcase the project and the technologies used.

---

##  Future Improvements

- Mobile push notifications
- Food level monitoring
- Camera integration for pet monitoring
- Feeding history analytics
- Cloud database integration
- Multi-pet support

---
