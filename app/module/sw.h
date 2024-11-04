/**
 *****************************************************************************
 * @file sw.h
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

int SW_Init(gpio_callback_handler_t handler);
int SW_Status(void);

