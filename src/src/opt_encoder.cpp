
#include "opt_encoder.h"
#include "esp_timer.h"
#include <chrono>
OptEncoder::OptEncoder(gpio_num_t enc_pin)
    : opt_in_pin{enc_pin}
{
    init_hw();
}

void OptEncoder::init_hw()
{
    printf("init hw\n");
    gpio_config_t io_conf;
    // init
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = 1ULL << opt_in_pin;
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);
    printf("init gpio\n");

    gptimer = NULL;
    gptimer_config_t tim_conf;
    tim_conf.clk_src = GPTIMER_CLK_SRC_DEFAULT;
    tim_conf.direction = GPTIMER_COUNT_UP;
    tim_conf.resolution_hz = 1 * 1000 * 1000;
    ESP_ERROR_CHECK(gptimer_new_timer(&tim_conf, &gptimer));
    ESP_ERROR_CHECK(gptimer_enable(gptimer));
    ESP_ERROR_CHECK(gptimer_start(gptimer));
    printf("init timer\n");
    gpio_install_isr_service(0);
    gpio_isr_handler_add(opt_in_pin, rise_isr, this);
    printf("init isr\n");
}

void IRAM_ATTR OptEncoder::rise_isr(void *data)
{
    OptEncoder *opt = (OptEncoder *)data;

    opt->count++;
}

int OptEncoder::get_rpm()
{
    uint64_t raw_count;

    ESP_ERROR_CHECK(gptimer_get_raw_count(gptimer, &raw_count));
    printf("raw count = %llu us\n", raw_count);
    // auto time = std::chrono::duration_cast<std::chrono::milliseconds>(raw_count).count();
    // if (time == 0)
    // {
    //     gptimer_start(gptimer);
    //     return 0;
    // }
    // rpm = (6000 * count) / time; // 6000 = (10 slots * 60 seconds)
    count = 0;

    // gptimer_set_raw_count(gptimer, 0);
    return rpm;
}
