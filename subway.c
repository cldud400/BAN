//===============================================================================
//	Drivers.c
//===============================================================================

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define		_LED
#define		_SPEAKER
#define		_FND
#define		_DOT_MATRIX
#define		_CLCD
#define		_STEPPER

#define		EX_CLCD_DATA		(*(volatile unsigned char *) 0x8000)
#define		EX_CLCD_CONTROL		(*(volatile unsigned char *) 0x8001)
#define		EX_FND_DATA		(*(volatile unsigned char *) 0x8002)
#define		EX_FND_SELECT		(*(volatile unsigned char *) 0x8003)
#define		EX_DOT_MATRIX_LINE	(*(volatile unsigned int *) 0x8004)
#define		EX_DOT_MATRIX_DATA	(*(volatile unsigned int *) 0x8006)
#define		EX_LED			(*(volatile unsigned char *) 0x8008)
#define		EX_STEPPER			(* (volatile unsigned char *) 0x8009)


extern void	updateLed(void);
extern void	playMusic(void);
extern void	updateFndDigits(void);
extern void	fndCountUp(int incr);
extern void	turnDotMatrixPage(void);
extern int fnd;
unsigned char   first = 0;

unsigned char   Speaker = 0;
char			t[]="                                  ";
//extern void	updateClock(int secs);


unsigned char	autoPlay = 0;

static void 	toggleSpeaker(void);
static void		playPiano(void);
static void 	updateFnd();
static void		updateDotMatrix(void);

void printLcd(int row, int col, char *str);


static void
initPorts(void)
{
	DDRG = 0x10;				//OUTPUT PORT���� ����, �Ҹ�Ű��
}

static void
initTimer0(void)					//Timer �ʱ�ȭ
{
	TCCR0 = 0x00;				//Timer ����
	TCNT0 = 0x06;				//count : 250kHz / (256 - 6) = 1kHz, 256���� 0�� �Ǵ� ��
	TCCR0 = 0x04;				//nomal mode, prescale : 16MHz / 64 = 250kHz
}

static void
initTimer1(void)
{
	TCCR1B = 0x00;
	TCNT1H = 0x00;				//16bit�� 2����
	TCNT1L = 0x00;
	TCCR1B = 0x01;
}

static void
initTimer2(void)
{
	TCCR2 = 0x00;
	TCNT2 = 0xce;
	TCCR2 = 0x03;
}

static void
initTimer3(void)
{
	TCCR3B = 0x00;
	TCNT3H = 0xc2;
	TCNT3L = 0xf7;
	TCCR3B = 0x05;
}

ISR(TIMER0_OVF_vect)				//overflow�� �Ͼ�� �� ȣ��Ǵ� interrupt, ��ũ��
{
	TCNT0 = 0x06;				//�ٽ� interrupt�� �߻��ϵ��� �ִ� �ʱⰪ
#ifdef	_LED
	updateLed();				//interrupt�� ������� �� ȣ���ϴ� �Լ�, Timer�� �����ϴ� LED�� ����
#endif
#ifdef	_SPEAKER
	if(autoPlay) playMusic();		//��������,�ڵ����� ����
	else playPiano();				//1/1000�ʸ��� playPiano()�Լ��� �θ���.
#endif
#ifdef	_FND
	if(fnd == 0)
	{
		updateFnd();
	}
#endif
}

ISR(TIMER1_OVF_vect)
{
#ifdef	_SPEAKER
	toggleSpeaker();		//keyboard�Է¿� ���� ������ overflow interrupt �߻� �ֱ⸦ ������ �� speaker toggle
#endif
}

ISR(TIMER2_OVF_vect)
{
	TCNT2 = 0xce;
#ifdef	_DOT_MATRIX
	updateDotMatrix();
#endif
#ifdef _STEPPER

#endif
	
}

ISR(TIMER3_OVF_vect)
{
	TCNT3H = 0xc2;
	TCNT3L = 0xf7;
#ifdef	_FND
	updateFndDigits();
#endif
#ifdef	_DOT_MATRIX
	turnDotMatrixPage();
#endif
#ifdef	_CLCD
	//updateClock(1);
#endif

#ifdef _STEPPER

#endif
}

void
initDevices(void)
{
	cli();
	initPorts();
	initTimer0();
	initTimer1();
	initTimer2();
	initTimer3();
	MCUCR = 0x80;
	TIMSK = 0x45;				//TIMER interrupts ����, Timer0�� overflow interrupt�� ���
	ETIMSK = 0x04;
	sei();
}

unsigned char		musicKey = 100;				//� ���� �����ϰ� �ִ��� ��Ÿ����.
//�� �� �� �� �� �� �� �� ��
static unsigned int 		musicScale[9] = {
	0xc440, 0xcac4, 0xd095, 0xd33b, 0xd817, 0xdc7e, 0xe100, 0xe300, 0xe562
	};

static void
toggleSpeaker(void)
{
	if(musicKey < 9) {						//musicKey�� 9���� ������ ����
		TCNT1H = musicScale[musicKey] >> 8;			//���� 1Byte
		TCNT1L = musicScale[musicKey] & 0xff;			//���� 1Byte
		PORTG ^= 0x10;
	} else
		PORTG &= 0xef;
}

//playPiano�� 1�ʿ� 1000�� �Ҹ��� ������ Chattering Prevention�� �䱸�ȴ�.

 //int number = 1;
 //char xxx[3];
 //itoa(number, xxx, 100);
 			 //printLcd(1, 5, xxx); 
			 //printLcd(1, 7, "1-2:0")
