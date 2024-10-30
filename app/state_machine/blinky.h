/**
 *****************************************************************************
 * @file blinky.h
 * @brief
 * @author Joseph Hsu
 * @date 2024/10/25
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
 * @defgroup BLINKY
 * @brief
 * @{
 */

#ifndef __BLINKY_H__
#define __BLINKY_H__

#ifdef  __cplusplus
extern "C" {
 #endif		/* __cplusplus */

enum BlinkySignals {
	DUMMY_SIG = Q_USER_SIG,
	MAX_PUB_SIG,		  /* the last published signal */

	TIMEOUT_SIG,
	MAX_SIG 			  /* the last signal */
};

static char* BLINKY_SIGNALS_NAMES[] = {
	"QEP_EMPTY_SIG_",
	"Q_ENTRY_SIG",
	"Q_EXIT_SIG",
	"Q_INIT_SIG",
	"DUMMY_SIG",
	"MAX_PUB_SIG",    /* the last published signal */
	"TIMEOUT_SIG",
	"MAX_SIG"         /* the last signal (keep always last) */
};

void Blinky_ctor(void);
extern QActive * const AO_Blinky; /* opaque pointer */

#ifdef __cplusplus
}
#endif	/* __cplusplus */
#endif  /* __BLINKY_H__ */

/**
  * @}
  */ /* BLINKY Module*/

