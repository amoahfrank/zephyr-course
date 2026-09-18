/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/init.h>
#include <zephyr/kernel.h>

static int heltec_scratch_v3_init(void)
{
	printk("Board Initialized\n");

	return 0;
}

SYS_INIT(heltec_scratch_v3_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);