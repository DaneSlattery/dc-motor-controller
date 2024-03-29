#ifndef OPT_ENC_H
#define OPT_ENC_H

#include "driver/gpio.h"
#include "driver/gptimer.h"
#include "freertos/FreeRTOS.h"
namespace peripherals
{
    class OptEncoder
    {
    private:
        gpio_num_t opt_in_pin;

        uint32_t count;
        const uint32_t num_slots = 20;

        gptimer_handle_t gptimer;
        uint64_t timer_count;

        uint32_t rpm;

        void init_hw();

        static void rise_isr(void *data);

    public:
        OptEncoder(gpio_num_t enc_pin);

        ~OptEncoder();

        int get_rotation_rate();
    };
}
#endif
