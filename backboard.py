import pygame

MAXWIDTH = 13
MAXHEIGHT = 26

backBoard = [[0 for _ in range(MAXWIDTH)] for _ in range(MAXHEIGHT)]

colorTable = [
    (0xFF, 0x00, 0x00),    #RED
    (0xFF, 0xEB, 0x5A),    #YELLO
    (0x52, 0xE2, 0x52),    #GREEN
    (0X00, 0X00, 0XCD),    #BLUE
    (0XFF, 0X7A, 0X85),    #PINK
    (0X8B, 0X45, 0X13),    #ORANGE
    (0x94, 0X00, 0XD3)     #VIOLET   
]

def drawBackBoard(screen):
    for i in range(MAXWIDTH):
        for j in range(MAXHEIGHT):
            if backBoard[j][i] != 0:
                pygame.draw.rect( screen, colorTable[backBoard[j][i] - 1], [i * 31, j * 31, 30, 30], 0)    
    
    # for i in range(MAXHEIGHT):
    #        pygame.draw.line(screen, (0xa9,0xa9,0xa9), (0, i * 30.9),(403, i * 30.9), width=4)
            

    for i in range(MAXWIDTH + 1):
            pygame.draw.line(screen, (0xa9,0xa9,0xa), (i*30.9, 0),(i * 30.9, 805), width=4)