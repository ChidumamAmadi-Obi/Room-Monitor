# Smart Room-Monitor 2nd Year Mini Project TUD

A USB-powered embedded system built on the ATmega328 microcontroller for real-time monitoring of indoor temperature, humidity, and air quality. Displays live sensor data on an LCD screen, provides audible/visual alerts when environmental values go out of range, and offers user-customizable settings via a rotary encoder interface.

![RoomMonitor gif](https://github.com/user-attachments/assets/9264ad2e-0142-4514-a347-18b75458b19e)

## 🔧 Features
- **Real-Time Environment Monitoring**
  - Monitors temperature, humidity, and air quality using onboard sensors.
  - Data displayed on a 16x2 LCD screen.
  
- **User Alerts**
  - Activates buzzer and LED alert if values exceed user-defined thresholds.
  
- **Customizable Parameters**
  - Rotary encoder allows users to:
    - Adjust acceptable temperature/humidity limits.
    - Change the LCD backlight color.
  
- **USB Powered**
  - Plug-and-play operation via any 5V USB power source.

## 🧠 Tech Stack & Components

- **Microcontroller**: ATmega328 (Arduino Uno-compatible)
-  **Firmware**: Embedded C / Arduino framework
- **Power**: 5V USB via micro-USB or USB-A
- **Components**:
  - DHT11 (Temperature + Humidity)
  - MQ503 (Air Quality Sensor)
  - 16x2 LCD with I2C interface
  - Rotary Encoder with push-button
  - Buzzer for audible alerts
  - LED for visual alerts

## 🛠️ How It Works
![Screenshot 2025-06-01 210545](https://github.com/user-attachments/assets/69fddbdb-38e8-4065-92b9-37dd44a8eb5f)
1. On start up the system displays a loading animation to allow the air quality sensor to heat up

2. The system goes into its monitoring state, displaying air quality, temperature and humidity readings on the display. its possible to customize the backlight color here.
3. When the encoder is pressed the system goes into its menus state, the user can customize the temperature and humidity parameters here.
4. When the encoder is pressed again the system goes back into its monitoring state.

## Setup Instructions 

1. Clone this repository:
```
git clone https://github.com/ChidumamAmadi-Obi/Room-Monitor.git
cd "Room-Monitor/EnviromentalRoomMonitor/Arduino Code"
```
2. Open the .ino file in the Arduino IDE.
3. install and include required libraries:
```
#include "DHT.h"
#include "rgb_lcd.h"
#include "Air_Quality_Sensor.h"
#include <Wire.h>
#include <SimpleRotary.h>
#include <avr/wdt.h>
#include "CustomChars.h"
```
4. Connect the hardware ad per wireing diagram.

<img width="963" height="727" alt="image" src="https://github.com/user-attachments/assets/671eedaa-b898-473e-81a0-19852f81f875" />

5. Flash the code to your Arduino.
6. Use the rotary encoder to navigate menus and configure settings.

## 💡Future Improvements

- Integrate Bluetooth / WiFi for remote monitoring
- Upgrade to touchscreen UI or OLED display
- Lipo Battery Power

## 📸 Screenshots / Media

 **Input/Output Diagram**
 
![Screenshot 2025-06-01 210504](https://github.com/user-attachments/assets/11fefbc9-64a6-42c8-b282-090952b944ae)

 **Assembled PCB**
 
![PCB](https://github.com/user-attachments/assets/fe57c225-1a96-4b35-8c68-d41d812004ea)
![PCB2](https://github.com/user-attachments/assets/b6b66159-c3de-483c-81bd-5781723ce417)

