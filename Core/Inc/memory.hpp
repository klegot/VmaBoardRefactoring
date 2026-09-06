#include "main.h"

struct MemoryMap {
  uint8_t pid_target_speed_rpm_conversion[MOTORS_COUNT];
  uint8_t pwm_targets_conversion[4];
  uint16_t vbat1_adc;
  uint16_t vbat2_adc;
  bool killswitch_state;
} __attribute__((__packed__));

class Memory {
public:
  hydrolib::ReturnCode Read(void *buffer, int address, int length);
  hydrolib::ReturnCode Write(const void *buffer, int address, int length);
};