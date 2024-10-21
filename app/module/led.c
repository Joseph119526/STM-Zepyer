/**
 *****************************************************************************
 * @file led.c
 * @brief
 * @author Joseph Hsu
 * @date 2024/10/21
 * @version
 *
 * @par History
 * <table>
 * <tr><th>Data        <th>Version <th>Author  <th>Description
 * <tr><td>            <td>        <td>        <td>
 * </table>
 *****************************************************************************
 */
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

#if !DT_NODE_HAS_PROP(DT_INST(0, peripheral), g2_gpios)
#error "Overlay for g2-gpios not properly defined."
#endif

const struct gpio_dt_spec LED = GPIO_DT_SPEC_GET_BY_IDX(DT_INST(0, peripheral), g2_gpios, 0);

int LED_Init(void)
{
	int ret = 0;

	if(!gpio_is_ready_dt(&LED))
	{
		return 1;
	}

	ret = gpio_pin_configure_dt(&LED, GPIO_OUTPUT);
	if(ret)
	{
		return 2;
	}

	return 0;
}

void LED_ON(void)
{
	gpio_pin_set(LED.port, LED.pin, 1);
}

void LED_OFF(void)
{
	gpio_pin_set(LED.port, LED.pin, 0);
}

