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

const struct device *EXT_FLASH = DEVICE_DT_GET(DT_NODELABEL(spi2));

struct spi_config spi_cfg = {
	.frequency = 1250000U,
	.operation = SPI_WORD_SET(8) |SPI_MODE_CPOL |SPI_MODE_CPHA,
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
#define DUMMY_BYTE              0xA5    ///< Dummy byte


int ExtFlash_Init(void)
{
	if(!device_is_ready(EXT_FLASH))
	{
		printk("ExtFlash is not ready\n");
	    return EXT_FLASH_ERROR;

	}
	else
	{
		printk("ExtFlash is ready\n");
	}

	ExtFlash_WakeUP();

	return EXT_FLASH_OK;
}

int ExtFlash_ReadID(void)
{
	uint8_t cmd[1] = {RDID}, data[3] = {0};
	int ret;
	struct spi_buf bufs[] =
	{
		{
			.buf = cmd,
			.len = 1
		},
        {
		    .buf = data,
		    .len = 3
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

	ret = spi_transceive(EXT_FLASH, &spi_cfg, &tx, &rx);

	printk("ExtFlash_ReadID: %d,%02x,%02x,%02x\n", ret, data[0], data[1], data[2]);
	return ret;
}

void ExtFlash_WakeUP(void)
{

	uint8_t cmd[1] = {RDP};
	int ret;
	struct spi_buf bufs[] =
	{
		{
			.buf = cmd,
			.len = 1
		},

	};
	struct spi_buf_set tx =
	{
		.buffers = bufs,
		.count = 1
	};

	ret = spi_transceive(EXT_FLASH, &spi_cfg, &tx, NULL);
	printk("ExtFlash_WakeUP: %d\n", ret);

}

