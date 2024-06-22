#include "gd32vf103.h"
#include "adc.h"
#include "delay.h"
#include <stdio.h>
#include <stdbool.h>

#define SENSOR1_PORT            GPIOA
#define SENSOR1_PIN             GPIO_PIN_0
#define SENSOR1_CHANNEL         ADC_CHANNEL_0

#define SENSOR2_PORT            GPIOA
#define SENSOR2_PIN             GPIO_PIN_1
#define SENSOR2_CHANNEL         ADC_CHANNEL_1

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

#define THRESHOLD_CURRENT       20  // Threshold current level for turning on the LED

void setup_gpio(void);
void check_button();

int main(void) {
    uint16_t current1, current2;

    /* Setup GPIO */
    setup_gpio();
    /* Initialize ADC */
    init_adc();

    while (1) {
        /* Read current from sensor 1 */
        adc_regular_channel_config(ADC0, 0, SENSOR1_CHANNEL, ADC_SAMPLETIME_13POINT5);
        adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
        while(!adc_flag_get(ADC0, ADC_FLAG_EOC)); //wait for conversion
        delay_1ms(50);
        current1 = ADC_RDATA(ADC0);
        adc_regular_channel_config(ADC0, 0, SENSOR2_CHANNEL, ADC_SAMPLETIME_13POINT5);
        adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
        while(!adc_flag_get(ADC0, ADC_FLAG_EOC)); //wait for conversion
        delay_1ms(50);
        current2 = ADC_RDATA(ADC0);


        if (current1>= THRESHOLD_CURRENT){
            gpio_bit_write(LED_SENSOR1_PORT, LED_SENSOR1_PIN,1);
        }
        else{
            gpio_bit_write(LED_SENSOR1_PORT, LED_SENSOR1_PIN, 0);
        }
        if (current2>= THRESHOLD_CURRENT){
            gpio_bit_write(LED_SENSOR2_PORT,LED_SENSOR2_PIN,1);
        }
        else{
            gpio_bit_write(LED_SENSOR2_PORT, LED_SENSOR2_PIN,0);
        }
    
        delay_1ms(100); // Add a delay to avoid unnecessary CPU load
        check_button();
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

void check_button(void) {
    if (gpio_input_bit_get(BUTTON1_PORT, BUTTON1_PIN) == 0) {
        gpio_bit_set(LED_BUTTON1_PORT, LED_BUTTON1_PIN);
    } else {
        gpio_bit_reset(LED_BUTTON1_PORT, LED_BUTTON1_PIN);
    }

    if (gpio_input_bit_get(BUTTON2_PORT, BUTTON2_PIN) == 0) {
        gpio_bit_set(LED_BUTTON2_PORT, LED_BUTTON2_PIN);
    } else {
        gpio_bit_reset(LED_BUTTON2_PORT, LED_BUTTON2_PIN);
    }

    if (gpio_input_bit_get(BUTTON3_PORT, BUTTON3_PIN) == 0) {
        gpio_bit_set(LED_BUTTON3_PORT, LED_BUTTON3_PIN);
    } else {
        gpio_bit_reset(LED_BUTTON3_PORT, LED_BUTTON3_PIN);
    }

    if (gpio_input_bit_get(BUTTON4_PORT, BUTTON4_PIN) == 0) {
        gpio_bit_set(LED_BUTTON4_PORT, LED_BUTTON4_PIN);
    } else {
        gpio_bit_reset(LED_BUTTON4_PORT, LED_BUTTON4_PIN);
    }
}
