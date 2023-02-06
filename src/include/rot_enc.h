#ifndef ROT_ENC_H
#define ROT_ENC_H

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define GPIO_LED GPIO_NUM_2

class RotaryEncoder
{
private:
    gpio_num_t in_a;
    gpio_num_t in_b;
    int32_t dir_counter;

    void init_hw();
    static void isr_in_a(void *data);

public:
    RotaryEncoder(gpio_num_t input_a, gpio_num_t input_b);
    int32_t get_count();
    int clk_val;
};

#endif
