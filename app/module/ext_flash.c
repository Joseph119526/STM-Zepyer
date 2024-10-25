/**
 *****************************************************************************
 * @file ext_flash.c
 * @brief
 * @author Joseph Hsu
 * @date 2024/10/24
 * @version
 *
 * @par History
 * <table>
 * <tr><th>Data        <th>Version <th>Author  <th>Description
 * <tr><td>            <td>        <td>        <td>
 * </table>
 *****************************************************************************
 */
#include "ext_flash.h"
#include "common.h"

const struct device *EXT_FLASH = DEVICE_DT_GET(DT_NODELABEL(spi2));

struct spi_config spi_cfg =
{
	.frequency = 1250000U,
	.operation = SPI_WORD_SET(8) | SPI_MODE_CPOL | SPI_MODE_CPHA,
};


/*!< flash command define */
#define RDID                    0x9F    ///< Read identification,RDID: Read identification
#define EN4B			        0xB7	///< Enter 4 Byte Address Mode
#define EX4B        			0xE9	///< Exit 4 Byte Address Mode

#define READ                    0x03    ///< Read from Memory,READ: Read from Memory instruction
#define FAST_READ               0x0B    ///< Read Data Bytes at Higher Speed

#define PE                      0xDB    ///< Page Erase
#define SSE                     0x20    ///< SubSector Erase
#define SE                      0xD8    ///< Sector Erase,SE: Sector Erase instruction
//#define BE                      0xC7    ///< Bulk Erase,BE: Bulk Erase instruction
#define CE                      0xC7    ///< Chip Erase,CE: Chip Erase instruction

#define WRITE                   0x02    ///< Write to Memory instruction
#define PP                      0x02    ///< Page Program,WRITE: Write to Memory instruction

#define RDSR                    0x05    ///< Read Status Register, RDSR, Read Status Register instruction
#define WRSR                    0x01    ///< Write Status Register,WRSR: Write Status Register instruction

#define DP                      0xB9    ///< Deep Power-down
#define RDP                     0xAB    ///< Release from Deep Power-down
#define WREN                    0x06    ///< Write enable,WREN: Write enable instruction
#define WRDI                    0x04    ///< Write Disable

//#define WRSR1                   0x01    ///< Write Status Register instruction for 8MB
//#define WRSR2                   0x31    ///< Write Status Register instruction
//#define WRLR                    0x2C//0xE5    ///< Write to Lock Register
//#define RDLR                    0x2D//0xE8    ///< Read Lock Register

//#define PW                      0x0A    ///< Page Write

#define WIP_FLAG                0x01    ///< Write In Progress (WIP) flag
#define SET 1
#define DUMMY_BYTE              0xA5    ///< Dummy byte



static int ExtFlash_SPI_ReadReg(uint8_t cmd, void *data, size_t len)
{
	struct spi_buf bufs[] =
	{
		{
			.buf = &cmd,
			.len = 1
		},
		{
			.buf = data,
			.len = len
		},
	};

	struct spi_buf_set tx =
	{
		.buffers = bufs,
		.count = 1
	};

	struct spi_buf_set rx =
	{
		.buffers = bufs,
		.count = 2
	};

	return spi_transceive(EXT_FLASH, &spi_cfg, &tx, &rx);
}


static int ExtFlash_SPI_WriteReg(uint8_t cmd, void *data, size_t len)
{
	struct spi_buf bufs[] =
	{
		{
			.buf = &cmd,
			.len = 1
		},
		{
			.buf = data,
			.len = len
		},
	};


	struct spi_buf_set tx =
	{
		.buffers = bufs,
		.count = 1
	};

	if(len)
	{
		tx.count = 2;
	}

	return spi_transceive(EXT_FLASH, &spi_cfg, &tx, NULL);
}

/**
 * @brief  Read Flash ID.
 * @param  None
 * @return Id number
 * @note   None
 */
static unsigned long ExtFlash_ReadID(void)
{
	var32 temp;
	int ret;

	uint8_t data[3] = {0};
	temp.u32 = 0;

	ret = ExtFlash_SPI_ReadReg(RDID, data, sizeof(data));
	temp.u8.b0 = data[2];
	temp.u8.b1 = data[1];
	temp.u8.b2 = data[0];

	return temp.u32;
}

/**
 * @brief  Flash enable function
 * @param  None
 * @return None
 * @note   None
 */
static void ExtFlash_WriteEnable(void)
{
	ExtFlash_SPI_WriteReg(WREN, NULL, 0);
}

/**
 * @brief  Flash wait write end function
 * @param  None
 * @return None
 * @note   None
 */
