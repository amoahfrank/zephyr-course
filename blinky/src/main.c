/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* The devicetree node identifier for the "app-led" alias. */
#define APP_LED_NODE DT_ALIAS(app_led)

#if !DT_NODE_EXISTS(APP_LED_NODE)
#error "app-led alias is not defined - check app.overlay"
#endif

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(APP_LED_NODE, gpios);

int main(void)
{
	int ret;
	bool led_state = true;

	if (!gpio_is_ready_dt(&led)) {
		printf("Error: GPIO port not ready\n");
		return 0;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		printf("Error %d: failed to configure pin\n", ret);
		return 0;
	}

	printf("Heartbeat on %s pin %d, period %d ms\n",
	       led.port->name, led.pin, CONFIG_APP_HEARTBEAT_PERIOD_MS);

	while (1) {
		ret = gpio_pin_toggle_dt(&led);
		if (ret < 0) {
			printf("Error %d: failed to toggle LED\n", ret);
			return 0;
		}

		led_state = !led_state;
		printf("LED state: %s\n", led_state ? "ON" : "OFF");
		k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
	}
	return 0;
}