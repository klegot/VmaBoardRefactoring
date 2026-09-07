#include "main.h"

struct MemoryMap {
  uint8_t pid_target_speed_rpm_conversion[MOTORS_COUNT]; // address 0 length 10
  uint8_t pwm_targets_conversion[4];                     // address 10 length 4
  uint16_t vbat1_adc;                                    // address 14 length 2
  uint16_t vbat2_adc;                                    // address 16 length 2
  bool killswitch_state;                                 // address 18 length 1
} __attribute__((__packed__));

class Memory {
public:
  hydrolib::ReturnCode Read(void *buffer, int address, int length);
  hydrolib::ReturnCode Write(const void *buffer, int address, int length);
};