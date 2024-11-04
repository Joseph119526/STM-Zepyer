/**
 *****************************************************************************
 * @file console.c
 * @brief
 * @author Joseph Hsu
 * @date 2024/10/22
 * @version
 *
 * @par History
 * <table>
 * <tr><th>Data        <th>Version <th>Author  <th>Description
 * <tr><td>            <td>        <td>        <td>
 * </table>
 *****************************************************************************
 */
#include <stdio.h>
#include <string.h>
#include <zephyr/devicetree.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/kernel.h>

#include "pulse_gen.h"

#define MSG_SIZE 32
/* queue to store up to 10 messages (aligned to 4-byte boundary) */
K_MSGQ_DEFINE(uart_msgq, MSG_SIZE, 10, 4);

/* size of stack area used by each thread */
#define STACKSIZE 1024

/* scheduling priority used by each thread */
#define PRIORITY 7

#define UART_DEV DT_NODELABEL(usart6)
const static struct device *CONSOLE = NULL;//= DEVICE_DT_GET(UART_DEV);

/* receive buffer used in UART ISR callback */
static char rx_buf[MSG_SIZE];
static int rx_buf_pos;

/*
 * Read characters from UART until line end is detected. Afterwards push the
 * data to the message queue.
 */
void Console_Callback(const struct device *dev, void *user_data)
{
	uint8_t c;

	if(!uart_irq_update(CONSOLE))
	{
		return;
	}

	if(!uart_irq_rx_ready(CONSOLE))
	{
		return;
	}

	/* read until FIFO empty */
	while(uart_fifo_read(CONSOLE, &c, 1) == 1)
	{
		if((c == '\n' || c == '\r') && rx_buf_pos > 0)
		{
			/* terminate string */
			rx_buf[rx_buf_pos] = '\0';

			/* if queue is full, message is silently dropped */
			k_msgq_put(&uart_msgq, &rx_buf, K_NO_WAIT);

			if(rx_buf[0]=='p') {
				PulseGen_Post(20, 80, 40);
			}

			/* reset the buffer (it was copied to the msgq) */
			rx_buf_pos = 0;
		}
		else if(rx_buf_pos < (sizeof(rx_buf) - 1))
		{
			rx_buf[rx_buf_pos++] = c;
		}
		/* else: characters beyond buffer size are dropped */
	}
}

/*
 * Print a null-terminated string character by character to the UART interface
 */
void print_uart(char *buf)
{
	int msg_len = strlen(buf);

	for(int i = 0; i < msg_len; i++)
	{
		uart_poll_out(CONSOLE, buf[i]);
	}
}

void Console_Thread(void)
{
	char tx_buf[MSG_SIZE];
    int ret;

	CONSOLE = device_get_binding("console");

	if(!device_is_ready(CONSOLE))
	{
		printk("Console is not ready\r\n");
		return;
	}

	/* configure interrupt and callback to receive data */
	ret = uart_irq_callback_user_data_set(CONSOLE, Console_Callback, NULL);

	if(ret < 0)
	{
		printk("Console callback is not ready\r\n");
		return;
	}
	uart_irq_rx_enable(CONSOLE);

	print_uart("Console Init OK!\r\n");
	while(k_msgq_get(&uart_msgq, &tx_buf, K_FOREVER) == 0)
	{
		print_uart("Echo: ");
		print_uart(tx_buf);
		print_uart("\r\n");
	}
}

K_THREAD_DEFINE(Console, STACKSIZE, Console_Thread, NULL, NULL, NULL, PRIORITY, 0, 0);