static int ExtFlash_WaitForWriteEnd(unsigned long timeout)
{
	unsigned char status;

	/*!< Loop as long as the memory is busy with a write cycle */
	while(timeout--)
	{
		ExtFlash_SPI_ReadReg(RDSR, &status, sizeof(status));
		if(!((status & WIP_FLAG) == SET))
		{
			return EXT_FLASH_OK;
		}
		k_sleep(K_MSEC(1));
	}
	return EXT_FLASH_ERROR;
}

static void ExtFlash_DisadbleProtection(void)
{
	char data = 0x00;

	ExtFlash_WriteEnable();
	ExtFlash_SPI_WriteReg(WRSR, &data, 1);

	k_sleep(K_MSEC(20));//wait a while
}

static void ExtFlash_Enable4ByteAddressMode(void)
{
	ExtFlash_WriteEnable();
	ExtFlash_SPI_WriteReg(EN4B, NULL, 0);
}
/*
static void ExtFlash_Disable4ByteAddressMode(void)
{
	ExtFlash_SPI_WriteReg(EX4B,NULL,0);
}
*/

void ExtFlash_WakeUP(void)
{
	ExtFlash_SPI_WriteReg(RDP, NULL, 0);
	k_sleep(K_MSEC(1));//wait a while
}

void ExtFlash_DeepPowerDown(void)
{
	ExtFlash_SPI_WriteReg(DP, NULL, 0);
}

int ExtFlash_Init(void)
{
	if(!device_is_ready(EXT_FLASH))
	{
		return EXT_FLASH_ERROR;
	}

	ExtFlash_WakeUP();
	ExtFlash_DisadbleProtection();

	if(ExtFlash_ReadID() != EXT_FLASH_ID)
		return EXT_FLASH_ERROR;

	if(EXT_FLASH_MEM_SIZE > EXT_FLASH_MEM_SIZE_16M)
	{
		ExtFlash_Enable4ByteAddressMode();
	}
	/*else
	{
	    ExtFlash_Disable4ByteAddressMode();
	}*/

	return EXT_FLASH_OK;
}


/**
 * @brief	Erase Flash
 * @param	cmd			PE/SE/SSE/BE
 * @param	addr		Flash address
 * @return	none
 * @note
 */
static int ExtFlash_Erase(unsigned char cmd, unsigned long address, unsigned long timeout)
{
	var32 Addr;
	char addr_temp[4], addr_len;
	int ret = 0;
	Addr.u32 = address & 0xFFFFFFFF;
	/*!< Send write enable instruction */
	ExtFlash_WriteEnable();

	/*!< Sector Erase */
	/*!< Send Sector Erase instruction */

	if(EXT_FLASH_MEM_SIZE > EXT_FLASH_MEM_SIZE_16M)
	{
		addr_temp[0] = Addr.u8.b3;
		addr_temp[1] = Addr.u8.b2;
		addr_temp[2] = Addr.u8.b1;
		addr_temp[3] = Addr.u8.b0;
		addr_len = 4;
	}
	else
	{
		addr_temp[0] = Addr.u8.b2;
		addr_temp[1] = Addr.u8.b1;
		addr_temp[2] = Addr.u8.b0;
		addr_len = 3;
	}

	ret = ExtFlash_SPI_WriteReg(cmd, addr_temp, addr_len);
	if(ret)
	{
		return ret;
	}

	/*!< Wait the end of Flash writing */
	return ExtFlash_WaitForWriteEnd(timeout);
}

/**
 * @brief	SubSector Erase Flash
 * @param	addr		Flash address
 * @return	SPI_FLASH_OK/SPI_FLASH_ERR, See SPI ERROR CODE DEFINE
 * @note	Execute this function will start to erase flash, and
 *			you can check SPI status to check finished or not.
 */
int ExtFlash_SubSectorErase(unsigned long addr)
{
	if(EXT_FLASH_MEM_SIZE > EXT_FLASH_MEM_SIZE_16M)
	{
		addr = addr & 0xFFFFF000;
	}
	else
	{
		addr = addr & 0x00FFF000;
	}

	return ExtFlash_Erase(SSE, addr, EXT_FLASH_SSEC_ERASE_TIME);
}

/**
 * @brief	Chip Erase Flash
 * @param	Noun
 * @return	SPI_FLASH_OK/SPI_FLASH_ERR, See SPI ERROR CODE DEFINE
 * @note	Execute this function will start to erase flash, and
 *			you can check SPI status to check finished or not.
 */
