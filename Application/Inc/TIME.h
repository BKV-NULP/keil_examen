#ifndef TIME_H
#define TIME_H

extern unsigned long time;
extern volatile unsigned long SysTickCnt;

void getTime(void);
#endif
