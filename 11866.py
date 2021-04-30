import sys
from collections import deque

n, k = sys.stdin.readline().strip().split()
n, k = int(n), int(k)

queue = deque([x for x in range(1, n + 1)])
seq = []
for _ in range(n):
    for _ in range(k-1): 
        queue.rotate(-1) #k-1번만큼 왼쪽으로 회전
        # print(queue)
    seq.append(queue.popleft()) #제일 queue왼쪽 원소를 seq에 오른쪽으로 추가


print('<', end = '')
print(*seq, sep = ', ', end = '') #리스트에 있는 []를 없애는 *
print('>')