static void
playPiano(void)
{								//������ switch�� ���� �Ҹ��� ����.
						//chatter�� 200�� �Ǹ� �ٽ� 0
	if(fnd==0) autoPlay=1;
	else autoPlay=0;
	if(PINB & 0x80) { printLcd(1, 1, "Wait            ");
						printLcd(2, 1, "Open            ");
						autoPlay = 0;
		if(PINB & 0x40) autoPlay = 1;				//0x80�� 0x40�� ���ÿ� ������ ��, �ڵ�����		
		else musicKey = 0;
	}  if(PINB & 0x40) printLcd(1, 1, "1-1 : 1, 1-2 : 1");
	 if(PINB & 0x20)  printLcd(2, 1, "2-1 : 1, 2-2 : 1");	
	if(PINB & 0x10)   printLcd(1, 1, "1-1 : 0, 1-2 : 1");
	 if(PINB & 0x08)  printLcd(1, 1, "1-1 : 0, 1-2 : 0");
	if(PINB & 0x04) printLcd(2, 1, "2-1 : 0, 2-2 : 1");
	 if(PINB & 0x02)printLcd(2, 1, "2-1 : 0, 2-2 : 0");
	 if(PINB & 0x01) musicKey = 7;
	else musicKey = 100;					//�ƹ��͵� ������ �ʾ��� ���� �Ҹ����� �ʴ´�.
}





unsigned char		fndDigit[4] = {0, 0, 0, 0};
static unsigned char		fndPosition = 0;
static const unsigned char	fndSegment[] = {
	0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7c, 0x07, 0x7f, 0x67	//16����, ���������ʴ´ٴ� const
	};

static void
updateFnd(void)
{
	EX_FND_SELECT = 0x0f;			//FND 4���� ��� 1�� ���� �����ν�, � FND�� �������� ���� ���°� ��
	EX_FND_DATA = fndSegment[fndDigit[fndPosition]];	//Data ���� �ٲ���
	EX_FND_SELECT = ~(0x01 << fndPosition);		//4�� �� �ϳ��� ����, �����ϴ� bit�� 0�̹Ƿ� ����

	fndPosition++;
	fndPosition %= 4;
}

//�����ʹ� ���ο� ���� ������ �� �ִ�
short		 *dotMatrixData = (short *) NULL;
static unsigned char		dotMatrixLine = 0;

static void
updateDotMatrix(void)
{
	if(!dotMatrixData) return;
	EX_DOT_MATRIX_LINE = 1 << dotMatrixLine;
	EX_DOT_MATRIX_DATA = dotMatrixData[dotMatrixLine];
	dotMatrixLine++;
	dotMatrixLine %= 10;
}

static char	clcdControl = 0;

//4���� control��ȣ�� �Ѱ� ��
#define LCD_EN_ON 	(EX_CLCD_CONTROL = (clcdControl |= 0x04));
#define LCD_EN_OFF 	(EX_CLCD_CONTROL = (clcdControl &= ~0x04));
#define LCD_RW_ON 	(EX_CLCD_CONTROL = (clcdControl |= 0x02));
#define LCD_RW_OFF 	(EX_CLCD_CONTROL = (clcdControl &= ~0x02));
#define LCD_RS_ON 	(EX_CLCD_CONTROL = (clcdControl |= 0x01));
#define LCD_RS_OFF 	(EX_CLCD_CONTROL = (clcdControl &= ~0x01));

void
delay(int n)
{
	for(volatile int i = 0; i < n; i++) {
		for(volatile int j = 0; j < 600; j++);	//15ms�� n����ŭ ����,
	}
}

void
delay2(int n)
{
    for(volatile int i = 0; i < n; i++) {
	   for(volatile int j = 0; j < 6; j++);
    }
}

void
writeCommand(char value)
{
	LCD_EN_OFF;
	delay(1);				//15~20ms
	LCD_RS_OFF;			//����� ���� �� ������
	delay(1);
	LCD_RW_OFF;			//AVR���� Character LCD�� �����͸� �����ڴ�
	delay(1);
	LCD_EN_ON;			//��ġ ����
	delay(1);
	EX_CLCD_DATA = value;		//�����͸� ������ ������ �ø�
	delay(1);
	LCD_EN_OFF;			//������ ���� ����
	delay(1);
}

void
writeData(char value)
{
	LCD_EN_OFF;
	delay(1);
	LCD_RS_ON;			//�����͸� ���� �� �Ѿ���
	delay(1);
	LCD_RW_OFF;
	delay(1);
	LCD_EN_ON;
	delay(1);
	EX_CLCD_DATA = value;
	delay(1);
	LCD_EN_OFF;
	delay(1);
}

void
initLcd()
{
	writeCommand(0x38);		//8bit data, 2lines, 5x7 dots
	writeCommand(0x0e);		//display on, cursor on, blink off
	writeCommand(0x06);		//address increment, ���������� ���ڰ� ä����
	writeCommand(0x01);		//ȭ���� ���� Ŭ����
	delay2(100);
	writeCommand(0x02);		//Ŀ���� ���� ��ܿ� ��ġ
	delay2(100);
}

void
writeString(char *str)						//Ư�� Ŀ����ġ���� ���� ����
{
	while(*str)
		writeData(*str++);				//�� �� �� �� ���
}

void
gotoLcdXY(unsigned char x, unsigned char y)			//Ŀ����ġ�� Ư�� ��� ���� ��ġ��Ŵ
{
	switch(y) {					//y�� ��, x�� ��
		case 1:					//1�� ��, 0x80���� ����
			writeCommand(0x80 + x - 1);		
			break;
		case 2:					//2�� ��, 0xc0���� ����
			writeCommand(0xc0 + x - 1);
			break;
	}	
}

void
printLcd(int row, int col, char *str)
{
	gotoLcdXY(col, row);				//��ġ�� ����ش�
	writeString(str);					//���޵� ���ڿ� ���
}