int ExtFlash_ChipErase(void)
{
	int ret;

	ExtFlash_WriteEnable();

	ret = ExtFlash_SPI_WriteReg(CE, NULL, 0);
	if(ret)
	{
		return ret;
	}

	return ExtFlash_WaitForWriteEnd(EXT_FLASH_CHIP_ERASE_TIME);;
}

static int ExtFlash_SPI_ReadData(unsigned long ReadAddr, unsigned char* pBuffer, unsigned short NumByteToRead)
{
	var32 Addr;
	char access[5], access_len;

	Addr.u32 = ReadAddr & 0xFFFFFFFF;

	access[0] = READ;// command
	if(EXT_FLASH_MEM_SIZE > EXT_FLASH_MEM_SIZE_16M) //address
	{
		access[1] = Addr.u8.b3;
		access[2] = Addr.u8.b2;
		access[3] = Addr.u8.b1;
		access[4] = Addr.u8.b0;
		access_len = 5;
	}
	else
	{
		access[1] = Addr.u8.b2;
		access[2] = Addr.u8.b1;
		access[3] = Addr.u8.b0;
		access_len = 4;
	}

	struct spi_buf bufs[] =
	{
		{
			.buf = access,
			.len = access_len
		},
		{
			.buf = pBuffer,
			.len = NumByteToRead
		},
	};

	struct spi_buf_set tx =
	{
		.buffers = bufs,
		.count = 1
	};

	struct spi_buf_set rx =
	{
		.buffers = bufs,
		.count = 2
	};

	return spi_transceive(EXT_FLASH, &spi_cfg, &tx, &rx);
}


/**
 * @brief  Page write function.
 * @param  pBuffer: data buffer
 * @param  WriteAddr: flash address
 * @param  NumByteToWrite: numbers byte to write
 * @return None
 * @note   None
 */
static int ExtFlash_SPI_WritePageData(unsigned long WriteAddr, unsigned char* pBuffer, unsigned short NumByteToWrite)
{
	var32 Addr;
	char access[5], access_len;
	int ret;

	Addr.u32 = WriteAddr & 0xFFFFFFFF;

	ExtFlash_WriteEnable();

	access[0] = WRITE;// command
	if(EXT_FLASH_MEM_SIZE > EXT_FLASH_MEM_SIZE_16M) //address
	{
		access[1] = Addr.u8.b3;
		access[2] = Addr.u8.b2;
		access[3] = Addr.u8.b1;
		access[4] = Addr.u8.b0;
		access_len = 5;
	}
	else
	{
		access[1] = Addr.u8.b2;
		access[2] = Addr.u8.b1;
		access[3] = Addr.u8.b0;
		access_len = 4;
	}

	struct spi_buf bufs[] =
	{
		{
			.buf = access,
			.len = access_len
		},
		{
			.buf = pBuffer,
			.len = NumByteToWrite
		},
	};
	struct spi_buf_set tx =
	{
		.buffers = bufs,
		.count = 2
	};

	ret = spi_transceive(EXT_FLASH, &spi_cfg, &tx, NULL);
	if(ret)
	{
		return ret;
	}

	/*!< Wait the end of Flash writing */
	return ExtFlash_WaitForWriteEnd(EXT_FLASH_PAGE_PROGRAM_TIME);;
}

/**
 * @brief  Write a buffer to flash.
 * @param  pBuffer: data buffer.
 * @param  WriteAddr: flash address.
 * @param  NumbByteToWrite: number bytes write to flash.
 * @return flash write status.
 * @note   None
 */

//#define EXT_FLASH_WRITE_CHECK
#ifdef EXT_FLASH_WRITE_CHECK
unsigned char ExtFlash_Buf[EXT_FLASH_PAGE_SIZE];
unsigned short ext_flash_write_retry = 0;
#endif

