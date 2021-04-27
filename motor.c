//==============================================
//         MOTOR1.c
//==============================================


#include   <avr/io.h>
#include   <avr/interrupt.h>
#include   <stdio.h>

#define      EX_LED      (*(volatile unsigned char*) 0x8008)
#define      EX_STEPPER   (*(volatile unsigned char*) 0x8009)
#define		_MAIN
extern void		initDevies(void);
extern void		delay(int n);
extern void		printLcd(int row, int col, char *str);
extern int fnd;




static unsigned char 	smStepPhase1[] = { //single phase 1.8µµ ¾¿
		0x01, 0x02, 0x04, 0x08
		};




#define N1STEPS (sizeof(smStepPhase1)/sizeof(unsigned char))





int
main(void)
{
	 int i, step = 0;
	initDevices();
	initLcd();
	printLcd(1, 1, "1-1 : 1, 1-2 : 1");
	printLcd(2, 1, "2-1 : 1, 2-2 : 1");	
	while(1){
	if(fnd==0){for(i=0; i<200; i++){ //200 x 1.8 = 360
		EX_STEPPER = smStepPhase1[step];
		step = (step + 1) % N1STEPS;
		delay(10);
		}}
		}
		
}
