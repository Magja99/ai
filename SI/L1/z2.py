def divide(s):
    global S
    dp = [[0, '']]
    for i in range (1, len(s)):
        dp.append([0, ''])
        for j in range(i + 1):
            ss = s[j:i + 1]
            k = len(s[0:j]) - 1
            if ss in S and dp[i][0] < dp[k][0] + len(ss)**2:
                dp[i] = [dp[k][0] + len(ss)**2, dp[k][1] + " " + ss]
                #print(i, j, dp[i][1])
    print(dp[len(s) - 1][1])

f = open("words_for_ai1.txt", "r")
line = f.readline()
S = set()
S.add(line)
nad = 0
while line:
    line = f.readline()
    if len(line) - 1 > 0 and line[len(line) - 1] == '\n':
        line = line[:-1]
    S.add(line)


f = open("pantadeusz", "r")
line = f.readline()
ans = ''
maks_dl = 0
divide(line)
print(ans)
ans = ''
maks_dl = 0
while line:
    line = f.readline()
    if len(line) - 1 > 0 and line[len(line) - 1] == '\n':
        line = line[:-1]
    ans = ''
    maks_dl = 0
    divide(line)
    print(ans)
#
# line = input()
# if len(line) - 1 > 0 and line[len(line) - 1] == '\n':
#     line = line[:-1]
# ans = ''
# maks_dl = 0
# divide(line)
# print(ans)
