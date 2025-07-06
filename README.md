# ESPHome SEN0322 Oxygen Sensor Component

ESPHome external component for DFRobot SEN0322 I2C Oxygen Sensor.

## Features

- ✅ I2C communication with SEN0322 sensor
- ✅ Automatic sensor initialization
- ✅ Configurable update interval
- ✅ Error handling and status reporting
- ✅ Compatible with Home Assistant
- ✅ Works with ESP8266 and ESP32

## Hardware Requirements

- **Sensor**: DFRobot SEN0322 I2C Oxygen Sensor
- **Microcontroller**: ESP8266 (Wemos D1 Mini, NodeMCU) or ESP32
- **Connections**:
  - VCC → 3.3V or 5V
  - GND → GND
  - SDA → GPIO4 (D2 on Wemos D1 Mini)
  - SCL → GPIO5 (D1 on Wemos D1 Mini)

## Installation

### 1. Add External Component to ESPHome

Add this to your ESPHome YAML configuration:

```yaml
external_components:
  - source: github://makbart1980/esphome-sen0322-oxygen-sensor
    components: [ sen0322 ]

i2c:
  sda: GPIO4  # D2 on Wemos D1 Mini
  scl: GPIO5  # D1 on Wemos D1 Mini
  scan: true  # Optional: scan for I2C devices

sensor:
  - platform: sen0322
    name: "Oxygen Concentration"
    update_interval: 60s  # Default: 60 seconds
    id: oxygen_sensor
```

### 2. Full Configuration Example

```yaml
esphome:
  name: oxygen-monitor
  platform: ESP8266
  board: d1_mini

wifi:
  ssid: "YOUR_WIFI_SSID"
  password: "YOUR_WIFI_PASSWORD"

api:
  encryption:
    key: "YOUR_API_KEY"

ota:
  password: "YOUR_OTA_PASSWORD"

logger:

external_components:
  - source: github://YOUR_GITHUB_USERNAME/esphome-sen0322-oxygen-sensor
    components: [ sen0322 ]

i2c:
  sda: GPIO4  # D2
  scl: GPIO5  # D1
  scan: true

sensor:
  - platform: sen0322
    name: "Oxygen Concentration"
    update_interval: 30s
    id: oxygen_sensor
    accuracy_decimals: 2
```

## Configuration Options

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `name` | String | Required | Name of the sensor in Home Assistant |
| `update_interval` | Time | 60s | How often to read the sensor |
| `id` | ID | Optional | ID for referencing in automations |
| `accuracy_decimals` | Integer | 2 | Number of decimal places to display |

## Troubleshooting

### Sensor Not Detected
- Check I2C connections (SDA/SCL)
- Verify sensor power (VCC/GND)
- Enable I2C scan in configuration
- Check ESPHome logs for I2C devices

### Invalid Readings
- Sensor may need warming up time (2-3 minutes)
- Check for loose connections
- Verify sensor is not damaged
- Normal oxygen range: 20-21% in air

### Compilation Errors
- Update ESPHome to latest version
- Check external component URL
- Verify all component files are present

## Technical Details

- **I2C Address**: 0x73 (115 decimal)
- **Measurement Range**: 0-25% oxygen concentration
- **Resolution**: 0.1%
- **Update Interval**: Configurable (default 60s)
- **Power**: 3.3V or 5V

## License

MIT License - feel free to use and modify!

## Contributing

Issues and pull requests welcome! This component was created for the Home Assistant community.

## Changelog

- **v1.0.0**: Initial release with basic I2C communication
- More features coming soon!
