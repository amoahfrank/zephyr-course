#include <zephyr/drivers/gpio.h>

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

int main(void)
{
    int ret;
    if (!gpio_is_ready_dt(&led)) {
        printk("Error: LED port not ready\n");
        return 0;
    }
    printk("port=%s, pin=%d\n", led.port->name, led.pin);
    ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        printk("Error: Failed to configure LED\n");
        return 0;
    }

    while (1) {
       ret = gpio_pin_toggle_dt(&led);
        if (ret < 0) {
            printk("Error: Failed to toggle LED\n");
        } else {
            printk("Toggled LED\n");
        }
        k_msleep(1000);
    }

}
//hardware heltec wireless stick lite v3 finally toggles the led on and off. I had to add hal_espressif to west.yml to get it to build.
//created app overlay with the right gpio configuration to correspond...
//checked with west espressif monitor -p COM9 to view the serial output. It works!