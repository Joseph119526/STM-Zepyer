/*
 * Copyright (c) 2023 Libre Solar Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include "module.h"

static void SW_handler(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	uint8_t gpio_pin = 31 - __builtin_clz(pins);
	int ret = gpio_pin_get(dev, gpio_pin);

	if(ret)
	{
		LED_ON();
	}
	else
	{
		LED_OFF();
	}
}

int main(void)
{
	int ret = 0;


	printk("Hello World! %s\n", CONFIG_BOARD);

	ret = LED_Init();
	if(ret)
	{
		printk("LED Init Error:%d\n", ret);
	}

	ret = SW_Init(SW_handler);
	if(ret)
	{
		printk("SW Init Error:%d\n", ret);
	}

	if(SW_Status())
	{
		LED_ON();
	}
	else
	{
		LED_OFF();
	}

	ExtEEPROM_Cmp();
	ExtEEPROM_Dump();

	while(1)
	{
		k_sleep(K_MSEC(2000));
	}
	return 0;
}
