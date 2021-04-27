//===============================================================================
//	Speaker.c
//===============================================================================

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

// #define		_MAIN

#define		EX_LED		(*(volatile unsigned char*) 0x8008)

extern void		initDevies(void);

extern unsigned char	musicKey;
extern unsigned char	autoPlay;

void	playPiano(void);

#ifdef		_MAIN
int
main(void)
{
	initDevices();
	while(1);
}
#endif

static unsigned char		musicScore[] = {
	0x44, 0x54, 0x44, 0x34, 0x24, 0x34, 0x48, 0x14, 0x24, 0x38, 0x24, 0x34, 0x48,
	0x44, 0x54, 0x44, 0x34, 0x24, 0x34, 0x48, 0x18, 0x48, 0x24, 0x04, 0x04, 0xb4,
	0x44, 0x54, 0x44, 0x34, 0x24, 0x34, 0x48, 0x14, 0x24, 0x38, 0x24, 0x34, 0x48,
	0x44, 0x54, 0x44, 0x34, 0x24, 0x34, 0x48, 0x18, 0x48, 0x24, 0x04, 0x04, 0xb4,
	0x44, 0x54, 0x44, 0x34, 0x24, 0x34, 0x48, 0x14, 0x24, 0x38, 0x24, 0x34, 0x48,
	0x44, 0x54, 0x44, 0x34, 0x24, 0x34, 0x48, 0x18, 0x48, 0x24, 0x04, 0x04, 0xb4
	};

static unsigned char		musicNote = 0;				//��ǥ�� ����Ų��.
static unsigned int		noteLength = 0;				//���� ���̸� ��Ÿ��, 0�� �� ���� ��ǥ�� �Ѿ

void
playMusic(void)
{
	if(PINB & 0x01)						//0x01 ����ġ�� ������ ���� ����
		{autoPlay = 0;  }
	else if(noteLength > 0) noteLength--;				//���� ���ֵǴ� ��ǥ���� �������� Ȯ��
	else if(musicNote < 100) {
		musicKey = musicScore[musicNote] >> 4;		//���� ����Ű�� �ִ� ��ǥ�� ���� 4bit ��
		noteLength = 100 * (musicScore[musicNote] & 0x0f);	//���� ����Ű�� �ִ� ��ǥ�� ���� 4bit ��
		musicNote++;
	} else
		musicKey = 200;					//���� ��
}
