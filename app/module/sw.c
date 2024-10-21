/**
 *****************************************************************************
 * @file sw.c
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

#include <stdio.h>

#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

#if !DT_NODE_HAS_PROP(DT_INST(0, peripheral), f2_gpios)
#error "Overlay for f2-gpios not properly defined."
#endif

const struct gpio_dt_spec SW = GPIO_DT_SPEC_GET_BY_IDX(DT_INST(0, peripheral), f2_gpios, 0);
static struct gpio_callback SW_cb;
static gpio_callback_handler_t SW_handler;

int SW_Init(gpio_callback_handler_t handler)
{
	int ret = 0;

	if(!gpio_is_ready_dt(&SW))
	{
		return 1;
	}

	ret = gpio_pin_configure_dt(&SW, GPIO_INPUT);
	if(ret)
	{
		return 2;
	}

	ret = gpio_pin_interrupt_configure_dt(&SW, GPIO_INT_EDGE_BOTH);
	if(ret)
	{
		return 2;
	}

	SW_handler = handler;

	gpio_init_callback(&SW_cb, SW_handler, BIT(SW.pin));

	ret = gpio_add_callback(SW.port, &SW_cb);
	if(ret)
	{
		return 3;
	}
	return 0;
}

int SW_Status(void)
{
	return gpio_pin_get_dt(&SW);
}

