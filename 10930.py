import sys, hashlib
read = sys.stdin.readline().rstrip()
# print(type(read.encode()))
print(hashlib.sha256(read.encode()).hexdigest())