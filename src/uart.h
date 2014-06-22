#ifndef __UART_H__
#define __UART_H__

extern volatile unsigned char rxBuffer;

void TimerA_UART_print(char *string);
void uart_init(void);
#define uart_print(c) TimerA_UART_print(c);


#endif/*__UART_H__*/
