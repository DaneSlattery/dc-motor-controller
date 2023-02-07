#include <stddef.h>
#include "driver/gpio.h"
#include "driver/gptimer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// #include "rot_enc.h"
#include "opt_encoder.h"

#define GPIO_ROT_A GPIO_NUM_25
#define GPIO_ROT_B GPIO_NUM_26
#define GPIO_OPT GPIO_NUM_27
// #define GPIO_STP_PIN_SEL (1ULL << GPIO_STP)

extern "C" void app_main();

void app_main()
{
	const TickType_t delay = 500 / portTICK_PERIOD_MS;
	// RotaryEncoder rot_enc{GPIO_ROT_A, GPIO_ROT_B};
	OptEncoder opt_enc{GPIO_OPT};
	while (1)
	{

		// Retrieve the timestamp at any time

		// printf("%ld, %d \n", rot_enc.get_count(), rot_enc.clk_val);
		opt_enc.get_rpm();
		// printf("%ld, %d \n", rot_enc.get_count(), rot_enc.clk_val);
		vTaskDelay(delay);
	}
}
