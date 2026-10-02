#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
	const struct device *driver = DEVICE_DT_GET(DT_NODELABEL(our_driver0));

	if (!device_is_ready(driver)) {
		LOG_ERR("%s is not ready", driver->name);
		return 0;
	}

	LOG_INF("%s is ready", driver->name);
	LOG_INF("Shell: sensor info %s | sensor fetch %s | sensor read %s",
		driver->name, driver->name, driver->name);

	/* ....the shell runs in its own thread. */
	return 0;
}