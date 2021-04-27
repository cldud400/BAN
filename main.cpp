#include "resource.h"
#include <windows.h>
#include <stdio.h>

int Block[7][4][4][4] = {
   0,1,0,0,
   0,1,0,0,
   0,1,0,0,
   0,1,0,0,

   0,0,0,0,
   1,1,1,1,
   0,0,0,0,
   0,0,0,0,

   0,1,0,0,
   0,1,0,0,
   0,1,0,0,
   0,1,0,0,

   0,0,0,0,
   1,1,1,1,
   0,0,0,0,
   0,0,0,0,

   0,0,1,0,
   0,0,1,0,
   0,1,1,0,
   0,0,0,0,

   1,1,1,0,
   0,0,1,0,
   0,0,0,0,
   0,0,0,0,

   1,1,0,0,
   1,0,0,0,
   1,0,0,0,
   0,0,0,0,

   0,0,0,0,
   1,0,0,0,
   1,1,1,0,
   0,0,0,0,

   1,0,0,0,
1, 0, 0, 0,
1, 1, 0, 0,
0, 0, 0, 0,

0, 0, 0, 0,
0, 0, 1, 0,
1, 1, 1, 0,
0, 0, 0, 0,

0, 1, 1, 0,
0, 0, 1, 0,
0, 0, 1, 0,
0, 0, 0, 0,

1, 1, 1, 0,
1, 0, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,

0, 0, 0, 0,
0, 1, 0, 0,
1, 1, 1, 0,
0, 0, 0, 0,

0, 0, 1, 0,
0, 1, 1, 0,
0, 0, 1, 0,
0, 0, 0, 0,

1, 1, 1, 0,
0, 1, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,

1, 0, 0, 0,
1, 1, 0, 0,
1, 0, 0, 0,
0, 0, 0, 0,

0, 1, 0, 0,
0, 1, 1, 0,
0, 0, 1, 0,
0, 0, 0, 0,

0, 1, 1, 0,
1, 1, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,

0, 1, 0, 0,
0, 1, 1, 0,
0, 0, 1, 0,
0, 0, 0, 0,

0, 1, 1, 0,
1, 1, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,

0, 1, 0, 0,
1, 1, 0, 0,
1, 0, 0, 0,
0, 0, 0, 0,

1, 1, 0, 0,
0, 1, 1, 0,
0, 0, 0, 0,
0, 0, 0, 0,

0, 1, 0, 0,
1, 1, 0, 0,
1, 0, 0, 0,
0, 0, 0, 0,

1, 1, 0, 0,
0, 1, 1, 0,
0, 0, 0, 0,
0, 0, 0, 0,

1, 1, 0, 0,
1, 1, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,

1, 1, 0, 0,
1, 1, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,

1, 1, 0, 0,
1, 1, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0,

1, 1, 0, 0,
1, 1, 0, 0,
0, 0, 0, 0,
0, 0, 0, 0, };


int BackGround[21][12];

#define WM_NewBlock WM_USER + 1
#define YES 1
#define NO 0
#define SUCCESS 1
#define FAIL 0
#define ON 1
#define OFF 0
#define ALIVE  1
#define DEAD 0

BOOL bTime = OFF;

int BlockNum;
int RotateNum;
int NowX, NowY;
int NextBlockNum;
int FullLineNum;
int PlayerState;

HWND hWnd;

