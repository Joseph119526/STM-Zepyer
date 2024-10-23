/**
 *****************************************************************************
 * @file i2c_bsp.c
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
#include "i2c_bsp.h"

int I2C_IsDeviceReady(const struct device *i2c_dev, uint8_t i2c_addr)
{
    struct i2c_msg msg;

    msg.buf = NULL;
	msg.len = 0;
	msg.flags = I2C_MSG_WRITE | I2C_MSG_STOP;
	return i2c_transfer(i2c_dev, &msg, 1, i2c_addr);
}

int I2C_Write(const struct device *i2c_dev, uint8_t i2c_addr, i2c_addr_size addr_size, uint16_t addr, uint8_t *data, uint32_t num_bytes)
{
	uint8_t wr_addr[2];
	struct i2c_msg msgs[2];

	/* address */
	if(addr_size == I2C_ADDR_SIZE_2BYTES)
	{
		wr_addr[0] = (addr >> 8) & 0xFF;
		wr_addr[1] = addr & 0xFF;
	}
	else
	{
		wr_addr[0] = addr & 0xFF;
	}

	/* Setup I2C messages */

	/* Send the address to write to */
	msgs[0].buf = wr_addr;
	msgs[0].len = (uint32_t)addr_size;
	msgs[0].flags = I2C_MSG_WRITE;

	/* Data to be written, and STOP after this. */
	msgs[1].buf = data;
	msgs[1].len = num_bytes;
	msgs[1].flags = I2C_MSG_WRITE | I2C_MSG_STOP;

	return i2c_transfer(i2c_dev, &msgs[0], 2, i2c_addr);

}

int I2C_Read(const struct device *i2c_dev, uint8_t i2c_addr, i2c_addr_size addr_size, uint16_t addr, uint8_t *data, uint32_t num_bytes)
{
	uint8_t wr_addr[2];
	struct i2c_msg msgs[2];

	/* address */
	if(addr_size == I2C_ADDR_SIZE_2BYTES)
	{
		wr_addr[0] = (addr >> 8) & 0xFF;
		wr_addr[1] = addr & 0xFF;
	}
	else
	{
		wr_addr[0] = addr & 0xFF;
	}

	/* Setup I2C messages */

	/* Send the address to write to */
	msgs[0].buf = wr_addr;
	msgs[0].len = (uint32_t)addr_size;
	msgs[0].flags = I2C_MSG_WRITE;

	/* Read from device. STOP after this. */
	msgs[1].buf = data;
	msgs[1].len = num_bytes;
	msgs[1].flags = I2C_MSG_RESTART | I2C_MSG_READ | I2C_MSG_STOP;

	return i2c_transfer(i2c_dev, &msgs[0], 2, i2c_addr);
}

