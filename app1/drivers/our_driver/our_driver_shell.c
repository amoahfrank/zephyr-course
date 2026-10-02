#include <zephyr/shell/shell.h>
#include <zephyr/drivers/sensor.h>

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

SHELL_STATIC_SUBCMD_SET_CREATE(our_driver_subcmd,
	SHELL_CMD_ARG(fetch, NULL, "Fetch sample of my/our driver", cmd_sample_fetch_handler, 2, 0),
	SHELL_CMD_ARG(read, NULL, "Get channel of my/our driver", cmd_channel_get_handler, 2, 0),
	SHELL_CMD_ARG(info, NULL, "Print name and ready state of my/our driver", cmd_info_handler, 2, 0),
	SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &our_driver_subcmd, "Our driver set of commands", NULL);