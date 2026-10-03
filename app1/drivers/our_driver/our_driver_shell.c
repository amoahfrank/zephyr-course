#include <stdlib.h>

#include <zephyr/shell/shell.h>
#include <zephyr/drivers/sensor.h>

#include "our_driver.h"

static int cmd_sample_fetch_handler(const struct shell *sh, size_t argc, char **argv)
{
	const struct device *dev = shell_device_get_binding(argv[1]);
	if (!dev) {
		shell_error(sh, "Could not find device %s", argv[1]);
		return -EFAULT;
	}

	int ret = sensor_sample_fetch(dev);

	if (ret != 0) {
		shell_error(sh, "Could not fetch sample, got %d", ret);
		return -EFAULT;
	}

	shell_info(sh, "Sample fetched");
	return 0;
}

static int cmd_channel_get_handler(const struct shell *sh, size_t argc, char **argv)
{
	const struct device *dev = shell_device_get_binding(argv[1]);
	if (!dev) {
		shell_error(sh, "Could not find device %s", argv[1]);
		return -EFAULT;
	}

	struct sensor_value val;
	int ret = sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &val);

	if (ret != 0) {
		shell_error(sh, "Could not fetch channel, got %d", ret);
		return -EFAULT;
	}

	shell_info(sh, "%d", val.val1);
	return 0;
}

static int cmd_info_handler(const struct shell *sh, size_t argc, char **argv)
{
	const struct device *dev = shell_device_get_binding(argv[1]);
	if (!dev) {
		shell_error(sh, "Could not find device %s", argv[1]);
		return -EFAULT;
	}

	shell_info(sh, "Device: %s", dev->name);
	shell_info(sh, "Ready: %s", device_is_ready(dev) ? "yes" : "no");
	return 0;
}

/* sensor set <device> <0|1> -> our_driver_set_hold() (L06 Task 2 extension API) */
static int cmd_set_handler(const struct shell *sh, size_t argc, char **argv)
{
	const struct device *dev = shell_device_get_binding(argv[1]);
	if (!dev) {
		shell_error(sh, "Could not find device %s", argv[1]);
		return -EFAULT;
	}

	/* SHELL_CMD_ARG(..., 2, 1) lets the value be omitted so we can report it here */
	if (argc < 3) {
		shell_error(sh, "Missing value, usage: sensor set <device> <0|1>");
		return -EINVAL;
	}

	char *end;
	long value = strtol(argv[2], &end, 10);

	if (end == argv[2] || *end != '\0') {
		shell_error(sh, "Value %s is not a number, expected 0 or 1", argv[2]);
		return -EINVAL;
	}

	if (value < 0 || value > 1) {
		shell_error(sh, "Value %ld out of range, expected 0 or 1", value);
		return -EINVAL;
	}

	int ret = our_driver_set_hold(dev, value == 1);

	if (ret != 0) {
		shell_error(sh, "Could not set hold, got %d", ret);
		return -EFAULT;
	}

	shell_info(sh, "Hold set to %ld", value);
	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(our_driver_subcmd,
	SHELL_CMD_ARG(fetch, NULL, "Fetch sample of my/our driver", cmd_sample_fetch_handler, 2, 0),
	SHELL_CMD_ARG(read, NULL, "Get channel of my/our driver", cmd_channel_get_handler, 2, 0),
	SHELL_CMD_ARG(info, NULL, "Print name and ready state of my/our driver", cmd_info_handler, 2, 0),
	SHELL_CMD_ARG(set, NULL, "Set hold of my/our driver: sensor set <device> <0|1>", cmd_set_handler, 2, 1),
	SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &our_driver_subcmd, "Our driver set of commands", NULL);