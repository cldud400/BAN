import sys, pygame
import random
from block import *
from backboard import *

pygame.init()

# size = width, height = 398, 805
size = width, height = 600, 805

pygame.display.set_caption("Tetris")




screen = pygame.display.set_mode(size)


delay = 200
interval = 50
pygame.key.set_repeat(delay, interval)

x = 120
y = 0
currentX = 4
currentY = 0

NextX = 440
NextY = 40

defaultTime = 0
levelTime = 0
speed = 0.5
clock = pygame.time.Clock()

game_font_1 = pygame.font.Font(None, 40) # 폰트 객체 생성(폰트, 크기)
game_font_2 = pygame.font.SysFont('consolas', 15) # 폰트 객체 생성(폰트, 크기)
isTextView = True



def freeze():
  for i in range(4):
    for j in range(4):
      if cell[i][j] != 0:
          backBoard[j + currentY][i + currentX] = cell[i][j]

# gameLines = 0
def checkLine():
    flag = False
    global score
    score = 0
    # line = 0
    for i in range(MAXHEIGHT):
        flag = True
        for j in range(MAXWIDTH):
            if backBoard[i][j] == 0:
                flag = False
        if flag:
            for p in range(i, 0, -1):
                for q in range(MAXWIDTH):
                    backBoard[p][q] = backBoard[p-1][q]
            score += 100
    print (score)
    #         line += 1

    # global gameLevel
    # global gameLines
    # gameLines += line
    # if gameLines > 0 and gameLines % 10 == 0:
    #     gameLevel += 1

  


type = random.randint(0, 6)
next = random.randint(0, 6)
cell = setCell(type)
Nextcell = setNextCell(next)


while 1:

    # speed += clock.get_rawtime()
    defaultTime += clock.get_rawtime()
    levelTime += clock.get_rawtime()
    clock.tick()
    checkLine()

    scores = game_font_1.render(str(score), 1, (0,0,0))
    score_text_box = game_font_2.render("score", 1, (0,0,0))


    if isTextView: # 텍스트 표시 여부
        screen.blit(scores, (500, 250)) # 텍스트 출력
        screen.blit(score_text_box, (440, 250))

    if levelTime / 1000 > 10:
        levelTime = 0
        speed -= 0.01

    if defaultTime / 1000 > speed:
        defaultTime = 0
        y += 31
        currentY += 1
        if not isMove(currentX, currentY):
            y -= 31
            currentY -= 1
            freeze()
            x = 120
            y = 0
            currentX = 4
            currentY = 0
            type = next
            cell = setCell(type)
            # type = random.randint(0, 6)
            next = random.randint(0, 6)
            NextX = 440
            NextY = 40
            Nextcell = setNextCell(next)
            
            
            if not isMove(currentX, currentY):
                font1 = pygame.font.SysFont('consolas', 50)
                text1 = font1.render('GAMEOVER!!!', 1, (0,0,0))
                screen.blit(text1, (70, 200))
                pygame.display.update()
                pygame.time.delay(2000)
                break

    for event in pygame.event.get():
        if event.type == pygame.QUIT: sys.exit()
        
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_RIGHT:
                x += 31
                currentX += 1
                if not isMove(currentX, currentY):
                    x -= 31
                    currentX -= 1
            elif event.key == pygame.K_LEFT:
                x -= 31
                currentX -= 1
                if not isMove(currentX, currentY):
                    x += 31
                    currentX += 1
            elif event.key == pygame.K_UP:
                rotate()
                if not isMove(currentX, currentY):
                    for _ in range(3):
                        rotate()
                    
            elif event.key == pygame.K_DOWN:
                y += 31
                currentY += 1
                if not isMove(currentX, currentY):
                    y -= 31
                    currentY -= 1

            elif event.key == pygame.K_SPACE:
                while isMove(currentX, currentY):
                    y += 31
                    currentY += 1
                y -= 31
                currentY -= 1
    pygame.display.update()
                 


    
    
    screen.fill((255,255,255))
    drawBlock(screen, colorTable[type], x, y)
    NextdrawBlock(screen, colorTable[next], NextX, NextY)
    pygame.draw.line(screen, (0, 0, 0), (402.5,0), (402.5,805))
    # pygame.draw.rect(screen, (0, 0, 0), (402.5, 300, 200, 605))
    drawBackBoard(screen)
    pygame.display.flip()