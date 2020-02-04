#ifndef TIME_H
#define TIME_H

#define TIME_ON 1
#define TIME_OFF 0
#define ONE_SECOND 1000

unsigned long getTime(void);
int getFlag(void);
void setFlag(int variable);
void initTimer(void);
#endif
