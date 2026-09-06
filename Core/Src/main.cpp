/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.cpp
 * @brief          : Merged Main program body (C++)
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
extern "C" {
#include "main.h"
}

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "hydrolib_bus_datalink_stream.hpp"
#include "hydrolib_bus_application_slave.hpp"
#include "hydrv_gpio_low.hpp"
#include "hydrv_uart.hpp"
#include "memory.hpp"
#include <chrono>
#include <cstring>
#include <ctime>

extern "C" {
#include <sys/time.h>
int _gettimeofday(struct timeval *tv, void *tz) {
  if (tv) {
    tv->tv_sec = HAL_GetTick() / 1000;
    tv->tv_usec = (HAL_GetTick() % 1000) * 1000;
  }
  return 0;
}
}
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
#define DSHOT_MAX_RPM 6000

constinit hydrv::GPIO::GPIOLow rx_pin1(hydrv::GPIO::GPIOLow::GPIOA_port, 10,
                                       hydrv::GPIO::GPIOLow::GPIO_UART_RX);
constinit hydrv::GPIO::GPIOLow tx_pin1(hydrv::GPIO::GPIOLow::GPIOA_port, 9,
                                       hydrv::GPIO::GPIOLow::GPIO_UART_TX);
constinit hydrv::UART::UART<255, 255>
    uart1(hydrv::UART::UARTLow::USART1_115200_LOW, rx_pin1, tx_pin1, 7);

constinit hydrv::GPIO::GPIOLow rx_pin2(hydrv::GPIO::GPIOLow::GPIOD_port, 6,
                                       hydrv::GPIO::GPIOLow::GPIO_UART_RX);
constinit hydrv::GPIO::GPIOLow tx_pin2(hydrv::GPIO::GPIOLow::GPIOD_port, 5,
                                       hydrv::GPIO::GPIOLow::GPIO_UART_TX);
constinit hydrv::UART::UART<255, 255>
    uart2(hydrv::UART::UARTLow::USART2_115200_LOW, rx_pin2, tx_pin2, 7);

class Logger {
public:
  Logger() = default;
};

Logger logger;
static inline Memory memory{};

static inline hydrolib::bus::datalink::StreamManager manager(1, uart1, logger);
static inline hydrolib::bus::datalink::Stream stream(manager, 2);
static inline hydrolib::bus::application::Slave slave(stream, memory, logger);

MemoryMap system_data = {
    .pid_target_speed_rpm_conversion = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    .pwm_targets_conversion = {150, 150, 150, 150},
    .vbat1_adc = 0,
    .vbat2_adc = 0,
    .killswitch_state = false};

float value = 0.0;
uint16_t pwm_targets[4] = {1500, 1500, 1500, 1500};
volatile float pid_target_speed_rpms[MOTORS_COUNT] = {0};

uint8_t pinState = 0;
uint16_t adc_buffer[2];
volatile bool battery_data_ready = false;

inline hydrolib::ReturnCode Memory::Read(void *read_buffer, int address,
                                         int length) {
  if (address < 0 || (address + length) > sizeof(MemoryMap)) {
    return hydrolib::ReturnCode::FAIL;
  }

  switch (address) {
  case offsetof(MemoryMap, pid_target_speed_rpm_conversion): {
    memcpy(read_buffer, &system_data.pid_target_speed_rpm_conversion,
           sizeof(system_data.pid_target_speed_rpm_conversion));
    length -= sizeof(system_data.pid_target_speed_rpm_conversion);

    for (int i = 0; i < 10; i++) {
      if (system_data.pid_target_speed_rpm_conversion[i] >= 100 &&
          system_data.pid_target_speed_rpm_conversion[i] <= 200) {
        int32_t signed_val =
            (int32_t)system_data.pid_target_speed_rpm_conversion[i] - 150;
        pid_target_speed_rpms[i] = (float)signed_val * 120.0f;
      }
    }
    if (length > 0) {
      void *next_buffer = static_cast<uint8_t *>(read_buffer) +
                          sizeof(system_data.pid_target_speed_rpm_conversion);
      return Read(next_buffer,
                  address + sizeof(system_data.pid_target_speed_rpm_conversion),
                  length);
    }
    break;
  }
  case offsetof(MemoryMap, pwm_targets_conversion): {
    memcpy(read_buffer, &system_data.pwm_targets_conversion,
           sizeof(system_data.pwm_targets_conversion));
    length -= sizeof(system_data.pwm_targets_conversion);

    for (int i = 0; i < 4; i++) {
      if (system_data.pwm_targets_conversion[i] >= 100 &&
          system_data.pwm_targets_conversion[i] <= 200) {
        pwm_targets[i] = (uint16_t)(system_data.pwm_targets_conversion[i] * 10);
      }
    }
    if (length > 0) {
      void *next_buffer = static_cast<uint8_t *>(read_buffer) +
                          sizeof(system_data.pwm_targets_conversion);
      return Read(next_buffer,
                  address + sizeof(system_data.pwm_targets_conversion), length);
    }
    break;
  }
  case offsetof(MemoryMap, vbat1_adc): {
    memcpy(read_buffer, &system_data.vbat1_adc, sizeof(system_data.vbat1_adc));
    length -= sizeof(system_data.vbat1_adc);
    if (length > 0) {
      void *next_buffer =
          static_cast<uint8_t *>(read_buffer) + sizeof(system_data.vbat1_adc);
      return Read(next_buffer, address + sizeof(system_data.vbat1_adc), length);
    }
    break;
  }
  case offsetof(MemoryMap, vbat2_adc): {
    memcpy(read_buffer, &system_data.vbat2_adc, sizeof(system_data.vbat2_adc));
    length -= sizeof(system_data.vbat2_adc);
    if (length > 0) {
      void *next_buffer =
          static_cast<uint8_t *>(read_buffer) + sizeof(system_data.vbat2_adc);
      return Read(next_buffer, address + sizeof(system_data.vbat2_adc), length);
    }
    break;
  }
  case offsetof(MemoryMap, killswitch_state): {
    memcpy(read_buffer, &system_data.killswitch_state,
           sizeof(system_data.killswitch_state));
    length -= sizeof(system_data.killswitch_state);
    if (length > 0) {
      void *next_buffer = static_cast<uint8_t *>(read_buffer) +
                          sizeof(system_data.killswitch_state);
      return Read(next_buffer, address + sizeof(system_data.killswitch_state),
                  length);
    }
    break;
  }

  default:
    return hydrolib::ReturnCode::FAIL;
  }
  return hydrolib::ReturnCode::OK;
}

