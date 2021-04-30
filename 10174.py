import sys

n = int(sys.stdin.readline().rstrip())

for _ in range(n):
    read = sys.stdin.readline().rstrip().lower()
    if read == read[::-1]:
        print('Yes')
    else: print('No')
    
    


        



