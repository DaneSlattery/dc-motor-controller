/*
 * Copyright (c) 2006-2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

#include "opt_encoder.hpp"
#include "pid.hpp"

#define TX_PIN PA_2
#define RX_PIN PA_3
#define POT_PIN PA_0
// interface to l289n motor driver
#define PWM_OUT_PIN PA_8 // so far, A1 and B0 are DEAD for PWM
#define MOT_DIR_1 PA_9
#define MOT_DIR_2 PA_10
#define PWM_PERIOD_US 100 // 20kHz

// from experimental data:
/*
* If the duty cycle is below 10%, it should drop to 0 % to shutdown the motor.
The motor will stall somewhere here.
If we want to start up the motor, the minimum speed should be 10%, ramping up from there.
*/
#define MOTOR_STALL_PWM 0.1
#define MAX_MOTOR_PWM 0.9 // max pwm why again?

#define OPT_ENCODER_PIN PA_11
#define OPT_ENC_STAT_LED PC_13

// Create a BufferedSerial object to be used by the system I/O retarget code.
static BufferedSerial serial_port(TX_PIN, RX_PIN, 9600);

AnalogIn pot_analog_in(POT_PIN);

PwmOut motor_pwm(PWM_OUT_PIN);
DigitalOut motor_dir_forward(MOT_DIR_1);
DigitalOut motor_dir_rev(MOT_DIR_2);

int main()
{
	// initialise ports
	// motor off, but ready to go forward
	motor_dir_forward.write(1);
	motor_dir_rev.write(0);
	motor_pwm.period_us(PWM_PERIOD_US);
	motor_pwm.write(0.0f);

	// set up serial port
	serial_port.set_format(
		/* bits */ 8,
		/* parity */ BufferedSerial::Even,
		/* stop bit */ 1);

	// initialise variables
	float pot_value = 0.0;
	float motor_duty_cycle = 0.0; // [0, 1]
	double target_rpm{}, current_rpm{};

	OptEncoder my_enc{OPT_ENCODER_PIN};

	PID rpm_pid{0.01, 0, 0};

	while (1)
	{
		// read the potentiometer value
		pot_value = pot_analog_in.read();
		target_rpm = pot_value * 100; // random scale

		// read the current rpm
		current_rpm = my_enc.read();
		// calculate the PID change
		double output_pwm = rpm_pid.update(target_rpm, current_rpm);

		// set the output
		motor_duty_cycle = output_pwm;

		// clamp the duty cycle below 90%
		motor_duty_cycle = fmin(motor_duty_cycle, MAX_MOTOR_PWM);
		// if it's less than 10%, set it to 0% to prevent stall current
		if (motor_duty_cycle < MOTOR_STALL_PWM)
			motor_duty_cycle = 0.0;

		motor_pwm.write(motor_duty_cycle);
		printf("pot: %.3f, target_rpm: %.3f, current_rpm: %.3f, motor_duty: %.3f  \n", pot_value, target_rpm, current_rpm, motor_duty_cycle);

		thread_sleep_for(50);
	}
}

// use this so we can use printf to debug to the serial buf
FileHandle *mbed::mbed_override_console(int fd)
{
	return &serial_port;
}
