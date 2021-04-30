import sys
from collections import deque
t = int(sys.stdin.readline())
queue = deque()
for _ in range(t):
    cmd, _, value = sys.stdin.readline().strip().partition(' ')
    if cmd == 'push_front':
        queue.appendleft(value)
    elif cmd == 'push_back':
        queue.append(value)
    elif cmd == 'pop_front':
        if len(queue):
            print(queue.popleft())
            # queue.popleft()
        else:
            print(-1)
    elif cmd == 'pop_back':
        if len(queue):
            print(queue.pop())
            # queue.pop()
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
            print(queue[0])
        else:
            print(-1)
    elif cmd == 'back':
        if len(queue):
            print(queue[-1])
        else:
            print(-1)