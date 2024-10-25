/**
 *****************************************************************************
 * @file watchdog.c
 * @brief
 * @author Joseph Hsu
 * @date 2024/10/25
 * @version
 *
 * @par History
 * <table>
 * <tr><th>Data        <th>Version <th>Author  <th>Description
 * <tr><td>            <td>        <td>        <td>
 * </table>
 *****************************************************************************
 */
#include "watchdog.h"

/*
 * To use this sample the devicetree's /aliases must have a 'watchdog0' property.
 */
#if DT_HAS_COMPAT_STATUS_OKAY(st_stm32_window_watchdog)
#define WDT_MAX_WINDOW  100U
#endif

#ifndef WDT_MAX_WINDOW
#define WDT_MAX_WINDOW  1000U
#endif

#ifndef WDT_MIN_WINDOW
#define WDT_MIN_WINDOW  0U
#endif

int wdt_channel_id;
const struct device *const WATCHDOG = DEVICE_DT_GET(DT_ALIAS(watchdog0));
struct wdt_timeout_cfg wdt_config =
{
	/* Reset SoC when watchdog timer expires. */
	.flags = WDT_FLAG_RESET_SOC,

	/* Expire watchdog after max window */
	.window.min = WDT_MIN_WINDOW,
	.window.max = WDT_MAX_WINDOW,
    .callback = NULL,
};

int Watchdog_Init(void)
{
	int ret;
	if(!device_is_ready(WATCHDOG))
	{
		return 1;
	}

	wdt_channel_id = wdt_install_timeout(WATCHDOG, &wdt_config);
	printk("Watchdog Channel ID: %d\n",wdt_channel_id);

	if(wdt_channel_id == -ENOTSUP)
	{
		/* IWDG driver for STM32 doesn't support callback */
		printk("Callback support rejected, continuing anyway\n");
		wdt_config.callback = NULL;
		wdt_channel_id = wdt_install_timeout(WATCHDOG, &wdt_config);
	}
	if(wdt_channel_id < 0)
	{
		return 2;
	}

	ret = wdt_setup(WATCHDOG, WDT_OPT_PAUSE_HALTED_BY_DBG);
	if(ret)
	{
		return 3;
	}

#if WDT_MIN_WINDOW != 0
	/* Wait opening window. */
	k_msleep(WDT_MIN_WINDOW);
#endif
	return 0;
}

void Watchdog_Feed(void)
{
	wdt_feed(WATCHDOG, wdt_channel_id);
}