inline hydrolib::ReturnCode Memory::Write(const void *write_buffer, int address,
                                          int length) {
  if (address < 0 || (address + length) > sizeof(MemoryMap)) {
    return hydrolib::ReturnCode::FAIL;
  }

  switch (address) {
  case offsetof(MemoryMap, pid_target_speed_rpm_conversion): {
    memcpy(&system_data.pid_target_speed_rpm_conversion, write_buffer,
           sizeof(system_data.pid_target_speed_rpm_conversion));
    length -= sizeof(system_data.pid_target_speed_rpm_conversion);
    if (length > 0) {
      const void *next_buffer =
          static_cast<const uint8_t *>(write_buffer) +
          sizeof(system_data.pid_target_speed_rpm_conversion);
      return Write(next_buffer,
                   address +
                       sizeof(system_data.pid_target_speed_rpm_conversion),
                   length);
    }
    break;
  }
  case offsetof(MemoryMap, pwm_targets_conversion): {
    memcpy(&system_data.pwm_targets_conversion, write_buffer,
           sizeof(system_data.pwm_targets_conversion));
    length -= sizeof(system_data.pwm_targets_conversion);
    if (length > 0) {
      const void *next_buffer = static_cast<const uint8_t *>(write_buffer) +
                                sizeof(system_data.pwm_targets_conversion);
      return Write(next_buffer,
                   address + sizeof(system_data.pwm_targets_conversion),
                   length);
    }
    break;
  }
  case offsetof(MemoryMap, vbat1_adc): {
    memcpy(&system_data.vbat1_adc, write_buffer, sizeof(system_data.vbat1_adc));
    length -= sizeof(system_data.vbat1_adc);
    if (length > 0) {
      const void *next_buffer = static_cast<const uint8_t *>(write_buffer) +
                                sizeof(system_data.vbat1_adc);
      return Write(next_buffer, address + sizeof(system_data.vbat1_adc),
                   length);
    }
    break;
  }
  case offsetof(MemoryMap, vbat2_adc): {
    memcpy(&system_data.vbat2_adc, write_buffer, sizeof(system_data.vbat2_adc));
    length -= sizeof(system_data.vbat2_adc);
    if (length > 0) {
      const void *next_buffer = static_cast<const uint8_t *>(write_buffer) +
                                sizeof(system_data.vbat2_adc);
      return Write(next_buffer, address + sizeof(system_data.vbat2_adc),
                   length);
    }
    break;
  }
  case offsetof(MemoryMap, killswitch_state): {
    memcpy(&system_data.killswitch_state, write_buffer,
           sizeof(system_data.killswitch_state));
    length -= sizeof(system_data.killswitch_state);
    if (length > 0) {
      const void *next_buffer = static_cast<const uint8_t *>(write_buffer) +
                                sizeof(system_data.killswitch_state);
      return Write(next_buffer, address + sizeof(system_data.killswitch_state),
                   length);
    }
    break;
  }
  default:
    return hydrolib::ReturnCode::FAIL;
  }

  return hydrolib::ReturnCode::OK;
}

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
extern "C" {
void SystemClock_Config(void);
}
void adc_start(void);
void calibration(void);
void quick_battery_read(void);

