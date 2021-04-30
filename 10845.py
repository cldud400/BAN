import sys
from collections import deque #시간초과 때문에 deque내장함수 사용한다
n = int(sys.stdin.readline()) #갯수 입력
queue = deque()
for _ in range(n):   #갯수만큼 동작
    cmd, _, value = sys.stdin.readline().strip().partition(' ')
    if cmd == 'push':
        # queue.insert(0, value) #0번째에 value추가
        queue.appendleft(value)     #왼쪽에 value추가
    elif cmd == 'pop':
        if len(queue):
            print(queue.pop())
        else:
            print(-1)
    elif cmd == 'size':
        print(len(queue))
    elif cmd == 'empty':
        if len(queue):
            print(0)
        else:
            print(1)
    elif cmd == 'front':
        if len(queue):
            print(queue[-1])
        else:
            print(-1)
    elif cmd == 'back':
        if len(queue):
            print(queue[0])
        else:
            print(-1)