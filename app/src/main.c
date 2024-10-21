/*
 * Copyright (c) 2023 Libre Solar Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>

#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>


#if !DT_NODE_HAS_PROP(DT_INST(0, peripheral), g2_gpios)
#error "Overlay for g2-gpios not properly defined."
#endif

const struct gpio_dt_spec gpio_g2 = GPIO_DT_SPEC_GET_BY_IDX(DT_INST(0, peripheral), g2_gpios, 0);

int main(void)
{
	int err;

	if(!gpio_is_ready_dt(&gpio_g2))
	{
		printk("GPIO pin is not ready.\n");
		return 0;
	}

	printk("Initializing GPIO pin.\n");

	err = gpio_pin_configure_dt(&gpio_g2, GPIO_OUTPUT_INACTIVE);
	if(err != 0)
	{
		printk("Configuring GPIO pin failed: %d\n", err);
		return 0;
	}

	while(1)
	{
		printk("0.5 second.\n");
		k_sleep(K_MSEC(500));
		err = gpio_pin_toggle_dt(&gpio_g2);
		if(err != 0)
		{
			printk("Setting GPIO pin goggle failed: %d\n", err);
		}
	}
	return 0;
}
