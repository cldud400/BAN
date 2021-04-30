from collections import deque

read = deque(input())
flag = True

while len(read) > 1: #리스트의 크기가 홀수이면 1이남고 짝수면 0이남기 때문
  if read.popleft() != read.pop():
    flag = False
    break

if flag: print(1)
else: print(0)
