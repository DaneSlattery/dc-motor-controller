#include <stepper_motor.hpp>

namespace peripherals
{
    StepperMotor::StepperMotor(const gpio_num_t &stp_pin_num, const gpio_num_t &dir_pin_num)
        : stp_pin_num{stp_pin_num},
          dir_pin_num{dir_pin_num}
    {
        init_hw();
    }

    void StepperMotor::init_hw()
    {
        gpio_config_t io_conf;
        io_conf.mode = GPIO_MODE_OUTPUT;
        io_conf.pin_bit_mask = 1ULL << dir_pin_num;
        io_conf.intr_type = GPIO_INTR_DISABLE;
        io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
        io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
        gpio_config(&io_conf);

        mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, stp_pin_num);
        mcpwm_config_t pwm_config;
        pwm_config.frequency = 250; // hz
        pwm_config.cmpr_a = 0.0;
        pwm_config.cmpr_b = 0.0;
        pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
        pwm_config.counter_mode = MCPWM_UP_COUNTER;
        mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);
    }

    void StepperMotor::set_dir(const Direction &dir_)
    {
        dir = dir_;
    }

    const Direction &StepperMotor::get_dir() const
    {
        return dir;
    }

    void StepperMotor::set_duty(const float duty)
    {

        current_duty = duty;
        mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_GEN_A, duty);
        mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_GEN_A, MCPWM_DUTY_MODE_0);
    }

}
