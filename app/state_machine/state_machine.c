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
#include "pulse_gen.h"

static struct k_timer zephyr_tick_timer;

static void zephyr_tick_function(struct k_timer *tid) {
    Q_UNUSED_PAR(tid);

    QTIMEEVT_TICK_X(0U, (void *)0);
}

int StateMachineFramwork(void)
{
    static QF_MPOOL_EL(PulseGen_Evt) smlPoolSto[10];
    static QSubscrList subscrSto[MAX_PUB_SIG];

    k_timer_init(&zephyr_tick_timer, &zephyr_tick_function, NULL);

    QF_init();  /* initialize the framework */
    /* Initialize publish-subscribe... */
    QF_psInit(subscrSto, Q_DIM(subscrSto));
    /* Initialize the event pools... */
    QF_poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

	PulseGen_Init();

    return QF_run(); // run the QF application
}

void QF_onStartup(void) {
    k_timer_start(&zephyr_tick_timer, K_MSEC(1), K_MSEC(1));
    printk("QF_onStartup\n");
}

void QF_onCleanup(void) {
    printk("QF_onCleanup\n");
}

Q_NORETURN Q_onError(char const * const module, int_t const id) {
    // NOTE: this implementation of the assertion handler is intended only
    // for debugging and MUST be changed for deployment of the application
    // (assuming that you ship your production code with assertions enabled).
    Q_UNUSED_PAR(module);
    Q_UNUSED_PAR(id);
    printk("\nERROR in %s:%d\n", module, id);

    for (;;) { // explicitly no-return
    }
}

void assert_failed(char const * const module, int_t const id); // prototype
void assert_failed(char const * const module, int_t const id) {
    Q_onError(module, id);
}

