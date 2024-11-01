/**
 *****************************************************************************
 * @file pulse_gen.h
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
 * @defgroup PULSE_GEN
 * @brief
 * @{
 */

#ifndef __PULSE_GEN_H__
#define __PULSE_GEN_H__

#ifdef  __cplusplus
extern "C" {
 #endif		/* __cplusplus */

#include "state_machine_manage.h"


typedef struct _PulseGen_Evt{
// protected:
    QEvt super;

// public:
    uint32_t hi_duration;
    uint32_t lo_duration;
} PulseGen_Evt;

void PulseGen_Init(void);
void PulseGen_Post(uint32_t hi_duration, uint32_t lo_duration);


#ifdef __cplusplus
}
#endif	/* __cplusplus */
#endif  /* __PULSE_GEN_H__ */

/**
  * @}
  */ /* PULSE_GEN Module*/

