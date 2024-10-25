/**
 *****************************************************************************
 * @file watchdog.h
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
/**
 * @defgroup WATCHDOG
 * @brief
 * @{
 */

#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

#ifdef  __cplusplus
extern "C" {
 #endif		/* __cplusplus */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/watchdog.h>
#include <zephyr/sys/printk.h>
#include <stdbool.h>

int Watchdog_Init(void);
void Watchdog_Feed(void);

#ifdef __cplusplus
}
#endif	/* __cplusplus */
#endif  /* __WATCHDOG_H__ */

/**
  * @}
  */ /* WATCHDOG Module*/

