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
#include "blinky.h"
#include "state_machine_bsp.h"

//Q_DEFINE_THIS_FILE

/*..........................................................................*/
static QEvt const *l_blinkyQSto[10]; /* event queue storage for Blinky */
K_THREAD_STACK_DEFINE(l_blinkyStack, 1024); /* stack storage for Blinky */

int State_Machine_Init(void) {

    QF_init();  /* initialize the framework and the underlying RT kernel */
    State_Machine_BSP_Init(); /* initialize the Board Support Package */

    /* publish-subscribe not used, no call to QF_psInit() */
    /* dynamic event allocation not used, no call to QF_poolInit() */

    /* instantiate and start the active objects... */
    Blinky_ctor();
    QActive_setAttr(AO_Blinky,
                    0,            /* thread opions */
                    "Blinky");    /* thread name */
    QACTIVE_START(AO_Blinky,      /* AO pointer to start */
                  1U,             /* unique QP priority of the AO */
                  l_blinkyQSto,   /* storage for the AO's queue */
                  Q_DIM(l_blinkyQSto), /* lenght of the queue [entries] */
                  (void *)l_blinkyStack, /* stack storage */
                  K_THREAD_STACK_SIZEOF(l_blinkyStack), /* stack size [bytes] */
                  (void *)0);     /* initial event (or 0) */

    return QF_run(); /* run the QF application */
}

