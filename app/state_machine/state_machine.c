/**
 *****************************************************************************
 * @file state_machine.c
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
#include "qpc.h"
#include "dpp.h"
#include "bsp.h"

/*..........................................................................*/
int State_Machine_Init(void)
{
    QF_init();       // initialize the framework and the underlying RT kernel
    BSP_init();      // initialize the BSP
    BSP_start();     // start the AOs/Threads
    return QF_run(); // run the QF application
}