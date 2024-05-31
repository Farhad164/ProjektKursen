#include "gd32vf103.h"
#include "adc.h"
#include "delay.h"
#include <stdio.h>

#define SENSOR1_PORT            GPIOA
#define SENSOR1_PIN             GPIO_eeuuPIN_0
#define SENSOR1_CHANNEL         ADC_CHANNEL_3

#define SENSOR2_PORT            GPIOA
#define SENSOR2_PIN             GPIO_PIN_1
#define SENSOR2_CHANNEL         ADC_CHANNEL_4

#define LED_SENSOR1_PORT        GPIOB
#define LED_SENSOR1_PIN         GPIO_PIN_0

#define LED_SENSOR2_PORT        GPIOB
#define LED_SENSOR2_PIN         GPIO_PIN_1

#define LED_BUTTON1_PORT        GPIOB
#define LED_BUTTON1_PIN         GPIO_PIN_2

#define LED_BUTTON2_PORT        GPIOB
#define LED_BUTTON2_PIN         GPIO_PIN_10

#define LED_BUTTON3_PORT        GPIOB
#define LED_BUTTON3_PIN         GPIO_PIN_11

#define LED_BUTTON4_PORT        GPIOB
#define LED_BUTTON4_PIN         GPIO_PIN_12

#define BUTTON1_PORT            GPIOA
#define BUTTON1_PIN             GPIO_PIN_2

#define BUTTON2_PORT            GPIOA
#define BUTTON2_PIN             GPIO_PIN_3

#define BUTTON3_PORT            GPIOA
#define BUTTON3_PIN             GPIO_PIN_4

#define BUTTON4_PORT            GPIOA
#define BUTTON4_PIN             GPIO_PIN_5


#define THRESHOLD_CURRENT       126  // Threshold current level for turning on the LED

void setup_gpio(void);
void init_adc(void);
uint16_t read_current(uint32_t channel);
void check_buttons(void);

int main(void) {
    uint16_t current1, current2;

    /* Setup GPIO */
    setup_gpio();

    /* Initialize ADC */
    init_adc();

    while(1) {
        /* Read current from sensor 1 */
        current1 = read_current(SENSOR1_CHANNEL);

        /* Read current from sensor 2 */
        current2 = read_current(SENSOR2_CHANNEL);


        /* Check if the current is above the threshold and set LEDs accordingly */
        if(read_current(SENSOR1_CHANNEL) >= THRESHOLD_CURRENT) {
            gpio_bit_set(LED_SENSOR1_PORT, LED_SENSOR1_PIN);  // Turn LED1 on
        } else {
            gpio_bit_reset(LED_SENSOR1_PORT, LED_SENSOR1_PIN);  // Turn LED1 off
        }
        if(read_current(SENSOR2_CHANNEL) >= THRESHOLD_CURRENT) {
            gpio_bit_set(LED_SENSOR2_PORT, LED_SENSOR2_PIN);  // Turn LED2 on
        } else {
            gpio_bit_reset(LED_SENSOR2_PORT, LED_SENSOR2_PIN);  // Turn LED2 off
        }

        /* Check buttons */
        check_buttons();

        delay_1ms(100); // Add a delay to avoid unnecessary CPU load
    }

    return 0;
}

void setup_gpio(void) {
    /* Enable clocks for GPIO ports */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);

    /* Initialize sensor pins as analog input */
    gpio_init(SENSOR1_PORT, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, SENSOR1_PIN);
    gpio_init(SENSOR2_PORT, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, SENSOR2_PIN);

    /* Initialize LED pins as push-pull output */
    gpio_init(LED_SENSOR1_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_SENSOR1_PIN);
    gpio_init(LED_SENSOR2_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_SENSOR2_PIN);
    gpio_init(LED_BUTTON1_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_BUTTON1_PIN);
    gpio_init(LED_BUTTON2_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_BUTTON2_PIN);
    gpio_init(LED_BUTTON3_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_BUTTON3_PIN);
    gpio_init(LED_BUTTON4_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_BUTTON4_PIN);

    /* Initialize button pins as input with pull-up */
    gpio_init(BUTTON1_PORT, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, BUTTON1_PIN);
    gpio_init(BUTTON2_PORT, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, BUTTON2_PIN);
    gpio_init(BUTTON3_PORT, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, BUTTON3_PIN);
    gpio_init(BUTTON4_PORT, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, BUTTON4_PIN);
}

void init_adc(void) {
    /* Enable clock for ADC and associated GPIOs */
    rcu_periph_clock_enable(RCU_ADC0);

    /* ADC Configuration */
    adc_deinit(ADC0);
    adc_mode_config(ADC_MODE_FREE);
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, ENABLE);
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 1);
    adc_regular_channel_config(ADC0, 0, SENSOR1_CHANNEL, ADC_SAMPLETIME_13POINT5);
    adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_EXTTRIG_REGULAR_NONE);
    adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);

    /* Enable and calibrate ADC */
    adc_enable(ADC0);
    delay_1ms(1);
    adc_calibration_enable(ADC0);

    /* Start ADC conversion */
    adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
}

uint16_t read_current(uint32_t channel) {
    adc_regular_channel_config(ADC0, 0, channel, ADC_SAMPLETIME_55POINT5);
    adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
    while (!adc_flag_get(ADC0, ADC_FLAG_EOC));
    return adc_regular_data_read(ADC0);
}

void check_buttons(void) {
    if (gpio_input_bit_get(BUTTON1_PORT, BUTTON1_PIN) == RESET) {
        gpio_bit_set(LED_BUTTON1_PORT, LED_BUTTON1_PIN);
    } else {
        gpio_bit_reset(LED_BUTTON1_PORT, LED_BUTTON1_PIN);
    }

    if (gpio_input_bit_get(BUTTON2_PORT, BUTTON2_PIN) == RESET) {
        gpio_bit_set(LED_BUTTON2_PORT, LED_BUTTON2_PIN);
    } else {
        gpio_bit_reset(LED_BUTTON2_PORT, LED_BUTTON2_PIN);
    }

    if (gpio_input_bit_get(BUTTON3_PORT, BUTTON3_PIN) == RESET) {
        gpio_bit_set(LED_BUTTON3_PORT, LED_BUTTON3_PIN);
    } else {
        gpio_bit_reset(LED_BUTTON3_PORT, LED_BUTTON3_PIN);
    }

    if (gpio_input_bit_get(BUTTON4_PORT, BUTTON4_PIN) == RESET) {
        gpio_bit_set(LED_BUTTON4_PORT, LED_BUTTON4_PIN);
    } else {
        gpio_bit_reset(LED_BUTTON4_PORT, LED_BUTTON4_PIN);
    }
}
