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
#include "ext_eeprom.h"

const struct device *EEPROM = DEVICE_DT_GET(DT_NODELABEL(i2c2));


int ExtEEPROM_ReadData(unsigned short ReadAddr, unsigned char* pBuffer,  unsigned short NumByteToRead)
{

	unsigned short  DevAddress = EXT_EEPOM_ADDR;
	unsigned short  MemAddress = ReadAddr;

#if EXT_EEPROM_MEMADDR_SIZE == EXT_EEPROM_MEMADDR_SIZE_8BIT
	DevAddress |= ((MemAddress >> 8) << 1);//DevAddress[2]=MemAddress[9],DevAddress[1]=MemAddress[8],
	MemAddress &= 0x00FF;
#endif

	if(EXT_EEPROM_I2C_Read(DevAddress, MemAddress, pBuffer, NumByteToRead))
	{
		return EXT_EEPROM_ERROR;
	}

	return EXT_EEPROM_OK;
}

static int ExtEEPROM_WaitForBusIdle(unsigned long timeout)
{
	while(timeout--)
	{
		if(!ExtEEPROM_IsDeviceReady())
		{
			return EXT_EEPROM_OK;
		}
		k_sleep(K_MSEC(1));
	}
	return EXT_EEPROM_TIMEOUT;
}

int ExtEEPROM_WriteData(unsigned short WriteAddr, unsigned char* pBuffer,  unsigned short NumByteToWrite)
{
	unsigned short  DevAddress;
	unsigned short  MemAddress;
	unsigned short NumByteCnt, Cnt, RemByteToWrite = NumByteToWrite;
	int ret;

	while(RemByteToWrite)
	{
		Cnt = EXT_EEPROM_PAGE_SIZE - WriteAddr % EXT_EEPROM_PAGE_SIZE;//1 page
		(RemByteToWrite > Cnt) ? (NumByteCnt = Cnt) : (NumByteCnt = RemByteToWrite);

		//Write Page
		DevAddress = EXT_EEPOM_ADDR;
		MemAddress = WriteAddr;

#if EXT_EEPROM_MEMADDR_SIZE == EXT_EEPROM_MEMADDR_SIZE_8BIT
		DevAddress |= ((MemAddress >> 8) << 1);//DevAddress[2]=MemAddress[9],DevAddress[1]=MemAddress[8],
		MemAddress &= 0x00FF;
#endif
		ret = EXT_EEPROM_I2C_Write(DevAddress, MemAddress, pBuffer, NumByteCnt);
		if(ret)
		{
			printk("ExtEEPROM_WriteData Error: %d\r\n", ret);
			return EXT_EEPROM_ERROR;
		}

		// Wait for EEPROM Standby state
		if(ExtEEPROM_WaitForBusIdle(EXT_EEPROM_WRITE_TIME))
		{
			return EXT_EEPROM_TIMEOUT;
		}

		/*!< if page not enught to write, change next page */
		RemByteToWrite -= NumByteCnt;
		WriteAddr = (WriteAddr & EXT_EEPROM_PAGE_MASK) + EXT_EEPROM_PAGE_SIZE;
		pBuffer += NumByteCnt;

		if(WriteAddr >= EXT_EEPROM_MEM_SIZE)
		{
			return EXT_EEPROM_ERROR;
		}
	}

	return EXT_EEPROM_OK;
}


void ExtEEPROM_Dump(void)
{
	unsigned int i, j = 0;
	unsigned char data;
	char string[100] = {0};

	EXT_EEPROM_DEBUG_PUTS("\r\nExtEEPROM Dump:\r\n");
	EXT_EEPROM_DEBUG_PUTS("   |x0 x1 x2 x3 x4 x5 x6 x7 x8 x9 xA xB xC xD xE xF\r\n");
	EXT_EEPROM_DEBUG_PUTS("---|-----------------------------------------------");

	for(i = 0; i < EXT_EEPROM_SIZE; i++)
	{
		if((i % 16 == 0))
		{
			if(j < 0x10)
			{
				sprintf(string, "\r\n0%Xx|", j);
			}
			else
			{
				sprintf(string, "\r\n%Xx|", j);
			}
			EXT_EEPROM_DEBUG_PUTS(string);
			j += 1;
		}

		ExtEEPROM_ReadData(i, &data, 1);

		if(data < 0x10)
		{
			sprintf(string, "0%X ", data);
		}
		else
		{
			sprintf(string, "%X ", data);
		}

		EXT_EEPROM_DEBUG_PUTS(string);
	}
}


void ExtEEPROM_Cmp(void)
{
	int ret = 0, i;
	uint8_t cmp_data[16];
	uint8_t data[16], data_base;

	ret = ExtEEPROM_IsDeviceReady();
	if(ret)
	{
		printk("EEPROM is not ready:%d\n", ret);
	}

	data[0] = 0x00;
	ret = ExtEEPROM_ReadData(0x00, &data[0], 1);
	if(ret)
	{
		printk("Error reading from FRAM! error code (%d)\n", ret);
		return;
	}
	else
	{
		printk("Read 0x%X from address 0x00.\n", data[0]);
	}

	data_base = data[0];

	/* Do multi-byte read/write */

	/* get some random data, and clear out data[] */
	for(i = 0; i < sizeof(cmp_data); i++)
	{
		cmp_data[i] = data_base + i + 1; //k_cycle_get_32() & 0xFF;
		data[i] = 0x00;
	}

	/* write them to the FRAM */
	ret = ExtEEPROM_WriteData(0x00, cmp_data, sizeof(cmp_data));
	if(ret)
	{
		printk("Error writing to EEPROM! error code (%d)\n", ret);
		return;
	}
	else
	{
		printk("Wrote %zu bytes to address 0x00.\n", sizeof(cmp_data));
	}

	ret = ExtEEPROM_ReadData(0x00, data, sizeof(data));
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
		//printk("0x%X ?= 0x%X\n", cmp_data[i], data[i]);
		if(cmp_data[i] != data[i])
		{
			printk("Data comparison failed @ %d:%02x\n", i, data[i]);
			ret = -EIO;
		}
	}
	if(ret == 0)
	{
		printk("Data comparison successful.\n");
	}
}

