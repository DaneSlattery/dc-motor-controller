#ifndef STEPPER_MOTOR_ENC_H
#define STEPPER_MOTOR_ENC_H

#include "driver/gpio.h"
#include "driver/gptimer.h"
#include "freertos/FreeRTOS.h"
#include "driver/mcpwm.h"
namespace peripherals
{
    enum Direction
    {
        clockwise,
        anticlockwise
    };

    class StepperMotor
    {
    public:
        StepperMotor(const gpio_num_t &stp_pin_num, const gpio_num_t &dir_pin_num);

        void set_dir(const Direction &dir);
        const Direction &get_dir() const;

        void set_duty(const float duty_cycle);

    private:
        gpio_num_t stp_pin_num{GPIO_NUM_NC};
        gpio_num_t dir_pin_num{GPIO_NUM_NC};

        Direction dir{Direction::clockwise};
        uint16_t current_duty{0};

        void init_hw();
    };
}
#endif // STEPPER_MOTOR_ENC_H