int ExtFlash_WriteData(unsigned long WriteAddr, unsigned char* pBuffer, unsigned short NumByteToWrite)
{
	//u16 NumByteCnt, Cnt;
	//u32 ix;
	unsigned short NumByteCnt, Cnt, RemByteToWrite = NumByteToWrite;
#ifdef EXT_FLASH_WRITE_CHECK
	unsigned char retry = 1;
#endif
	while(RemByteToWrite)
	{
		/*!< if address biger 0x200000 address, the flash size not into any data */
		if(WriteAddr >= EXT_FLASH_MEM_SIZE)
		{
			return EXT_FLASH_ERROR;
		}

		Cnt = EXT_FLASH_PAGE_SIZE - WriteAddr % EXT_FLASH_PAGE_SIZE;//1 page ,256byte
		(RemByteToWrite > Cnt) ? (NumByteCnt = Cnt) : (NumByteCnt = RemByteToWrite);
#ifdef EXT_FLASH_WRITE_CHECK
	retry_tag:
#endif
		if(ExtFlash_SPI_WritePageData(WriteAddr, pBuffer, NumByteCnt) != EXT_FLASH_OK)
		{
			return EXT_FLASH_ERROR;
		}

		//read back and check
#ifdef EXT_FLASH_WRITE_CHECK
		ExtFlash_SPI_ReadData(WriteAddr, ExtFlash_Buf, NumByteCnt);
		if(memcmp(ExtFlash_Buf, pBuffer, NumByteCnt) != 0)
		{
			if(retry)
			{
				ext_flash_write_retry++;
				retry--;
				goto retry_tag;
			}
			else
			{
				return EXT_FLASH_ERROR;
			}
		}
#endif

		/*!< if page not enught to write, change next page */
		//ix += NumByteCnt;
		RemByteToWrite -= NumByteCnt;
		WriteAddr = (WriteAddr & EXT_FLASH_PAGE_MASK) + EXT_FLASH_PAGE_SIZE;
		pBuffer += NumByteCnt;
	}

	return EXT_FLASH_OK;
}

int ExtFlash_ReadData(unsigned long ReadAddr, unsigned char* pBuffer,  unsigned short NumByteToRead)
{
	return ExtFlash_SPI_ReadData(ReadAddr, pBuffer, NumByteToRead);
}

void ExtFlash_Dump(unsigned long ReadAddr,  unsigned short NumByteToRead)
{
	unsigned int i, j = 0;
	unsigned char data;
	char string[100] = {0};
	unsigned long addr = ReadAddr & 0xFFFFFFF0;

	EXT_FLASH_DEBUG_PRINT("\r\nExtFlash Dump: 0x%08lX,%d\r\n", addr, NumByteToRead);
	EXT_FLASH_DEBUG_PUTS("   |x0 x1 x2 x3 x4 x5 x6 x7 x8 x9 xA xB xC xD xE xF\r\n");
	EXT_FLASH_DEBUG_PUTS("---|-----------------------------------------------");

	for(i = 0; i < NumByteToRead; i++)
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
			EXT_FLASH_DEBUG_PUTS(string);
			j += 1;
		}

		ExtFlash_ReadData(addr + i, &data, 1);

		if(data < 0x10)
		{
			sprintf(string, "0%X ", data);
		}
		else
		{
			sprintf(string, "%X ", data);
		}

		EXT_FLASH_DEBUG_PUTS(string);
	}
	EXT_FLASH_DEBUG_PUTS("\r\n");
}


void ExtFlash_Cmp(void)
{
	int ret = 0, i;
	uint8_t cmp_data[16];
	uint8_t data[16], data_base;

	printk("ExtFlash_Cmp:\n");

	data[0] = 0x00;
	ret = ExtFlash_ReadData(0x00, &data[0], 1);
	if(ret)
	{
		printk("ExtFlash_ReadData Error:%d\n", ret);
		return;
	}
	else
	{
		printk("Addr[0x00] = 0x%02X\n", data[0]);
	}

	data_base = data[0];

	/* Do multi-byte read/write */

	/* get some random data, and clear out data[] */
	for(i = 0; i < sizeof(cmp_data); i++)
	{
		cmp_data[i] = data_base + i + 1; //k_cycle_get_32() & 0xFF;
		data[i] = 0x00;
	}

	ret = ExtFlash_SubSectorErase(0x00);
	if(ret)
	{
		printk("ExtFlash_SubSectorErase error:%d\n", ret);
		return;
	}

	ret = ExtFlash_WriteData(0x00, cmp_data, sizeof(cmp_data));
	if(ret)
	{
		printk("ExtFlash_WriteData error:%d\n", ret);
		return;
	}

	ret = ExtFlash_ReadData(0x00, data, sizeof(data));
	if(ret)
	{
		printk("ExtFlash_ReadData error:%d\n", ret);
		return;
	}

	ret = 0;
	for(i = 0; i < sizeof(cmp_data); i++)
	{
		/* uncomment below if you want to see all the bytes */
		//printk("0x%X ?= 0x%X\n", cmp_data[i], data[i]);
		if(cmp_data[i] != data[i])
		{
			printk("Data comparison failed @ %d:%02X\n", i, data[i]);
			ret = -EIO;
		}
	}
	if(ret == 0)
	{
		printk("Data comparison successful.\n");
	}
}

