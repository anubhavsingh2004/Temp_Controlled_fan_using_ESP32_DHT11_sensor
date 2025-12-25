# Temperature-Aware Smart Fan System

An IoT-based temperature-controlled fan system using ESP32, DHT11 sensor, and L298N motor driver. The system monitors ambient temperature and automatically controls a DC fan based on predefined thresholds, with both automatic and manual control modes via MQTT.

## 📋 Features

- **Automatic Temperature Control**: Fan automatically turns ON when temperature exceeds 30°C
- **Manual Control**: Override automatic mode via MQTT commands (ON/OFF/AUTO)
- **Real-time Monitoring**: Publishes temperature, humidity, and fan status to MQTT broker
- **Node-RED Dashboard**: Visual interface for monitoring and controlling the fan system
- **WiFi Connectivity**: Remote monitoring and control capabilities
- **PWM Speed Control**: Uses L298N motor driver for fan speed regulation

## 🔧 Hardware Requirements

- **ESP32 Development Board**
- **DHT11 Temperature & Humidity Sensor**
- **L298N Motor Driver Module**
- **DC Fan** (5V or 12V depending on your setup)
- **Power Supply** (appropriate for your fan and motor driver)
- **Jumper Wires**
- **Breadboard** (optional)

## 📐 Circuit Connections

### DHT11 Sensor
- VCC → 3.3V (ESP32)
- GND → GND (ESP32)
- DATA → GPIO 4 (ESP32)

### L298N Motor Driver
- ENA → GPIO 13 (ESP32) - PWM for speed control
- IN1 → GPIO 12 (ESP32)
- IN2 → GPIO 14 (ESP32)
- OUT1 & OUT2 → DC Fan terminals
- 12V → External power supply
- GND → Common ground with ESP32

## 📚 Software Requirements

### Arduino Libraries
```cpp
- WiFi.h (built-in)
- PubSubClient.h (MQTT client)
- DHT.h (DHT sensor library)
```

### Installation
1. Install the required libraries via Arduino Library Manager:
   - **PubSubClient** by Nick O'Leary
   - **DHT sensor library** by Adafruit
   - **Adafruit Unified Sensor** (dependency for DHT)

2. Select **ESP32 Dev Module** as your board in Arduino IDE

### Additional Software
- **MQTT Broker** (Mosquitto, HiveMQ, or any MQTT broker)
- **Node-RED** (optional, for dashboard visualization)

## ⚙️ Configuration

Before uploading the code, update the following credentials in the Arduino sketch:

```cpp
// WiFi Credentials
const char* ssid = "Your_WiFi_SSID";
const char* password = "Your_WiFi_Password";

// MQTT Broker
const char* mqtt_server = "Your_MQTT_Broker_IP";
```

### Configurable Parameters
- `tempThreshold = 30.0`: Temperature threshold in °C to trigger fan
- MQTT Topics:
  - `fanSystem/temperature`: Temperature readings
  - `fanSystem/humidity`: Humidity readings
  - `fanSystem/status`: Fan status (ON/OFF)
  - `fanSystem/command`: Control commands (ON/OFF/AUTO)

## 🚀 Getting Started

### 1. Hardware Setup
Connect all components according to the circuit diagram above.

### 2. Upload Code
1. Open `temp_aware_fan_using_DHT11/temp_aware_fan_using_DHT11.ino` in Arduino IDE
2. Update WiFi and MQTT credentials
3. Select the correct COM port
4. Upload the sketch to ESP32

### 3. MQTT Broker Setup
Ensure your MQTT broker is running and accessible on the network:
```bash
# Install Mosquitto (example for Linux/WSL)
sudo apt-get install mosquitto mosquitto-clients

# Start Mosquitto broker
mosquitto -v
```

### 4. Node-RED Dashboard (Optional)
1. Install Node-RED if not already installed
2. Import the dashboard flow from `temp_aware_fan_using_DHT11/dashboard_new.json`
3. Configure MQTT broker settings in Node-RED
4. Deploy the flow and access the dashboard

## 📱 Usage

### Automatic Mode (Default)
The system operates in automatic mode by default:
- Fan turns **ON** when temperature > 30°C
- Fan turns **OFF** when temperature ≤ 30°C

### Manual Control
Send MQTT commands to `fanSystem/command`:
- `ON`: Force fan on (disables auto mode)
- `OFF`: Force fan off (disables auto mode)
- `AUTO`: Return to automatic temperature-based control

### Example MQTT Commands
```bash
# Using mosquitto_pub
mosquitto_pub -h localhost -t "fanSystem/command" -m "ON"
mosquitto_pub -h localhost -t "fanSystem/command" -m "OFF"
mosquitto_pub -h localhost -t "fanSystem/command" -m "AUTO"

# Subscribe to status updates
mosquitto_sub -h localhost -t "fanSystem/#"
```

## 📊 MQTT Topics

| Topic | Type | Description |
|-------|------|-------------|
| `fanSystem/temperature` | Publish | Current temperature in °C |
| `fanSystem/humidity` | Publish | Current humidity in % |
| `fanSystem/status` | Publish | Fan status (ON/OFF) |
| `fanSystem/command` | Subscribe | Control commands (ON/OFF/AUTO) |

## 🎨 Node-RED Dashboard

The included Node-RED flow provides:
- Real-time temperature gauge
- Humidity display
- Fan status indicator
- Manual control buttons (ON/OFF/AUTO)
- Historical data charts

Import `dashboard_new.json` into Node-RED to get started.

## 🔍 Troubleshooting

### DHT11 Sensor Issues
- **"Failed to read from DHT sensor!"**: Check wiring, ensure sensor has adequate power
- Add a 10kΩ pull-up resistor between DATA and VCC if readings are unstable

### WiFi Connection Problems
- Verify SSID and password are correct
- Ensure ESP32 is within WiFi range
- Check if router supports 2.4GHz (ESP32 doesn't support 5GHz)

### MQTT Connection Failed
- Verify MQTT broker IP address
- Ensure broker is running: `mosquitto -v`
- Check firewall settings (port 1883)
- Try connecting with an MQTT client tool first

### Fan Not Running
- Check L298N power supply (should be appropriate for your fan)
- Verify all connections to L298N module
- Test fan directly with power supply
- Ensure ESP32 and L298N share common ground

## 📝 Code Structure

- **Setup Functions**: WiFi and MQTT initialization
- **Callback Function**: Handles incoming MQTT commands
- **Fan Control**: Functions to turn fan ON/OFF with PWM
- **Main Loop**: Temperature monitoring and auto-control logic

## 🛠️ Future Enhancements

- [ ] Add multiple temperature thresholds for variable fan speeds
- [ ] Implement PID control for smooth fan speed adjustment
- [ ] Add local display (OLED/LCD) for status
- [ ] Battery backup support
- [ ] Web server for configuration without code changes
- [ ] Data logging to SD card or cloud
- [ ] Push notifications for temperature alerts




