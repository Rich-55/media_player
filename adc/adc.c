#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_common.h"
#include "fsl_device_registers.h"
#include "board.h"
#include "pin_mux.h"
#include "fsl_debug_console.h"
#include "fsl_adc16.h"
#include <MKL46Z4.h>
#include <stdio.h>

#define SW1_GPIO       GPIOC         // GPIO của nút SW1
#define SW1_PIN        3U            // Chân PTC3
#define SW1_PORT       PORTC         // Cổng PORTC
#define SW1_IRQ        PORTC_PORTD_IRQn  // Ngắt của PORTC

#define SW2_GPIO       GPIOC         // GPIO của nút SW2
#define SW2_PIN        10U           // Chân PTC10
#define SW2_PORT       PORTC         // Cổng PORTC
#define SW2_IRQ        PORTC_PORTD_IRQn

#define SW4_GPIO       GPIOC         // GPIO của nút SW4
#define SW4_PIN        11U           // Chân PTC11
#define SW4_PORT       PORTC         // Cổng PORTC
#define SW4_IRQ        PORTC_PORTD_IRQn

#define SW3_GPIO       GPIOC         // GPIO của nút SW3
#define SW3_PIN        12U           // Chân PTC12
#define SW3_PORT       PORTC         // Cổng PORTC
#define SW3_IRQ        PORTC_PORTD_IRQn  // Ngắt của PORTC

#define DEMO_ADC16_BASE ADC0                 // Using ADC0
#define DEMO_ADC16_CHANNEL_GROUP 0U         // Channel group 0
#define DEMO_ADC16_USER_CHANNEL 3U          // Channel ADC0_SE3 (PTE22)
#define ADC_MAX_VALUE 4095.0                // 12-bit resolution
#define VREFH 3.3                           // Reference voltage
#define DELAY_MS 800                       // Delay between ADC readings (2 seconds)
#define AVERAGE_COUNT 10                    // Số lần đọc để tính trung bình

volatile uint32_t sys_tick_counter = 0;   // Global time counter
volatile bool adc_running = false;        // Flag to control ADC running state
volatile bool sw1_pressed = false;        // Flag cho SW1
volatile bool sw2_pressed = false;        // Flag cho SW2
volatile bool sw3_pressed = false;        // Flag cho SW3
volatile bool sw4_pressed = false;        // Flag cho SW4
volatile bool is_playing = true;          // Trạng thái play/pause

void SysTick_Handler(void) {
    sys_tick_counter++;
}

void delay_ms(uint32_t delay) {
    uint32_t start_time = sys_tick_counter;
    while ((sys_tick_counter - start_time) < delay);
}

void init_uart(void) {
    BOARD_InitDebugConsole(); // UART init is handled here
}

void uart_send_string(const char *str) {
    while (*str) {
        PUTCHAR(*str++); // Gửi từng ký tự trong chuỗi
    }
    PUTCHAR('\n'); // Kết thúc chuỗi bằng ký tự xuống dòng
}

void init_switch_interrupt(void) {
    CLOCK_EnableClock(kCLOCK_PortC);

    // Configure các nút SW1, SW2, SW3, SW4 làm GPIO input với pull-up resistor
    const port_pin_config_t switch_config = {
        kPORT_PullUp,               // Enable pull-up resistor
        kPORT_SlowSlewRate,         // Slow slew rate
        kPORT_PassiveFilterDisable, // Disable passive filter
        kPORT_LowDriveStrength,     // Low drive strength
        kPORT_MuxAsGpio             // Configure as GPIO
    };

    // SW1
    PORT_SetPinConfig(SW1_PORT, SW1_PIN, &switch_config);
    GPIO_PinInit(SW1_GPIO, SW1_PIN, &(gpio_pin_config_t){kGPIO_DigitalInput, 0});
    PORT_SetPinInterruptConfig(SW1_PORT, SW1_PIN, kPORT_InterruptFallingEdge);

    // SW3
    PORT_SetPinConfig(SW3_PORT, SW3_PIN, &switch_config);
    GPIO_PinInit(SW3_GPIO, SW3_PIN, &(gpio_pin_config_t){kGPIO_DigitalInput, 0});
    PORT_SetPinInterruptConfig(SW3_PORT, SW3_PIN, kPORT_InterruptFallingEdge);

    // SW2
    PORT_SetPinConfig(SW2_PORT, SW2_PIN, &switch_config);
    GPIO_PinInit(SW2_GPIO, SW2_PIN, &(gpio_pin_config_t){kGPIO_DigitalInput, 0});
    PORT_SetPinInterruptConfig(SW2_PORT, SW2_PIN, kPORT_InterruptFallingEdge);

    // SW4
    PORT_SetPinConfig(SW4_PORT, SW4_PIN, &switch_config);
    GPIO_PinInit(SW4_GPIO, SW4_PIN, &(gpio_pin_config_t){kGPIO_DigitalInput, 0});
    PORT_SetPinInterruptConfig(SW4_PORT, SW4_PIN, kPORT_InterruptFallingEdge);

    // Enable IRQ for PORTC_PORTD_IRQn
    EnableIRQ(SW1_IRQ);
}


