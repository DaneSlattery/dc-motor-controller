#include "rot_enc.h"

RotaryEncoder::RotaryEncoder(gpio_num_t input_a, gpio_num_t input_b)
    : in_a{input_a}, in_b{input_b}
{
    init_hw();
}

void IRAM_ATTR RotaryEncoder::isr_in_a(void *data)
{
    RotaryEncoder *rot_enc = (RotaryEncoder *)data;
    int a_level = gpio_get_level(rot_enc->in_a);
    int b_level = gpio_get_level(rot_enc->in_b);
    gpio_set_level(GPIO_LED, rot_enc->clk_val);

    if (a_level)
    {
        rot_enc->clk_val = a_level;
        if (a_level == b_level)
        {
            // clockwise
            rot_enc->dir_counter++;
            return;
        }

        rot_enc->dir_counter--;
    }
}

void RotaryEncoder::init_hw()
{
    gpio_config_t io_conf;
    // init
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = 1ULL << in_a;
    io_conf.intr_type = GPIO_INTR_ANYEDGE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = 1ULL << in_b;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);

    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = 1ULL << GPIO_LED;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(in_a, isr_in_a, this);

    gpio_set_level(GPIO_LED, 1);
}

int32_t RotaryEncoder::get_count()
{
    return dir_counter;
}