/* Private user code ---------------------------------------------------------*/
void adc_start(void) { HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buffer, 2); }

void quick_battery_read(void) {
  system_data.vbat1_adc = adc_buffer[0];
  system_data.vbat2_adc = adc_buffer[1];
}

void calibration(void) {
  for (int i = 0; i < MOTORS_COUNT; i++) {
    pid_target_speed_rpms[i] = value;
  }
  for (int i = 0; i < MOTORS_COUNT; i++) {
    motor_values[i] = prepare_Dshot_package(0, false);
  }
  uint32_t calibration_start_time = HAL_GetTick();
  while (HAL_GetTick() - calibration_start_time < 2000) {
    update_motors_Tx_Only();
    for (volatile int i = 0; i < 100; i++)
      ;
  }
  for (int i = 0; i < MOTORS_COUNT; i++) {
    motor_values[i] = prepare_Dshot_package(10, false);
  }
  for (int t = 0; t < 6; t++) {
    update_motors_Tx_Only();
  }
  for (int i = 0; i < MOTORS_COUNT; i++) {
    motor_values[i] = prepare_Dshot_package(12, false);
  }
  for (int t = 0; t < 6; t++) {
    update_motors_Tx_Only();
  }
  HAL_Delay(40);
}

int main(void) {
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_TIM8_Init();
  MX_TIM9_Init();
  MX_TIM12_Init();
  MX_ADC1_Init();

  PWM_Init();
  adc_start();
  setup_Dshot_Tx_Only();
  preset_bb_Dshot_buffers();
  pid_reset_all();
  calibration();

  //NVIC_SetPriorityGrouping(0);
  uart1.Init();
  uart2.Init();

  uint32_t last_50hz_time = 0;
  uint32_t last_100hz_time = 0;
  uint16_t count = 0;
  uint16_t err = 0;

  GPIOC->MODER |= GPIO_MODER_MODER13_0;
  GPIOC->MODER |= GPIO_MODER_MODER14_0;

  while (1) {
    system_data.killswitch_state = (GPIOA->IDR & GPIO_PIN_3) ? true : false;
    if (pinState == 0 && system_data.killswitch_state == 1) {
      HAL_Delay(200);
      calibration();
      pinState = system_data.killswitch_state;
    }

    if (battery_data_ready) {
      quick_battery_read();
      battery_data_ready = false;
    }

    if (telemetry_done_flag) {
      process_telemetry_with_new_method();
      for (int m = 0; m < MOTORS_COUNT; m++) {
        uint32_t current_rpm = motor_telemetry_data[m].valid_rpm
                                   ? motor_telemetry_data[m].raw_rpm_value
                                   : 0;
        float dt = 0.005f;
        uint16_t new_command =
            pid_calculate_command(m, current_rpm, pid_target_speed_rpms[m], dt);
        motor_values[m] = prepare_Dshot_package(new_command, true);
      }
      update_motors_Tx_Only();
      GPIOC->ODR |= GPIO_ODR_OD13;
      GPIOC->ODR &= ~GPIO_ODR_OD14;
      err = 0;
    } else {
      if (err > 1000) {
        GPIOC->ODR &= ~GPIO_ODR_OD13;
        GPIOC->ODR |= GPIO_ODR_OD14;
      }
      err++;
    }

    uint32_t now2 = HAL_GetTick();
    if (now2 - last_50hz_time >= 20) {
      if (count == 0) {
        PWM_SetDuty(&htim9, TIM_CHANNEL_1, 1000);
        PWM_SetDuty(&htim9, TIM_CHANNEL_2, 1000);
        count++;
      } else {
        PWM_SetDuty(&htim9, TIM_CHANNEL_1, pwm_targets[0]);
        PWM_SetDuty(&htim9, TIM_CHANNEL_2, pwm_targets[1]);
      }
      last_50hz_time = now2;
    }

    if (now2 - last_100hz_time >= 10) {
      PWM_SetDuty(&htim12, TIM_CHANNEL_1, pwm_targets[2]);
      PWM_SetDuty(&htim12, TIM_CHANNEL_2, pwm_targets[3]);
      last_100hz_time = now2;
    }
  }
}

extern "C" void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

extern "C" void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
  if (hadc->Instance == ADC1) {
    battery_data_ready = true;
  }
}

extern "C" {
void USART1_IRQHandler(void) {
  uart1.IRQCallback();
  manager.Process();
  slave.Process();
}
}

extern "C" {
void USART2_IRQHandler(void) {
  uart2.IRQCallback();
  manager.Process();
  slave.Process();
}
}

extern "C" void Error_Handler(void) {
  __disable_irq();
  while (1) {
  }
}