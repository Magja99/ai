from itertools import *

def variables(F):
	return list(filter(lambda x: ord(x) >= 97 and ord(x) <= 122 and x != 'v', F))

def TF(n):
	l = n * [[False, True]]
	return product(*l)

def vals(var):
	n = len(var)
	ans = []
	for i in TF(n):
		ans.append(dict(zip(var, i)))
	return ans

def value(F, w):
	F = F.replace('^', ' and ')
	F = F.replace('v', ' or ')
	F = F.replace('-', ' not ')
	return eval(F, w)

def satisfiable(F):
	z = variables(F)
	w = vals(z)
	return any(value(F, i) for i in w)

#f = input()
print(satisfiable("a ^ (b v -c)"))
print(satisfiable("a ^ a"))
print(satisfiable("a ^ -a"))
