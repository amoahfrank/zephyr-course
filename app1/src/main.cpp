#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "our_driver.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

namespace {
constexpr int kStepMs = 1000;
constexpr int kHoldEvery = 5; /* toggle hold every 5 cycles */
}

int main(void)
{
	const struct device *driver = DEVICE_DT_GET(DT_NODELABEL(our_driver0));
	struct sensor_value val{};
	bool hold = false;
	int cycle = 0;

	if (!device_is_ready(driver)) {
		LOG_ERR("our_driver0 is not ready");
		return 0;
	}

	while (true) {
		/* Custom extension API: every 5 cycles, flip "hold" */
		if (cycle > 0 && cycle % kHoldEvery == 0) {
			hold = !hold;
			int ret = our_driver_set_hold(driver, hold);
			LOG_INF("Set hold ret %d", ret);
		}

		/* sample_fetch -> LED ON */
		int ret = sensor_sample_fetch(driver);
		LOG_INF("Fetch ret %d", ret);
		k_msleep(kStepMs);

		/* channel_get -> LED OFF (or stays on while hold is set) */
		ret = sensor_channel_get(driver, SENSOR_CHAN_AMBIENT_TEMP, &val);
		LOG_INF("Channel ret %d, val %d", ret, val.val1);
		k_msleep(kStepMs);

		cycle++;
	}

	return 0;
}