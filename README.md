# IoT-Based Smart Garbage Management System

An IoT-enabled smart waste management system designed to classify waste, monitor bin levels, and send notifications when bins are full. This system improves waste segregation efficiency and helps in maintaining a cleaner environment.

## Features
- **Automated Waste Segregation**: Classifies waste as wet or dry using a soil moisture sensor.
- **Real-time Monitoring**: Uses ultrasonic sensors to measure bin fill levels.
- **Notification System**: Sends SMS alerts when bins reach maximum capacity using a GSM module.
- **GPS Tracking**: Helps in tracking bin locations.
- **LCD Display**: Displays status messages on an OLED screen.
- **Buzzer Alert**: Provides an audio alert when necessary.

## Components Used
- **Microcontroller**: Arduino Uno
- **Sensors**:
  - Soil Moisture Sensor (A0) – Classifies waste as wet or dry.
  - Ultrasonic Sensors – Measure waste levels inside the bins.
    - Sensor 1: Echo on pin 12, Trigger on pin 13
    - Sensor 2: Echo on pin 3, Trigger on pin 2
    - Sensor 3: Echo on pin 4, Trigger on pin 5
- **Actuator**: Servo Motor (Pin 7) – Directs waste to the appropriate bin.
- **Communication Modules**:
  - GSM Module (TXD: Pin 1, RXD: Pin 0) – Sends SMS alerts.
  - GPS Module (RXD: Pin 6) – Tracks bin location.
- **Display**: OLED LCD (Adafruit 128x32 SSD1306)
  - SCL to A4, SDA to A5
- **Buzzer**: Provides alerts (Pin 8)

## Installation & Usage
1. Clone the repository:
   ```sh
   git clone https://github.com/Quickaman/IoT--Smart-Garbage-Management-System.git
   cd IoT--Smart-Garbage-Management-System
   ```
2. Open the Arduino IDE and upload the code to the Arduino Uno.
3. Connect all components as per the wiring diagram.
4. Power up the system and monitor the bin status on the OLED display.
5. Receive SMS alerts when the bins are full.

## Working Principle
1. When waste is placed in the system, the soil moisture sensor determines if it is wet or dry.
2. The servo motor directs the waste into the correct bin.
3. Ultrasonic sensors monitor the bin's fill level.
4. When a bin reaches its maximum capacity, the GSM module sends an SMS alert.
5. The OLED display shows real-time status updates.

## Future Enhancements
- **Mobile App Integration**: To provide real-time monitoring on smartphones.
- **Machine Learning**: For improved waste classification.
- **Solar Power Integration**: For sustainable operation.

## License
This project is open-source and available under the MIT License.

## Authors
- Aman Chaurasiya and Others

## Acknowledgments
Special thanks to all contributors and mentors for their guidance in developing this system.

