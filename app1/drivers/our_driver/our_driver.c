#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#include "our_driver.h"

#define DT_DRV_COMPAT our_driver

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

/* Read-only, filled from the devicetree node */
struct our_driver_config {
	struct gpio_dt_spec led;
};

/* Runtime state, changes while the program runs */
struct our_driver_data {
	bool led_on;
	bool hold; /* changed by our_driver_set_hold() */
};

static int sample_fetch_my_impl(const struct device *dev,
				enum sensor_channel chan)
{
	const struct our_driver_config *cfg = dev->config;
	struct our_driver_data *data = dev->data;

	LOG_INF("Hello From Sample Fetch, channel %d -> LED ON", chan);

	data->led_on = true;
	return gpio_pin_set_dt(&cfg->led, 1);
}

static int channel_get_my_impl(const struct device *dev,
			       enum sensor_channel chan,
			       struct sensor_value *val)
{
	const struct our_driver_config *cfg = dev->config;
	struct our_driver_data *data = dev->data;

	/* Report what the last fetch did: 1 = LED was on */
	val->val1 = data->led_on ? 1 : 0;
	val->val2 = 0;

	if (data->hold) {
		LOG_INF("Hello From Channel Get, channel %d -> LED HELD ON", chan);
		return 0;
	}

	LOG_INF("Hello From Channel Get, channel %d -> LED OFF", chan);

	data->led_on = false;
	return gpio_pin_set_dt(&cfg->led, 0);
}

static DEVICE_API(sensor, our_driver_api) = {
	.sample_fetch = sample_fetch_my_impl,
	.channel_get = channel_get_my_impl,
};

/* Custom extension API (declared in our_driver.h) */
int our_driver_set_hold(const struct device *dev, bool hold)
{
	/* Only accept devices that belong to this driver */
	if (dev->api != &our_driver_api) {
		return -EINVAL;
	}

	struct our_driver_data *data = dev->data;

	data->hold = hold;
	LOG_INF("Hold %s", hold ? "ON" : "OFF");

	return 0;
}

static int init(const struct device *dev)
{
	const struct our_driver_config *cfg = dev->config;

	if (!gpio_is_ready_dt(&cfg->led)) {
		LOG_ERR("LED GPIO is not ready");
		return -ENODEV;
	}

	LOG_INF("Hello From Init");

	return gpio_pin_configure_dt(&cfg->led, GPIO_OUTPUT_INACTIVE);
}

#define OUR_DRIVER_DEFINE(inst)                                               \
	static struct our_driver_data data_##inst;                            \
	static const struct our_driver_config cfg_##inst = {                  \
		.led = GPIO_DT_SPEC_INST_GET(inst, gpios),                    \
	};                                                                    \
	DEVICE_DT_INST_DEFINE(inst, init, NULL, &data_##inst, &cfg_##inst,    \
			      POST_KERNEL, CONFIG_SENSOR_INIT_PRIORITY,       \
			      &our_driver_api);

DT_INST_FOREACH_STATUS_OKAY(OUR_DRIVER_DEFINE)
