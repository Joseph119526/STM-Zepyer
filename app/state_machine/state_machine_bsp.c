/**
 *****************************************************************************
 * @file state_machine_bsp.c
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
#include "state_machine_bsp.h"

/* other drivers, if necessary ... */
//Q_DEFINE_THIS_FILE

static struct k_timer QF_tick_timer;
static void QF_tick_function(struct k_timer *tid) {
    (void)tid; /* unused parameter */
    QTIMEEVT_TICK_X(0U, (void *)0);
}

void State_Machine_BSP_Init(void) {

    k_timer_init(&QF_tick_timer, &QF_tick_function, NULL);
}

/* QF callbacks ============================================================*/
void QF_onStartup(void) {
    k_timer_start(&QF_tick_timer, K_MSEC(1), K_MSEC(1));
    printk("QF_onStartup\n");
}

/*..........................................................................*/
void QF_onCleanup(void) {
    printk("QF_onCleanup\n");
}

/*..........................................................................*/
Q_NORETURN Q_onAssert(char const * const module, int_t const loc) {
    /*
    * NOTE: add here your application-specific error handling
    */
    printk("\nASSERTION in %s:%d\n", module, loc);
}

