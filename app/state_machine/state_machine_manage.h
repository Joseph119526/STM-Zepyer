/**
 *****************************************************************************
 * @file state_machine_manage.h
 * @brief
 * @author Joseph Hsu
 * @date 2024/11/01
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
 * @defgroup STATE_MACHINE_MANAGE
 * @brief
 * @{
 */

#ifndef __STATE_MACHINE_MANAGE_H__
#define __STATE_MACHINE_MANAGE_H__

#ifdef  __cplusplus
extern "C" {
 #endif		/* __cplusplus */

#include "qpc.h"

static const char* SM_SIGNALS_NAMES[] = {
	"Q_EMPTY_SIG",
	"Q_ENTRY_SIG",
	"Q_EXIT_SIG",
	"Q_INIT_SIG",
	"DUMMY_SIG",
	"MAX_PUB_SIG",

	"TIMEOUT_SIG",
	"TRIGGER_SIG",

	"MAX_SIG"		  /* the last signal (keep always last) */
};

extern enum _SM_SIGNALS {
	DUMMY_SIG = Q_USER_SIG,
	MAX_PUB_SIG,		  // the last published signal

	TIMEOUT_SIG,
	PULSE_GEN_TRIGGER_SIG,
	PULSE_GEN_HI_SIG,
	PULSE_GEN_LO_SIG,
	PULSE_GEN_IDLE_SIG,

	MAX_SIG 			  // the last signal
} SM_SIGNALS;

#define PULSE_GEN_THREAD_PRIORITY		QF_MAX_ACTIVE - 1U


#ifdef __cplusplus
}
#endif	/* __cplusplus */
#endif  /* __STATE_MACHINE_MANAGE_H__ */

/**
  * @}
  */ /* STATE_MACHINE_MANAGE Module*/

