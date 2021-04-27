//===============================================================================
//	FND.c
//===============================================================================

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

//  #define		_MAIN

extern void		initDevies(void);
extern void		initTimer0(void);
#define GO 0

#ifdef		_MAIN
int
main(void)
{
	initDevices();
	while(1)
	{for fndValue !=0;}
}
#endif

extern unsigned char	fndDigit[];
extern unsigned char    fndData[];
static unsigned int		fndValue = 15;

int fnd = 0;


void
updateFndDigits(void)
{
	
	if(fndValue !=0) fndValue--;
	//fndValue %= 30;					//0~9999
	//fndDigit[0] = fndValue % 3600 / 600; 
	fndDigit[0] = 0;
	//fndDigit[1] = fndValue % 3600 % 600 / 60; 
	fndDigit[1] = 0;
	if(fndValue <= 15)
	{
		fndDigit[2] = fndValue % 60 / 10; 
	}
	fndDigit[3] = fndValue % 10;
	if (fndValue == 0) fnd = 1;

}




