t = int(input())

for _ in range(t):
  left, right = input().split()
  if sorted(left) == sorted(right):
    print(f'{left} & {right} are anagrams.')
  else:
    print(f'{left} & {right} are NOT anagrams.')
