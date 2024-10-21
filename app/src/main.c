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
#if !DT_NODE_HAS_PROP(DT_INST(0, peripheral), f2_gpios)
#error "Overlay for f2-gpios not properly defined."
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


const struct gpio_dt_spec SW = GPIO_DT_SPEC_GET_BY_IDX(DT_INST(0, peripheral), f2_gpios, 0);
static struct gpio_callback SW_cb;

void SW_handler(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
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

int SW_Init(void)
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

int main(void)
{
	int ret = 0;

	printk("Hello World! %s\n", CONFIG_BOARD);

	ret = LED_Init();
	if(ret)
	{
		printk("LED Init Error:%d\n", ret);
	}

	ret = SW_Init();
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

	while(1)
	{
		printk("0.5 second. SW Status: %d\n", SW_Status());
		k_sleep(K_MSEC(500));
	}
	return 0;
}
