/**
 *****************************************************************************
 * @file ext_flash.h
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
/**
 * @defgroup EXT_FLASH
 * @brief
 * @{
 */

#ifndef __EXT_FLASH_H__
#define __EXT_FLASH_H__

#ifdef  __cplusplus
extern "C" {
 #endif		/* __cplusplus */

#include <stdio.h>
#include <zephyr/devicetree.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/spi.h>

#define SIZE_1K				0x400
#define SIZE_1M				0x100000

//#define EXT_FLASH_MX25L6406
//#define EXT_FLASH_AT25DF641
//#define EXT_FLASH_M25PE16
//#define EXT_FLASH_W25Q16JV
#define EXT_FLASH_W25Q32BV
//#define EXT_FLASH_MT25QL256A
//#define EXT_FLASH_MX25L51245G

#ifdef EXT_FLASH_M25PE16
#define EXT_FLASH_MEM_SIZE			  (2 * SIZE_1M)			///< Flash Size: 16Mb = 2MB
#define EXT_FLASH_ID				  (0x208015)				///<M25P64 ID
#define EXT_FLASH_PAGE_PROGRAM_TIME   (3)//
#define EXT_FLASH_SSEC_ERASE_TIME     (300) //4KB
#define EXT_FLASH_SEC_ERASE_TIME      (15000)//64KB
#define EXT_FLASH_CHIP_ERASE_TIME     (80000) //chip erase
#endif

#ifdef EXT_FLASH_W25Q16JV
#define EXT_FLASH_MEM_SIZE		      (2 * SIZE_1M)			///< Flash Size: 16Mb = 2MB
#define EXT_FLASH_ID	              (0xEF4015)
#define EXT_FLASH_PAGE_PROGRAM_TIME   (3)//
#define EXT_FLASH_SSEC_ERASE_TIME     (400) //4KB
#define EXT_FLASH_SEC_ERASE_TIME      (1600)//64KB
#define EXT_FLASH_CHIP_ERASE_TIME     (25000) //chip erase
#endif

#ifdef EXT_FLASH_W25Q32BV
#define EXT_FLASH_MEM_SIZE		      (4 * SIZE_1M)			///< Flash Size: 32Mb = 4MB
#define EXT_FLASH_ID	              (0xEF4016)
#define EXT_FLASH_PAGE_PROGRAM_TIME   (3)//
#define EXT_FLASH_SSEC_ERASE_TIME     (300) //4KB
#define EXT_FLASH_SEC_ERASE_TIME      (1500)//64KB
#define EXT_FLASH_CHIP_ERASE_TIME     (80000) //chip erase
#endif

#ifdef EXT_FLASH_AT25DF641
#define EXT_FLASH_MEM_SIZE			  (8 * SIZE_1M)			///< Flash Size: 64Mb = 8MB
#define EXT_FLASH_ID				  (0x1F4800)				///<AT25DF641 ID
#define EXT_FLASH_PAGE_PROGRAM_TIME   (3)//
#define EXT_FLASH_SSEC_ERASE_TIME     (300) //4KB
#define EXT_FLASH_SEC_ERASE_TIME      (1500)//64KB
#define EXT_FLASH_CHIP_ERASE_TIME     (80000) //chip erase
#endif

#ifdef EXT_FLASH_MX25L6406
#define EXT_FLASH_MEM_SIZE		      (8 * SIZE_1M)	///< Flash Size: 64Mb = 8MB
#define EXT_FLASH_ID	              (0xC22017)
#define EXT_FLASH_PAGE_PROGRAM_TIME   (3)//
#define EXT_FLASH_SSEC_ERASE_TIME     (300) //4KB
#define EXT_FLASH_SEC_ERASE_TIME      (1500)//64KB
#define EXT_FLASH_CHIP_ERASE_TIME     (80000) //chip erase
#endif

#ifdef EXT_FLASH_MT25QL256A
#define EXT_FLASH_MEM_SIZE		      (32 * SIZE_1M)	///< Flash Size: 256Mb = 32MB
#define EXT_FLASH_ID	              (0x20BA19)
#define EXT_FLASH_PAGE_PROGRAM_TIME   (3)//
#define EXT_FLASH_SSEC_ERASE_TIME     (300) //4KB
#define EXT_FLASH_SEC_ERASE_TIME      (1500)//64KB
#define EXT_FLASH_CHIP_ERASE_TIME     (80000) //chip erase
#endif

#ifdef EXT_FLASH_MX25L51245G
#define EXT_FLASH_MEM_SIZE		      (64 * SIZE_1M)	///< Flash Size: 512Mb = 64MB
#define EXT_FLASH_ID	              (0xC2201A)
#define EXT_FLASH_PAGE_PROGRAM_TIME   (3)//
#define EXT_FLASH_SSEC_ERASE_TIME     (500) //4KB
#define EXT_FLASH_SEC_ERASE_TIME      (3000)//64KB
#define EXT_FLASH_CHIP_ERASE_TIME     (200000) //chip erase

#endif

#define EXT_FLASH_PAGE_SIZE		      (256)					///< Flash Page Size: 256 Byte
#define EXT_FLASH_PAGE_MASK           (0xFFFFFF00)               ///< 0xFFFFFFFF - (page size -1)
#define EXT_FLASH_SSEC_SIZE		      (4 * SIZE_1K)			///< Flash SubSector Size: 4KB
#define EXT_FLASH_SEC_SIZE		      (64 * SIZE_1K)			///< Flash Sector Size: 64KB
#define EXT_FLASH_MEM_SIZE_16M		  (16 * SIZE_1M)

#define EXT_FLASH_TOTAL_SSEC	      (EXT_FLASH_MEM_SIZE / EXT_FLASH_SSEC_SIZE)	///< Total SubSector number
#define EXT_FLASH_TOTAL_SEC		      (EXT_FLASH_MEM_SIZE / EXT_FLASH_SEC_SIZE)	///< Total Sector number

#define EXT_FLASH_SIZE                (EXT_FLASH_MEM_SIZE)
#define EXT_FLASH_START_ADDR	      (0)								      ///< Flash Start Address
#define EXT_FLASH_END_ADDR		      (EXT_FLASH_START_ADDR+EXT_FLASH_SIZE)  ///< Flash End Address


#define EXT_FLASH_OK				  0	///< Success
#define EXT_FLASH_ERROR				  1	///< Error:flash busy

int ExtFlash_Init(void);
void ExtFlash_WakeUP(void);

int ExtFlash_ReadID(void);

#ifdef __cplusplus
}
#endif	/* __cplusplus */
#endif  /* __EXT_FLASH_H__ */

/**
  * @}
  */ /* EXT_FLASH Module*/

