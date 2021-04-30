read = input()
size = len(read)
result = ''
def findPallindrome(left, right):
  while left >= 0 and right < len(read) and read[left] == read[right]:
    left -= 1
    right += 1
  return read[left + 1:right]
if size < 2:
  print(1)
else:
  for i in range(size - 1):
    result = max(result, findPallindrome(i, i+1), findPallindrome(i, i+2))
  print(len(result))
