/**
 *****************************************************************************
 * @file eeprom.c
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
#include "eeprom.h"

const struct device *EEPROM = DEVICE_DT_GET(DT_NODELABEL(i2c2));

void EEPROM_Comp(void)
{
	int ret = 0, i;
	uint8_t cmp_data[16];
	uint8_t data[16];

	if(!EEPROM_Init())
	{
		printk("EEPROM is not ready.\n");
		return;
	}

	/* Do one-byte read/write */
	/*
		data[0] = 0xAE;
		ret = write_bytes(i2c_dev, 0x00, &data[0], 1);
		if (ret) {
			printk("Error writing to FRAM! error code (%d)\n", ret);
			return;
		} else {
			printk("Wrote 0xAE to address 0x00.\n");
		}

		data[0] = 0x86;
		ret = write_bytes(i2c_dev, 0x01, &data[0], 1);
		if (ret) {
			printk("Error writing to FRAM! error code (%d)\n", ret);
			return;
		} else {
			printk("Wrote 0x86 to address 0x01.\n");
		}

		data[0] = 0x00;
		ret = read_bytes(i2c_dev, 0x00, &data[0], 1);
		if (ret) {
			printk("Error reading from FRAM! error code (%d)\n", ret);
			return;
		} else {
			printk("Read 0x%X from address 0x00.\n", data[0]);
		}
	*/
	data[0] = 0x00;
	ret = EEPROM_Read(0x00, &data[0], 1);
	if(ret)
	{
		printk("Error reading from FRAM! error code (%d)\n", ret);
		return;
	}
	else
	{
		printk("Read 0x%X from address 0x00.\n", data[0]);
	}

	/* Do multi-byte read/write */

	/* get some random data, and clear out data[] */
	for(i = 0; i < sizeof(cmp_data); i++)
	{
		cmp_data[i] = data[i] + i + 1; //k_cycle_get_32() & 0xFF;
		data[i] = 0x00;
	}

	/* write them to the FRAM */
	ret = EEPROM_Write(0x00, cmp_data, sizeof(cmp_data));
	if(ret)
	{
		printk("Error writing to EEPROM! error code (%d)\n", ret);
		return;
	}
	else
	{
		printk("Wrote %zu bytes to address 0x00.\n", sizeof(cmp_data));
	}

	ret = EEPROM_Read(0x00, data, sizeof(data));
	if(ret)
	{
		printk("Error reading from EEPROM! error code (%d)\n", ret);
		return;
	}
	else
	{
		printk("Read %zu bytes from address 0x00.\n", sizeof(data));
	}

	ret = 0;
	for(i = 0; i < sizeof(cmp_data); i++)
	{
		/* uncomment below if you want to see all the bytes */
		/* printk("0x%X ?= 0x%X\n", cmp_data[i], data[i]); */
		if(cmp_data[i] != data[i])
		{
			printk("Data comparison failed @ %d.\n", i);
			ret = -EIO;
		}
	}
	if(ret == 0)
	{
		printk("Data comparison successful.\n");
	}
}