void InitBackGround(void);
void DrawBackGround(void);
void DrawBlock(void);
void EraseBlock(void);
void DrawNextBlock(void);
BOOL BlockCanMove(int x, int y);
void LeftMove(void);
void RightMove(void);
void Rotate(void);
BOOL DownMove(void);
void UpdateBackGround(void);
void CheckFullLine(void);
void EraseFullLine(int);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,
	int nShowCmd)
{

	MSG mSg;


	//hInst = hInstance;
	char szTitle[] = "테트리스";
	char szClass[] = "Class";
	WNDCLASSEX WndEx;
	WndEx.cbSize = sizeof(WndEx);
	WndEx.style = NULL;
	WndEx.lpfnWndProc = WndProc;
	WndEx.cbClsExtra = 0;
	WndEx.cbWndExtra = 0;
	WndEx.hInstance = hInstance;
	WndEx.hIcon = LoadIcon(NULL, "");
	WndEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndEx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndEx.lpszMenuName = "IDR_MENU";
	WndEx.lpszClassName = szClass;
	WndEx.hIconSm = LoadIcon(hInstance, "");
	RegisterClassEx(&WndEx);
	hWnd = CreateWindowEx(NULL, szClass, szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		320, 300, NULL, NULL, hInstance, NULL); // 윈도우창 사이즈
	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

	while (TRUE)
	{
		if (PeekMessage(&mSg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&mSg, NULL, 0, 0))
				break;
			TranslateMessage(&mSg);
			DispatchMessage(&mSg);
		}
	}
	return mSg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd,
	UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
		//main structure

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 40001:
			InitBackGround();
			DrawBackGround();
			PlayerState = ALIVE;
			NextBlockNum = rand() % 7;
			FullLineNum = 0;
			SendMessage(hWnd, WM_NewBlock, 0, 0);
			if (bTime == ON)
				KillTimer(hWnd, 3);
			SetTimer(hWnd, 3, 1000, NULL);
			bTime = ON;
			break;
		case 40002:
			DestroyWindow(hWnd);
			break;
		}
		return FALSE;

	case WM_NewBlock:
		NowX = 3;
		NowY = 0;
		RotateNum = 0;
		BlockNum = NextBlockNum;
		NextBlockNum = rand() % 7;

		DrawBlock();
		DrawNextBlock();

		if (!BlockCanMove(NowX, NowY))
			PlayerState = DEAD;
		return FALSE;


	case WM_KEYDOWN:
		switch (LOWORD(wParam))
		{
		case VK_UP:
			Rotate();
			break;
		case VK_DOWN:
			DownMove();
			break;
		case VK_LEFT:
			LeftMove();
			break;
		case VK_RIGHT:
			RightMove();
			break;


		case VK_SPACE: //start
			while (DownMove());
			break;
		}
		return FALSE;

	case WM_TIMER:
		if (PlayerState == ALIVE)
			DownMove();
		else
		{
			if (bTime == ON)
				KillTimer(hWnd, 3);
		}
		return FALSE;

	case WM_DESTROY:
		if (bTime == ON)
			KillTimer(hWnd, 3);
		PostQuitMessage(0);
		return FALSE;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


void InitBackGround()
{
	for (int row = 0; row < 21; row++)
		for (int col = 0; col < 12; col++)
		{
			if (row == 20)
				BackGround[row][col] = 1;
			else if (col == 0)
				BackGround[row][col] = 1;
			else if (col == 11)
				BackGround[row][col] = 1;
			else
				BackGround[row][col] = 0;
		}
}

void DrawBackGround()
{
	HDC hDC = GetDC(hWnd);
	HPEN hPen, hOldPen;
	HBRUSH hBrush, hOldBrush;
	int x, y;

	hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	hBrush = CreateSolidBrush(RGB(0, 255, 0));

	hOldPen = (HPEN)SelectObject(hDC, hPen);
	hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

	for (int row = 0; row < 21; row++)
		for (int col = 0; col < 12; col++)
			if (BackGround[row][col] == 1)
			{
				x = 100 + col * 10;
				y = row * 10;
				Rectangle(hDC, x, y, x + 10, y + 10);
			}
	SelectObject(hDC, hOldPen);
	SelectObject(hDC, hOldBrush);
	DeleteObject(hPen);
	DeleteObject(hBrush);
	ReleaseDC(hWnd, hDC);
}

void DrawBlock()
{
	HDC hDC = GetDC(hWnd);
	HPEN hPen, hOldPen;
	HBRUSH hBrush, hOldBrush;
	int x, y;

	hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	hBrush = CreateSolidBrush(RGB(255, 255, 255));
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	for (int row = 0; row < 4; row++)
		for (int col = 0; col < 4; col++)
			if (Block[BlockNum][RotateNum][row][col] == 1)
			{
				x = 110 + NowX * 10 + col * 10;
				y = NowY * 10 + row * 10;
				Rectangle(hDC, x, y, x + 10, y + 10);
			}
	SelectObject(hDC, hOldPen);
	SelectObject(hDC, hOldBrush);
	DeleteObject(hPen);
	DeleteObject(hBrush);
	ReleaseDC(hWnd, hDC);
}

void EraseBlock()
{
	HDC hDC = GetDC(hWnd);
	HPEN hPen, hOldPen;
	HBRUSH hBrush, hOldBrush;
	int x, y;

	hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	hBrush = CreateSolidBrush(RGB(0, 0, 0));
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);

	for (int row = 0; row < 4; row++)
		for (int col = 0; col < 4; col++)
			if (Block[BlockNum][RotateNum][row][col])
			{
				x = 110 + NowX * 10 + col * 10;
				y = NowY * 10 + row * 10;
				Rectangle(hDC, x, y, x + 10, y + 10);
			}
	SelectObject(hDC, hOldPen);
	SelectObject(hDC, hOldBrush);
	DeleteObject(hPen);
	DeleteObject(hBrush);
	ReleaseDC(hWnd, hDC);
}

