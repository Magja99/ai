from itertools import *

def give_dict(val, a, b, c):
	k = len(val)
	dictionaries = []
	for i in combinations([0, 1, 2, 3, 4, 5, 6, 7, 8, 9], len(val)):
		for j in permutations(i):
				d = dict(zip(val, j))
				if d[a] != 0 and d[b] != 0 and d[c] != 0:
					dictionaries.append(d)
	return dictionaries

def to_num(s, dic):
	l = s[::-1]
	ans = 0
	for i in range(len(l)):
		ans += 10 ** i * dic[l[i]]
	return ans

def solve(riddle):
	arg1, eq1, arg2, eq2, ans = riddle.split()
	Val = set(arg1 + arg2 + ans)
	dictionaries = give_dict(Val, arg1[0], arg2[0], ans[0])
	for d in dictionaries:
		a = to_num(arg1, d)
		b = to_num(arg2, d)
		c = to_num(ans, d)
		if a + b == c:
			print(a, "+", b, "=", c)
			print(d)
			exit()


solve("send + more = money")
