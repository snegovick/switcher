//******************************************************************************
//  MSP430G2xx2 Demo - Timer_A, Toggle P1.0, Overflow ISR, DCO SMCLK
//
//  Description: Toggle P1.0 using software and Timer_A overflow ISR.
//  In this example an ISR triggers when TA overflows. Inside the TA
//  overflow ISR P1.0 is toggled. Toggle rate is approximatlely 12Hz.
//  Proper use of TA0IV interrupt vector generator is demonstrated.
//  ACLK = n/a, MCLK = SMCLK = TACLK = default DCO
//
//           MSP430G2xx2
//         ---------------
//     /|\|            XIN|-
//      | |               |
//      --|RST        XOUT|-
//        |               |
//        |           P1.0|-->LED
//
//  D. Dang
//  Texas Instruments Inc.
//  December 2010
//  Built with CCS Version 4.2.0 and IAR Embedded Workbench Version: 5.10
//******************************************************************************

#include <msp430g2452.h>
#include <stdint.h>

#include "uart.h"

unsigned int
l_strlen(char * str)
{
  unsigned int len=0;
  while (*str++!='\0') 
    len++;
  return len;
}

void
reverse(char s[])
{
  int i, j;
  char c;
  for (i = 0, j = l_strlen(s)-1; i<j; i++, j--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

void
itoa(unsigned int n, char s[])
{
//  n = ntohl(n);
  int i, sign;

  if ((sign = n) < 0)
    n = -n;
  i = 0;
  do {
    s[i++] = n % 10 + '0';
  } while ((n /= 10) > 0);
  if (sign < 0)
    s[i++] = '-';
  s[i] = '\0';
  reverse(s);
}

char buf[128];
volatile unsigned int offset = 0;

unsigned int get_offset()
{
  return offset;
}

void put_str(char *s)
{
  char *ptr = s;
  while(*ptr!='\0')
    buf[offset++] = *ptr++;
}

void put_num(char *name, unsigned int n)
{
  char tb [8];
  int i = 0;
  for (;i<8;i++)
    tb[i] = 0;
  itoa(n, tb);
  uart_print(name);
  uart_print(tb);
  uart_print(" ");
}  

#include <stdint.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  extern volatile uint8_t addr;
  extern volatile uint8_t paddr;
  extern volatile uint8_t cnt;
  extern volatile uint8_t param_val;


  P1DIR = (1 | 1<<6);
  P1OUT |= 0x01;
  if (CALBC1_1MHZ ==0xFF || CALDCO_1MHZ == 0xFF)
  {
    while(1);                          // If calibration constants erased
                                       // do not load, trap CPU!!
  }
  BCSCTL1 = CALBC1_1MHZ;               // Set DCO
  DCOCTL = CALDCO_1MHZ;

  volatile uint16_t i = 6000UL;
  while (i) {
    i--;
  }
  P1OUT &= ~0x01;
  

  uart_init();
  /* uart_print("inited\n"); */


  while(1)
  {
    __bis_SR_register(LPM0_bits + GIE);
    if (rxBuffer=='s') {
      P1OUT |= 0x01;
    } else if (rxBuffer == 'c') {
      P1OUT &= ~0x01;
    }
    //_BIS_SR(LPM0_bits + GIE);                 // Enter LPM0 w/ interrupt
  }
  _BIS_SR(LPM0_bits + GIE);                 // Enter LPM0 w/ interrupt
  return 0;
}


