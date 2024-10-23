/**
 *****************************************************************************
 * @file i2c_bsp.h
 * @brief
 * @author Joseph Hsu
 * @date 2024/10/23
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
 * @defgroup I2C_BSP
 * @brief
 * @{
 */

#ifndef __I2C_BSP_H__
#define __I2C_BSP_H__

#ifdef  __cplusplus
extern "C" {
 #endif		/* __cplusplus */

#include <stdio.h>
#include <zephyr/devicetree.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>

typedef enum _i2c_addr_size
{
	I2C_ADDR_SIZE_1BYTE = 1,
	I2C_ADDR_SIZE_2BYTES = 2,
} i2c_addr_size;

int I2C_IsDeviceReady(const struct device *i2c_dev, uint8_t i2c_addr);
int I2C_Write(const struct device *i2c_dev, uint8_t i2c_addr, i2c_addr_size addr_size, uint16_t addr, uint8_t *data, uint32_t num_bytes);
int I2C_Read(const struct device *i2c_dev, uint8_t i2c_addr, i2c_addr_size addr_size, uint16_t addr, uint8_t *data, uint32_t num_bytes);

#define I2C_Reg8Write(dev,dev_addr, addr, data, len)  I2C_Write(dev, dev_addr, I2C_ADDR_SIZE_1BYTE, addr, data, len)
#define I2C_Reg8Read(dev,dev_addr, addr, data, len)   I2C_Read(dev, dev_addr, I2C_ADDR_SIZE_1BYTE, addr, data, len)
#define I2C_Reg16Write(dev,dev_addr, addr, data, len) I2C_Write(dev, dev_addr, I2C_ADDR_SIZE_2BYTES, addr, data, len)
#define I2C_Reg16Read(dev,dev_addr, addr, data, len)  I2C_Read(dev, dev_addr, I2C_ADDR_SIZE_2BYTES, addr, data, len)


#ifdef __cplusplus
}
#endif	/* __cplusplus */
#endif  /* __I2C_BSP_H__ */

/**
  * @}
  */ /* I2C_BSP Module*/


