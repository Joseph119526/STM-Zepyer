/**
 *****************************************************************************
 * @file eeprom.h
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
#include "i2c_drv.h"

#define EEPROM_I2C_ADDR	 (0xA0>>1)
extern const struct device *EEPROM;

#define EEPROM_Init()              device_is_ready(EEPROM)
#define EEPROM_Write(addr,buf,len) i2c_write_bytes(EEPROM,EEPROM_I2C_ADDR,addr,buf,len)
#define EEPROM_Read(addr,buf,len)  i2c_read_bytes(EEPROM,EEPROM_I2C_ADDR,addr,buf,len)

void EEPROM_Comp(void);


