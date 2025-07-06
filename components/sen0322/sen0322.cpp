#include "sen0322.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace sen0322 {

static const char *const TAG = "sen0322";

// Register commands according to datasheet
static const uint8_t SEN0322_COLLECT_PHASE = 0x01;
static const uint8_t SEN0322_JUDGE_PHASE = 0x02;
static const uint8_t SEN0322_OXYGEN_DATA = 0x03;

void SEN0322Sensor::setup() {
  ESP_LOGCONFIG(TAG, "Setting up SEN0322...");
  
  // Initialize sensor
  if (!this->write_byte(SEN0322_COLLECT_PHASE, 0x00)) {
    ESP_LOGE(TAG, "Failed to initialize sensor");
    this->mark_failed();
    return;
  }
  
  delay(100);
  ESP_LOGCONFIG(TAG, "SEN0322 setup complete");
}

void SEN0322Sensor::dump_config() {
  ESP_LOGCONFIG(TAG, "SEN0322 Oxygen Sensor:");
  LOG_I2C_DEVICE(this);
  LOG_UPDATE_INTERVAL(this);
  LOG_SENSOR("  ", "Oxygen", this);
  
  if (this->is_failed()) {
    ESP_LOGE(TAG, "Communication with SEN0322 failed!");
  }
}

void SEN0322Sensor::update() {
  ESP_LOGV(TAG, "Updating SEN0322 sensor...");
  
  // According to SEN0322 datasheet, we need to read oxygen data directly
  // Write command to read oxygen concentration
  if (!this->write_byte(SEN0322_OXYGEN_DATA, 0x00)) {
    ESP_LOGE(TAG, "Failed to send oxygen data command");
    this->status_set_warning();
    return;
  }
  
  // Wait for sensor to process
  delay(50);
  
  // Read 3 bytes of data
  uint8_t data[3];
  if (!this->read_bytes_raw(data, 3)) {
    ESP_LOGE(TAG, "Failed to read oxygen data");
    this->status_set_warning();
    return;
  }
  
  // Debug: Log raw data
  ESP_LOGD(TAG, "Raw data: 0x%02X 0x%02X 0x%02X", data[0], data[1], data[2]);
  
  // Parse oxygen concentration according to datasheet
  // Method 1: Standard parsing (data[0] = high byte, data[1] = low byte)
  uint16_t raw_oxygen = (data[0] << 8) | data[1];
  float oxygen_concentration = raw_oxygen * 0.01f;  // Changed from 0.1f to 0.01f
  
  // Debug: Log calculated value
  ESP_LOGD(TAG, "Calculated oxygen (method 1): %.2f%%", oxygen_concentration);
  
  // If still invalid, try alternative parsing method
  if (oxygen_concentration < 0.0f || oxygen_concentration > 25.0f) {
    // Method 2: Alternative parsing (reverse byte order)
    raw_oxygen = (data[1] << 8) | data[0];
    oxygen_concentration = raw_oxygen * 0.01f;
    ESP_LOGD(TAG, "Calculated oxygen (method 2): %.2f%%", oxygen_concentration);
  }
  
  // If still invalid, try method 3
  if (oxygen_concentration < 0.0f || oxygen_concentration > 25.0f) {
    // Method 3: Use only first byte
    oxygen_concentration = data[0] * 0.1f;
    ESP_LOGD(TAG, "Calculated oxygen (method 3): %.2f%%", oxygen_concentration);
  }
  
  // Validate reading (oxygen should be between 15-25% in normal air)
  if (oxygen_concentration < 0.0f || oxygen_concentration > 30.0f) {
    ESP_LOGW(TAG, "Invalid oxygen reading: %.2f%% (raw: 0x%04X)", oxygen_concentration, raw_oxygen);
    this->status_set_warning();
    return;
  }
  
  ESP_LOGD(TAG, "Got oxygen concentration: %.2f%%", oxygen_concentration);
  this->publish_state(oxygen_concentration);
  this->status_clear_warning();
}

float SEN0322Sensor::get_setup_priority() const {
  return setup_priority::DATA;
}

}  // namespace sen0322
}  // namespace esphome
