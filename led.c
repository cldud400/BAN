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
#ifdef		_MAIN					//define�Ǿ��ִ����� ���� �����ϴ��� ���ϴ��� ����
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
	n1kHzCycles++;					//updateLed() �Լ��� �Ҹ� ������ �ϳ��� ����
	if(n1kHzCycles == ledDelay) {				//n1kHzCycles�� 200�� �Ǹ� LED�� ���� �ٲ��ش�.
		EX_LED = ledData;
		if(ledDir == 0) {
			if(ledData == 0x80) {
			ledDir = 1;}	//LED�� �ֻ��� ���� 1�� �Ǹ�, �������� ���� ��ȯ
			else {
				ledData <<= 1;		//�׷��� ���� ��� �������� shift
				ledDelay -= 20;
			}
		} else {
			if(ledData == 0x01) ledDir = 0;	//������ ���� 1�� �Ǿ��� ��, �������� ���� ��ȯ
			else {
				ledData >>= 1;
				ledDelay += 20;
			}
		}
		n1kHzCycles = 0;			//�ٽ� �ʱ�ȭ
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
