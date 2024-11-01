/**
 *****************************************************************************
 * @file pulse_gen.c
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

#include "qpc.h"
#include "pulse_gen.h"
#include "led.h"

typedef struct {
// protected:
    QActive super;   // inherit QActive

// private:
    QTimeEvt timeEvt; // private time event generator

    uint32_t hi_duration;
    uint32_t lo_duration;
} PulseGen;

PulseGen PulseGen_inst; // the Blinky active object
QActive * const AO_PulseGen = &PulseGen_inst.super;

static QState PulseGen_initial(PulseGen * const me, void const * const par);
static QState PulseGen_idle(PulseGen * const me, QEvt const * const e);
static QState PulseGen_set_hi(PulseGen * const me, QEvt const * const e);
static QState PulseGen_set_lo(PulseGen * const me, QEvt const * const e);

void PulseGen_Post(uint32_t hi_duration, uint32_t lo_duration)
{
    PulseGen_Evt *evt = Q_NEW(PulseGen_Evt, PULSE_GEN_TRIGGER_SIG);
    evt->hi_duration = hi_duration;
    evt->lo_duration = lo_duration;
    QACTIVE_POST(AO_PulseGen, &evt->super, 0);

    printk("PulseGen_Post:%d,%d\n",hi_duration,lo_duration);
}

QState PulseGen_initial(PulseGen * const me, void const * const par) {
    Q_UNUSED_PAR(par);
    LED_OFF();

    return Q_TRAN(&PulseGen_idle);
}

QState PulseGen_idle(PulseGen * const me, QEvt const * const e) {
    QState status;

	printk("%s: %s\r\n", __func__,SM_SIGNALS_NAMES[e->sig]);

    switch (e->sig) {
        case Q_ENTRY_SIG: {
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
            status = Q_HANDLED();
            break;
        }
        case PULSE_GEN_TRIGGER_SIG: {
            printk("PulseGen Trigger:%d,%d\n",me->hi_duration,me->lo_duration);
            status = Q_TRAN(&PulseGen_set_hi);
            break;
        }
        default: {
            status = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status;
}

QState PulseGen_set_hi(PulseGen * const me, QEvt const * const e) {
    QState status;

	printk("%s: %s\r\n", __func__,SM_SIGNALS_NAMES[e->sig]);

    switch (e->sig) {
        case Q_ENTRY_SIG: {
			printk("PulseGen set hi:%d\n",me->hi_duration);
			//QTimeEvt_armX(&me->timeEvt, me->hi_duration, 0U);
			QTimeEvt_armX(&me->timeEvt, 20, 0U);

            LED_ON();
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
			(void)QTimeEvt_disarm(&me->timeEvt);
            status = Q_HANDLED();
            break;
        }
        case TIMEOUT_SIG: {
            status = Q_TRAN(&PulseGen_set_lo);
            break;
        }
        default: {
            status = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status;
}
QState PulseGen_set_lo(PulseGen * const me, QEvt const * const e) {
    QState status;

	printk("%s: %s\r\n", __func__,SM_SIGNALS_NAMES[e->sig]);

    switch (e->sig) {
        case Q_ENTRY_SIG: {
			printk("PulseGen set lo:%d\n",me->lo_duration);
			//QTimeEvt_armX(&me->timeEvt, me->lo_duration, 0U);
			QTimeEvt_armX(&me->timeEvt, 80, 0U);

            LED_OFF();
            status = Q_HANDLED();
            break;
        }
        case Q_EXIT_SIG: {
			(void)QTimeEvt_disarm(&me->timeEvt);
            status = Q_HANDLED();
            break;
        }
        case TIMEOUT_SIG: {
            status = Q_TRAN(&PulseGen_idle);
            break;
        }
        default: {
            status = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status;
}

void PulseGen_ctor(void) {
    PulseGen * const me = &PulseGen_inst;
    QActive_ctor(&me->super, Q_STATE_CAST(&PulseGen_initial));
    QTimeEvt_ctorX(&me->timeEvt, &me->super, TIMEOUT_SIG, 0U);
}

void PulseGen_Init(void)
{

  PulseGen_ctor();

  static QEvt const *PulseGenQueueSto[10];
  static K_THREAD_STACK_DEFINE(PulseGenStack, 1024);
  PulseGen_ctor();
  QACTIVE_START(AO_PulseGen,
      PULSE_GEN_THREAD_PRIORITY,            // QP prio. of the AO
      PulseGenQueueSto,                     // event queue storage
      Q_DIM(PulseGenQueueSto),              // queue length [events]
      PulseGenStack,                        // private stack for embOS
      K_THREAD_STACK_SIZEOF(PulseGenStack), // stack size [Zephyr]
      (void *)0);                           // no initialization param
}