void DrawNextBlock()
{
	HDC hDC;
	HPEN hPen, hOldPen;
	HBRUSH hBrush, hOldBrush;
	int x, y;

	hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	hBrush = CreateSolidBrush(RGB(0, 0, 0));
	hDC = GetDC(hWnd);
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	Rectangle(hDC, 240, 45, 290, 90);
	SelectObject(hDC, hOldPen);
	SelectObject(hDC, hOldBrush);
	DeleteObject(hPen);
	DeleteObject(hBrush);
	ReleaseDC(hWnd, hDC);

	hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	hBrush = CreateSolidBrush(RGB(255, 255, 255));
	hDC = GetDC(hWnd);
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	for (int row = 0; row < 4; row++)
		for (int col = 0; col < 4; col++)
		{
			if (Block[NextBlockNum][RotateNum][row][col])
			{
				x = 250 + col * 10;
				y = 50 + row * 10;
				Rectangle(hDC, x, y, x + 10, y + 10);
			}
		}
	SelectObject(hDC, hOldPen);
	SelectObject(hDC, hOldBrush);
	DeleteObject(hPen);
	DeleteObject(hBrush);
	ReleaseDC(hWnd, hDC);
}

BOOL BlockCanMove(int x, int y)
{
	int check = 0;
	int row, col;
	for (row = 0; row < 4; row++)
		for (col = 0; col < 4; col++)
			if (Block[BlockNum][RotateNum][row][col])
				check += BackGround[y + row][x + col + 1];
	if (check == 0)
		return YES;
	else
		return NO;
}

void LeftMove()
{
	if (PlayerState == DEAD)
		return;
	if (BlockCanMove(NowX - 1, NowY))
	{
		EraseBlock();
		NowX--;
		DrawBlock();
	}
}

void RightMove()
{
	if (PlayerState == DEAD)
		return;
	if (BlockCanMove(NowX + 1, NowY))
	{
		EraseBlock();
		NowX++;
		DrawBlock();
	}
}

void Rotate()
{
	if (PlayerState == DEAD)
		return;
	int temp = RotateNum;
	RotateNum++;
	RotateNum %= 4;
	if (BlockCanMove(NowX, NowY))
	{
		RotateNum = temp;
		EraseBlock();
		RotateNum++;
		RotateNum %= 4;
		DrawBlock();
	}
	else
		RotateNum = temp;
}


BOOL DownMove()
{
	if (PlayerState == DEAD) return FAIL;
	if (BlockCanMove(NowX, NowY + 1))
	{
		EraseBlock();
		NowY++;
		DrawBlock();
		return SUCCESS;
	}
	else
	{
		UpdateBackGround();
		CheckFullLine();
		SendMessage(hWnd, WM_NewBlock, 0, 0);
		return FAIL;
	}
}

void UpdateBackGround()
{
	int element;
	for (int row = 0; row < 4; row++)
		for (int col = 0; col < 4; col++)
		{
			element = Block[BlockNum][RotateNum][row][col];
			if (element)
				BackGround[NowY + row][NowX + col + 1] = element;
		}
}

void CheckFullLine()
{
	int row, col, line;
	int elementNum;
	for (row = 19; row >= 0; row--)
	{
		elementNum = 0;
		for (col = 1; col <= 10; col++)
			elementNum += BackGround[row][col];
		if (elementNum == 10)
		{
			for (line = row; line > 0; line--)
				for (col = 1; col <= 10; col++)
					BackGround[line][col] = BackGround[line - 1][col];
			for (col = 1; col <= 10; col++)
				BackGround[0][col] = 0;
			EraseFullLine(row);
			FullLineNum++;
			row++;
		}
	}
}

void EraseFullLine(int row)
{
	HDC hDC, hMemDC;
	HBITMAP hBmp;
	int Xlen = 209 - 110 + 1;

	int Ylen = row * 10;

	hDC = GetDC(hWnd);
	hMemDC = CreateCompatibleDC(hDC);
	hBmp = CreateCompatibleBitmap(hDC, 100, 200);
	SelectObject(hMemDC, hBmp);

	BitBlt(hMemDC, 0, 0, Xlen, Ylen, hDC, 110, 0, SRCCOPY);
	BitBlt(hDC, 110, 10, Xlen, Ylen, hMemDC, 0, 0, SRCCOPY);

	DeleteDC(hMemDC);
	ReleaseDC(hWnd, hDC);
	DeleteObject(hBmp);
}