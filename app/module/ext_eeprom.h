/**
 *****************************************************************************
 * @file ext_eeprom.h
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
 * @defgroup EXT_EEPROM
 * @brief
 * @{
 */

#ifndef __EXT_EEPROM_H__
#define __EXT_EEPROM_H__

#ifdef  __cplusplus
extern "C" {
 #endif		/* __cplusplus */


#include <stdio.h>
#include <zephyr/devicetree.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include "i2c_bsp.h"

#define EEPROM_I2C_ADDR	 (0xA0>>1)
extern const struct device *EEPROM;

#define EXT_EEPOM_ADDR                (0xA0 >> 1) //7-bit addr
#define EXT_EEPROM_MEMADDR_SIZE_8BIT  0
#define EXT_EEPROM_MEMADDR_SIZE_16BIT 1

#define EXT_EEPROM_AT24C02
	//#define EXT_EEPROM_24LC08B

#ifdef EXT_EEPROM_24LC08B
#define EXT_EEPROM_MEMADDR_SIZE       EXT_EEPROM_MEMADDR_SIZE_8BIT
#define EXT_EEPROM_MEM_SIZE			  (1024)		    ///< EEPROM Size: 8Kb = 1KB
#define EXT_EEPROM_PAGE_SIZE		  (16)			    ///< EEPROM Page Size: 16 Byte
#endif

#ifdef EXT_EEPROM_AT24C02
#define EXT_EEPROM_MEMADDR_SIZE       EXT_EEPROM_MEMADDR_SIZE_8BIT
#define EXT_EEPROM_MEM_SIZE			  (256)			    ///< EEPROM Size: 8Kb = 1KB
#define EXT_EEPROM_PAGE_SIZE		  (8)					///< EEPROM Page Size: 16 Byte
#endif

#define EXT_EEPROM_PAGE_MASK          (0xFFFF - (EXT_EEPROM_PAGE_SIZE - 1))
#define EXT_EEPROM_TOTAL_PAGE		  (EXT_EEPROM_MEM_SIZE / EXT_EEPROM_PAGE_SIZE)	///< Total Page number

#define EXT_EEPROM_SIZE               (EXT_EEPROM_MEM_SIZE)
#define EXT_EEPROM_START_ADDR	      (0)								      ///< Flash Start Address
#define EXT_EEPROM_END_ADDR		      (EXT_EEPROM_START_ADDR + EXT_EEPROM_SIZE)  ///< Flash End Address

#define EXT_EEPROM_WRITE_TIME         (5)//5ms

#define EXT_EEPROM_OK				  0	///< Success
#define EXT_EEPROM_ERROR			  1	///< Error:eeprom busy
#define EXT_EEPROM_TIMEOUT            2

#if EXT_EEPROM_MEMADDR_SIZE == EXT_EEPROM_MEMADDR_SIZE_16BIT
  #define EXT_EEPROM_I2C_Write(dev_addr,addr,buf,len)        I2C_Reg16Write(EEPROM,dev_addr,addr,buf,len)
  #define EXT_EEPROM_I2C_Read(dev_addr,addr,buf,len)         I2C_Reg16Read(EEPROM,dev_addr,addr,buf,len)
#else
  #define EXT_EEPROM_I2C_Write(dev_addr,addr,buf,len)        I2C_Reg8Write(EEPROM,dev_addr,addr,buf,len)
  #define EXT_EEPROM_I2C_Read(dev_addr,addr,buf,len)         I2C_Reg8Read(EEPROM,dev_addr,addr,buf,len)
#endif
#define ExtEEPROM_IsDeviceReady(dev_addr)                    I2C_IsDeviceReady(EEPROM,EXT_EEPOM_ADDR)

#define EXT_EEPROM_DEBUG                                     (1) //(pCMD->csmsg.mode.dest.ext_flash)
#define EXT_EEPROM_DEBUG_PUTS(x)                             do{if(EXT_EEPROM_DEBUG){printk("%s",x);}}while(0)
#define EXT_EEPROM_DEBUG_PRINT(...)                          do{if(EXT_EEPROM_DEBUG){printk(__VA_ARGS__);}}while(0)

int ExtEEPROM_ReadData(unsigned short ReadAddr,unsigned char* pBuffer,	unsigned short NumByteToRead);
int ExtEEPROM_WriteData(unsigned short WriteAddr,unsigned char* pBuffer,  unsigned short NumByteToWrite);
void ExtEEPROM_Dump(void);
void ExtEEPROM_Cmp(void);

#ifdef __cplusplus
}
#endif	/* __cplusplus */
#endif  /* __EXT_EEPROM_H__ */

/**
  * @}
  */ /* EXT_EEPROM Module*/




