#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* Heartbeat LED, supplied by the app-led alias in app.overlay. */
#define APP_LED_NODE DT_ALIAS(app_led)

#if !DT_NODE_EXISTS(APP_LED_NODE)
#error "app-led alias is not defined - check app.overlay"
#endif

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(APP_LED_NODE, gpios);

int main(void)
{
    int ret;
    if (!gpio_is_ready_dt(&led)) {
        printk("Error: LED port not ready\n");
        return 0;
    }
    printk("port=%s, pin=%d, period=%d ms\n", led.port->name, led.pin,
           CONFIG_APP_HEARTBEAT_PERIOD_MS);
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
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    }

}
//hardware heltec wireless stick lite v3 finally toggles the led on and off. I had to add hal_espressif to west.yml to get it to build.
//created app overlay with the right gpio configuration to correspond...
//checked with west espressif monitor -p COM9 to view the serial output. It works!
//L4: heartbeat LED uses DT_ALIAS(app_led) and CONFIG_APP_HEARTBEAT_PERIOD_MS.
