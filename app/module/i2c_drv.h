/**
 *****************************************************************************
 * @file i2c_drv.h
 * @brief
 * @author Joseph Hsu
 * @date 2024/10/22
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
#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>

int i2c_write_bytes(const struct device *i2c_dev, uint16_t i2c_addr, uint8_t addr, uint8_t *data, uint32_t num_bytes);
int i2c_read_bytes(const struct device *i2c_dev, uint16_t i2c_addr, uint8_t addr, uint8_t *data, uint32_t num_bytes);

