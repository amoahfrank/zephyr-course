#ifndef OUR_DRIVER_H_
#define OUR_DRIVER_H_

#include <stdbool.h>
#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Custom extension API: keep the LED on after channel_get.
 * Changes the "hold" field in the driver's runtime data.
 *
 * @return 0 on success, -EINVAL if dev is not an our,driver device.
 */
int our_driver_set_hold(const struct device *dev, bool hold);

#ifdef __cplusplus
}
#endif

#endif /* OUR_DRIVER_H_ */