void PORTC_PORTD_IRQHandler(void) {
    uint32_t flags = PORT_GetPinsInterruptFlags(PORTC);

    // Xử lý ngắt SW1
    if (flags & (1U << SW1_PIN)) {
        PORT_ClearPinsInterruptFlags(PORTC, (1U << SW1_PIN));
        sw1_pressed = true; // Đặt cờ cho SW1
    }

    // Xử lý ngắt SW3
    if (flags & (1U << SW3_PIN)) {
        PORT_ClearPinsInterruptFlags(PORTC, (1U << SW3_PIN));
        sw3_pressed = true; // Đặt cờ cho SW3
    }

    // Xử lý ngắt SW2
    if (flags & (1U << SW2_PIN)) {
        PORT_ClearPinsInterruptFlags(PORTC, (1U << SW2_PIN));
        sw2_pressed = true;
       }

    // Xử lý ngắt SW4
    if (flags & (1U << SW4_PIN)) {
        PORT_ClearPinsInterruptFlags(PORTC, (1U << SW4_PIN));
        sw4_pressed = true;
       }
}

void init_adc(void) {
    adc16_config_t adc16ConfigStruct;

    ADC16_GetDefaultConfig(&adc16ConfigStruct);
    adc16ConfigStruct.referenceVoltageSource = kADC16_ReferenceVoltageSourceVref; // VREFH = 3.3V
    adc16ConfigStruct.resolution = kADC16_ResolutionSE12Bit;                      // 12-bit
    ADC16_Init(DEMO_ADC16_BASE, &adc16ConfigStruct);
    ADC16_EnableHardwareTrigger(DEMO_ADC16_BASE, false);                          // Use software trigger

#if defined(FSL_FEATURE_ADC16_HAS_CALIBRATION) && FSL_FEATURE_ADC16_HAS_CALIBRATION
    if (ADC16_DoAutoCalibration(DEMO_ADC16_BASE) == kStatus_Success) {
        PRINTF("ADC calibration successful.\r\n");
    } else {
        PRINTF("ADC calibration failed.\r\n");
    }
#endif

    // Enable ADC interrupt
    EnableIRQ(ADC0_IRQn);
}

int main(void) {
    adc16_channel_config_t adc16ChannelConfigStruct;

    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("ADC and SW1, SW3 Example\r\n");

    // Initialize UART, ADC, and switch interrupt
    init_uart();
    init_adc();
    init_switch_interrupt();

    // Configure ADC channel
    adc16ChannelConfigStruct.channelNumber = DEMO_ADC16_USER_CHANNEL;
    adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = false; // Disable ADC interrupt

    // Initialize SysTick for timing
    SysTick_Config(SystemCoreClock / 1000U);

    // Start ADC initially
    adc_running = true;

    uint32_t last_adc_time = 0; // Thời điểm đọc ADC cuối cùng
    uint8_t last_volume = 0;    // Lưu giá trị volume trước đó


    while (1) {
        // Kiểm tra và xử lý nút nhấn
        if (sw1_pressed) {
            sw1_pressed = false; // Reset flag
            is_playing = !is_playing; // Chuyển trạng thái play/pause

            if (is_playing) {
                uart_send_string("play");
                PRINTF("Sent: play\r\n");
            } else {
                uart_send_string("pause");
                PRINTF("Sent: pause\r\n");
            }
        }

        if (sw3_pressed) {
            sw3_pressed = false; // Reset flag
            uart_send_string("SW3"); // Gửi chuỗi "SW3"
            PRINTF("Sent: stop\r\n");
        }
        if (sw2_pressed) {
            sw2_pressed = false;
            uart_send_string("next");
            PRINTF("Sent: next\r\n");
        }

        if (sw4_pressed) {
            sw4_pressed = false;
            uart_send_string("previous");
            PRINTF("Sent: previous\r\n");
        }

        // Kiểm tra thời gian để đọc ADC (mỗi 2000ms)
        if ((sys_tick_counter - last_adc_time) >= DELAY_MS) {
            last_adc_time = sys_tick_counter; // Cập nhật thời gian đọc ADC

            if (adc_running) {
                uint32_t adcSum = 0;

                // Đọc giá trị ADC trung bình
                for (int i = 0; i < AVERAGE_COUNT; i++) {
                    ADC16_SetChannelConfig(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP, &adc16ChannelConfigStruct);

                    while (0U == (kADC16_ChannelConversionDoneFlag &
                                  ADC16_GetChannelStatusFlags(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP))) {
                    }

                    adcSum += ADC16_GetChannelConversionValue(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP);
                }

                uint32_t avgAdcValue = adcSum / AVERAGE_COUNT;

                // Chuyển đổi giá trị ADC thành volume (0 - 128)
                float voltage = (avgAdcValue * VREFH) / ADC_MAX_VALUE;
                uint8_t volume = (uint8_t)((avgAdcValue * 128) / ADC_MAX_VALUE);

                if (abs(volume - last_volume) >= 1) { // Kiểm tra sự thay đổi volume
                last_volume = volume; // Cập nhật giá trị last_volume
                // Gửi volume và điện áp dưới dạng chuỗi
                char voltage_str[20];
                sprintf(voltage_str, "%.3f V", voltage);

                char volume_str[10];
                sprintf(volume_str, "%d", volume);
                uart_send_string(voltage_str);
                uart_send_string(volume_str);

                PRINTF("ADC Average Value: %4d\tVoltage: %s\tVolume: %s\r\n", avgAdcValue, voltage_str, volume_str);
            }
        }
    }
}
}

