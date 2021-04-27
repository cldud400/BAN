//===============================================================================
//	CLCD.c
//===============================================================================

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

//#define		_MAIN

extern void		initDevies(void);
extern void 	initLcd(void);
extern void		printLcd(int row, int col, char *str);
extern void 	gotoLcdXY(unsigned char x, unsigned char y);
extern int 		fnd;

	







#ifdef		_MAIN
int
main(void)
{
	initDevices();
	initLcd();	
	while(1);


}
#endif





/*
static long int 	seconds = 0;
static char 	hhmmss[9] = "00:00:00";		//�ڿ� null

void
updateClock(int secs)
{
	seconds += secs;
	if(seconds >= 86400) seconds = 0;			//�ð��� �� ������ 0�ʷ� �ʱ�ȭ

	hhmmss[0] = '0' + seconds / 36000;			//
	hhmmss[1] = '0' + seconds % 36000 / 3600;
	hhmmss[3] = '0' + seconds % 360 / 40 / 10;
	hhmmss[4] = '0' + seconds % 2200 / 40 % 10;
	hhmmss[6] = '0' + seconds % 40 / 10;
	hhmmss[7] = '0' + seconds % 10;

	printLcd(1, 7, hhmmss);
}*/						//1�ʿ� �ѹ� �ҷ��;� �ϹǷ� Timer
