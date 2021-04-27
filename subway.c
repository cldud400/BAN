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
	DDRG = 0x10;				//OUTPUT PORT임을 선언, 소리키움
}

static void
initTimer0(void)					//Timer 초기화
{
	TCCR0 = 0x00;				//Timer 정지
	TCNT0 = 0x06;				//count : 250kHz / (256 - 6) = 1kHz, 256에서 0이 되는 것
	TCCR0 = 0x04;				//nomal mode, prescale : 16MHz / 64 = 250kHz
}

static void
initTimer1(void)
{
	TCCR1B = 0x00;
	TCNT1H = 0x00;				//16bit라 2가지
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

ISR(TIMER0_OVF_vect)				//overflow가 일어났을 때 호출되는 interrupt, 매크로
{
	TCNT0 = 0x06;				//다시 interrupt가 발생하도록 넣는 초기값
#ifdef	_LED
	updateLed();				//interrupt가 실행됐을 때 호출하는 함수, Timer로 동작하는 LED가 동작
#endif
#ifdef	_SPEAKER
	if(autoPlay) playMusic();		//수동연주,자동연주 선택
	else playPiano();				//1/1000초마다 playPiano()함수를 부른다.
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
	toggleSpeaker();		//keyboard입력에 따라서 다음번 overflow interrupt 발생 주기를 설정한 후 speaker toggle
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
	TIMSK = 0x45;				//TIMER interrupts 조절, Timer0의 overflow interrupt를 허용
	ETIMSK = 0x04;
	sei();
}

unsigned char		musicKey = 100;				//어떤 음을 연주하고 있는지 나타낸다.
//도 레 미 파 솔 라 시 도 레
static unsigned int 		musicScale[9] = {
	0xc440, 0xcac4, 0xd095, 0xd33b, 0xd817, 0xdc7e, 0xe100, 0xe300, 0xe562
	};

static void
toggleSpeaker(void)
{
	if(musicKey < 9) {						//musicKey가 9보다 작으면 연주
		TCNT1H = musicScale[musicKey] >> 8;			//상위 1Byte
		TCNT1L = musicScale[musicKey] & 0xff;			//하위 1Byte
		PORTG ^= 0x10;
	} else
		PORTG &= 0xef;
}

//playPiano는 1초에 1000번 불리기 때문에 Chattering Prevention이 요구된다.

 //int number = 1;
 //char xxx[3];
 //itoa(number, xxx, 100);
 			 //printLcd(1, 5, xxx); 
			 //printLcd(1, 7, "1-2:0")
static void
playPiano(void)
{								//누르는 switch에 따라 소리가 난다.
						//chatter가 200이 되면 다시 0
	if(fnd==0) autoPlay=1;
	else autoPlay=0;
	if(PINB & 0x80) { printLcd(1, 1, "Wait            ");
						printLcd(2, 1, "Open            ");
						autoPlay = 0;
		if(PINB & 0x40) autoPlay = 1;				//0x80과 0x40이 동시에 눌렸을 때, 자동연주		
		else musicKey = 0;
	}  if(PINB & 0x40) printLcd(1, 1, "1-1 : 1, 1-2 : 1");
	 if(PINB & 0x20)  printLcd(2, 1, "2-1 : 1, 2-2 : 1");	
	if(PINB & 0x10)   printLcd(1, 1, "1-1 : 0, 1-2 : 1");
	 if(PINB & 0x08)  printLcd(1, 1, "1-1 : 0, 1-2 : 0");
	if(PINB & 0x04) printLcd(2, 1, "2-1 : 0, 2-2 : 1");
	 if(PINB & 0x02)printLcd(2, 1, "2-1 : 0, 2-2 : 0");
	 if(PINB & 0x01) musicKey = 7;
	else musicKey = 100;					//아무것도 누르지 않았을 때는 소리나지 않는다.
}





unsigned char		fndDigit[4] = {0, 0, 0, 0};
static unsigned char		fndPosition = 0;
static const unsigned char	fndSegment[] = {
	0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7c, 0x07, 0x7f, 0x67	//16진법, 변경하지않는다는 const
	};

static void
updateFnd(void)
{
	EX_FND_SELECT = 0x0f;			//FND 4개에 모두 1의 값을 줌으로써, 어떤 FND도 선택하지 않은 상태가 됨
	EX_FND_DATA = fndSegment[fndDigit[fndPosition]];	//Data 값을 바꿔줌
	EX_FND_SELECT = ~(0x01 << fndPosition);		//4개 중 하나를 선택, 선택하는 bit이 0이므로 반전

	fndPosition++;
	fndPosition %= 4;
}

//포인터는 새로운 값을 지정할 수 있다
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

//4개의 control신호를 켜고 끔
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
		for(volatile int j = 0; j < 600; j++);	//15ms를 n번만큼 지연,
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
	LCD_RS_OFF;			//명령을 보낼 때 꺼야함
	delay(1);
	LCD_RW_OFF;			//AVR에서 Character LCD로 데이터를 보내겠다
	delay(1);
	LCD_EN_ON;			//장치 가동
	delay(1);
	EX_CLCD_DATA = value;		//데이터를 데이터 버스에 올림
	delay(1);
	LCD_EN_OFF;			//데이터 전달 종료
	delay(1);
}

void
writeData(char value)
{
	LCD_EN_OFF;
	delay(1);
	LCD_RS_ON;			//데이터를 보낼 때 켜야함
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
	writeCommand(0x06);		//address increment, 오른쪽으로 글자가 채워짐
	writeCommand(0x01);		//화면을 전부 클리어
	delay2(100);
	writeCommand(0x02);		//커서를 좌측 상단에 위치
	delay2(100);
}

void
writeString(char *str)						//특정 커서위치부터 문자 전달
{
	while(*str)
		writeData(*str++);				//한 자 한 자 출력
}

void
gotoLcdXY(unsigned char x, unsigned char y)			//커서위치를 특정 행과 열에 위치시킴
{
	switch(y) {					//y는 행, x는 열
		case 1:					//1번 행, 0x80부터 시작
			writeCommand(0x80 + x - 1);		
			break;
		case 2:					//2번 행, 0xc0부터 시작
			writeCommand(0xc0 + x - 1);
			break;
	}	
}

void
printLcd(int row, int col, char *str)
{
	gotoLcdXY(col, row);				//위치를 잡아준다
	writeString(str);					//전달된 문자열 출력
}


