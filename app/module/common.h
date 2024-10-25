/**
 *****************************************************************************
 * @file common.h
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
 * @defgroup COMMON
 * @brief
 * @{
 */

#ifndef __COMMON_H__
#define __COMMON_H__

#ifdef  __cplusplus
extern "C" {
 #endif		/* __cplusplus */

#pragma pack(1)

	/**
	 * @union _var8
	 * @brief union type for 16bit varable
	 */
	typedef union _var8
	{
		unsigned char u8;		///< 8bit variable
		char s8;
		struct
		{
			unsigned char b0: 1; ///< 1bit low byte variable
			unsigned char b1: 1;
			unsigned char b2: 1;
			unsigned char b3: 1;
			unsigned char b4: 1;
			unsigned char b5: 1;
			unsigned char b6: 1;
			unsigned char b7: 1; ///< 1bit high byte variable
		} u1;
	} var8;

	/**
	 * @union _var16
	 * @brief union type for 16bit varable
	 */
	typedef union _var16
	{
		unsigned short u16; 	///< 16bit variable
		short s16;
		struct
		{
			unsigned char lo;	///< 8bit low byte variable
			unsigned char hi;	///< 8bit high byte variable
		} u8;
	} var16;

	/**
	 * @union _var32
	 * @brief union type for 32bit variable
	 */
	typedef union _var32
	{
		unsigned long u32;		///< 32bit variable
		long s32;
		struct
		{
			unsigned short lo;	///< 16bit lower variable
			unsigned short hi;	///< 16bit higher variable
		} u16;
		struct
		{
			unsigned char b0;	///< 8bit byte 0 variable
			unsigned char b1;	///< 8bit byte 1 variable
			unsigned char b2;	///< 8bit byte 2 variable
			unsigned char b3;	///< 8bit byte 3 variable
		} u8;
	} var32;

	typedef union _var64
	{
		unsigned long long u64;
		long long s64;
		struct
		{
			unsigned long lo;
			unsigned long hi;
		} u32;
		struct
		{
			unsigned short b0;
			unsigned short b1;
			unsigned short b2;
			unsigned short b3;
		} u16;
		struct
		{
			unsigned char b0;	///< 8bit byte 0 variable
			unsigned char b1;	///< 8bit byte 1 variable
			unsigned char b2;	///< 8bit byte 2 variable
			unsigned char b3;	///< 8bit byte 3 variable
			unsigned char b4;	///< 8bit byte 0 variable
			unsigned char b5;	///< 8bit byte 1 variable
			unsigned char b6;	///< 8bit byte 2 variable
			unsigned char b7;	///< 8bit byte 3 variable
		} u8;
	} var64;
#pragma pack()


#ifdef __cplusplus
}
#endif	/* __cplusplus */
#endif  /* __COMMON_H__ */

/**
  * @}
  */ /* COMMON Module*/

