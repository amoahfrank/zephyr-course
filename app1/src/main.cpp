#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

namespace {
constexpr int kStepMs = 1000;
}

int main(void)
{
	const struct device *driver = DEVICE_DT_GET(DT_NODELABEL(our_driver0));
	struct sensor_value val{};

	if (!device_is_ready(driver)) {
		LOG_ERR("our_driver0 is not ready");
		return 0;
	}

	while (true) {
		/* sample_fetch -> LED ON */
		int ret = sensor_sample_fetch(driver);
		LOG_INF("Fetch ret %d", ret);
		k_msleep(kStepMs);

		/* channel_get -> LED OFF (the driver ignores which channel) */
		ret = sensor_channel_get(driver, SENSOR_CHAN_AMBIENT_TEMP, &val);
		LOG_INF("Channel ret %d, val %d", ret, val.val1);
		k_msleep(kStepMs);
	}

	return 0;
}
