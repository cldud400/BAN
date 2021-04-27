//===============================================================================
//	LED.c
//===============================================================================

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

// #define 	_MAIN

#define		EX_LED		(*(volatile unsigned char*) 0x8008)

extern void	initDevices(void);
extern void delay2(void);
#ifdef		_MAIN					//define되어있는지에 따라 포함하는지 안하는지 결정
int
main(void)
{
	initDevices();				
	while(1) 
}
#endif

static unsigned char 	n1kHzCycles = 0;
static unsigned char	ledData = 0xaa ;
static unsigned char	ledDelay = 250;
static unsigned char	ledDir = 0;

void
updateLed(void)
{
	n1kHzCycles++;
	if(n1kHzCycles == ledDelay) {
		EX_LED = ledData;
		if(ledDir == 0) { // to left
			if(ledData == 0xaa) {ledDir = 1; ledData >>= 1;}
			else {
				ledData <<= 1;
				ledDelay -=25;
			}
		}else{ // to right
			if(ledData == 0x55) {ledDir = 0; ledData <<= 1;}
			else {
				ledData >>= 1;
				ledDelay += 25;
				}
		}
		
		n1kHzCycles = 0;
	} 
}



	

/*void
updateLed()
{
	n1kHzCycles++;					//updateLed() 함수가 불릴 때마다 하나씩 증가
	if(n1kHzCycles == ledDelay) {				//n1kHzCycles이 200이 되면 LED의 값을 바꿔준다.
		EX_LED = ledData;
		if(ledDir == 0) {
			if(ledData == 0x80) {
			ledDir = 1;}	//LED의 최상위 값이 1이 되면, 우측으로 방향 전환
			else {
				ledData <<= 1;		//그렇지 않은 경우 왼쪽으로 shift
				ledDelay -= 20;
			}
		} else {
			if(ledData == 0x01) ledDir = 0;	//최하위 값이 1이 되었을 때, 좌측으로 방향 전환
			else {
				ledData >>= 1;
				ledDelay += 20;
			}
		}
		n1kHzCycles = 0;			//다시 초기화
	}
}	if(ledData= 0x01){
	ledData =0x03;
	ledDelay+=20;}
	if(ledData= 0x03){
	ledData =0x03;
	ledDelay+=20;}
	if(ledData= 0x07){
	ledData =0x07;
	ledDelay+=20;}
	if(ledData= 0x07){
	ledData =0x0f;
	ledDelay+=20;}
	if(ledData= 0x0f){
	ledData =0x1f;
	ledDelay+=20;}
	if(ledData= 0x1f){
	ledData =0x3f;
	ledDelay+=20;}
	if(ledData= 0x3f){
	ledData =0x7f;
	ledDelay+=20;}
	if(ledData= 0x7f){
	ledData =0xff;
	ledDelay+=20;}*/
