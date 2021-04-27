import pygame
from backboard import *

cell = 0

blocks = [
  [
    [0, 0, 0, 0],
    [0, 0, 0, 0],
    [1, 1, 1, 1],
    [0, 0, 0, 0]
  ],
  [
    [0, 0, 0, 0],
    [2, 0, 0, 0],
    [2, 2, 2, 0],
    [0, 0, 0, 0]
  ],
  [
    [0, 0, 0, 0],
    [0, 0, 3, 0],
    [3, 3, 3, 0],
    [0, 0, 0, 0]
  ],
  [
    [0, 0, 0, 0],
    [4, 4, 0, 0],
    [0, 4, 4, 0],
    [0, 0, 0, 0]
  ],
  [
    [0, 0, 0, 0],
    [0, 5, 5, 0],
    [5, 5, 0, 0],
    [0, 0, 0, 0]
  ],
  [
    [0, 0, 0, 0],
    [0, 6, 6, 0],
    [0, 6, 6, 0],
    [0, 0, 0, 0]
  ],
  [
    [0, 0, 0, 0],
    [0, 7, 0, 0],
    [7, 7, 7, 0],
    [0, 0, 0, 0]
  ]
]

blocks1 = [
  [
    [0, 0, 0, 0],
    [0, 0, 0, 0],
    [1, 1, 1, 1],
    [0, 0, 0, 0]
  ],
  [
    [0, 0, 0, 0],
    [2, 0, 0, 0],
    [2, 2, 2, 0],
    [0, 0, 0, 0]
  ],
  [
    [0, 0, 0, 0],
    [0, 0, 3, 0],
    [3, 3, 3, 0],
    [0, 0, 0, 0]
  ],
  [
    [0, 0, 0, 0],
    [4, 4, 0, 0],
    [0, 4, 4, 0],
    [0, 0, 0, 0]
  ],
  [
    [0, 0, 0, 0],
    [0, 5, 5, 0],
    [5, 5, 0, 0],
    [0, 0, 0, 0]
  ],
  [
    [0, 0, 0, 0],
    [0, 6, 6, 0],
    [0, 6, 6, 0],
    [0, 0, 0, 0]
  ],
  [
    [0, 0, 0, 0],
    [0, 7, 0, 0],
    [7, 7, 7, 0],
    [0, 0, 0, 0]
  ]
]


def setCell(type):
  global cell 
  cell = blocks[type][:][:]
  return cell

def setNextCell(next):
  global Nextcell 
  Nextcell = blocks1[next][:][:]
  return Nextcell


def drawBlock(screen, color, currentX, currentY):
  for i in range(4):
    for j in range(4):
      if cell[i][j] != 0:
        pygame.draw.rect( screen, color, [currentX + 4 + i * 31, currentY + j * 31, 30, 30], 0) 

def NextdrawBlock(screen, color, NextX, NextY):
  for i in range(4):
    for j in range(4):
      if Nextcell[i][j] != 0:
        pygame.draw.rect( screen, color, [NextX + 4 + i * 31, NextY + j * 31, 30, 30], 0)   

def rotate():
  cell[1][1], cell[1][2], cell[2][2], cell[2][1] = cell[2][1], cell[1][1], cell[1][2], cell[2][2]
  cell[2][0], cell[0][1], cell[1][3], cell[3][2] = cell[3][2], cell[2][0], cell[0][1], cell[1][3]
  cell[1][0], cell[0][2], cell[2][3], cell[3][1] = cell[3][1], cell[1][0], cell[0][2], cell[2][3]
  cell[0][0], cell[0][3], cell[3][3], cell[3][0] = cell[3][0], cell[0][0], cell[0][3], cell[3][3]

def isMove(x, y):
  for i in range(4):
    for j in range(4):
      if cell[i][j] != 0:
        if (i + x < 0) or (j + y < 0) or (i + x >= MAXWIDTH) or (j + y >= MAXHEIGHT):
          return False
        if backBoard[j + y][i + x] != 0:
          return False
  return True

if __name__ == '__main__':
  pass