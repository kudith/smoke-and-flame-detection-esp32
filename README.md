# Smoke And Flame Detection with ESP32

This project is an IoT-based smoke and flame detection system using an ESP32 microcontroller. The system detects smoke and flames using sensors and sends notifications via Telegram when thresholds are exceeded.

## Features

- Detects smoke using an MQ2 sensor
- Detects flames using an analog flame sensor
- Sends notifications to multiple Telegram chat IDs
- Activates an LED and buzzer when smoke or flames are detected
- Controls a relay to activate external devices

## Hardware Requirements

- ESP32 microcontroller
- MQ2 smoke sensor
- Analog flame sensor
- LED
- Buzzer
- Relay module
- WiFi connection

## Software Requirements

- Arduino IDE
- WiFi library
- HTTPClient library
