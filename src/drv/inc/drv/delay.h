#ifndef _DELAY_H_
#define _DELAY_H_

void __delay(unsigned long ncy);
int __ints(int stat);

#define CLOCK_FREQ      SystemCoreClock
#define INSTR_FREQ      (CLOCK_FREQ * 4 / 3)
#define US_CYCLES       (INSTR_FREQ / 1000000)
#define MS_CYCLES       (INSTR_FREQ / 1000)

#define delay_us(us)     __delay((us) * US_CYCLES)
#define delay_ms(ms)     __delay((ms) * MS_CYCLES)



#endif