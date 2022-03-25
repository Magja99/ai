import sys
import copy
import itertools
import cProfile
import time
from copy import deepcopy
row_var = []
row_domain = []
col_var = []
col_domain = []
lk = 0
lw = 0

def inference(domain, arr, d, e):
	l = []
	new_domain = []
	#print(arr, domain)
	for i in domain:
		invalid = False
		for j in range(len(i)):
			if i[j] != arr[j] and arr[j] != -1:
				invalid = True
				break
		if not invalid:
			if len(l) == 0:
				l = list(i)
			for j in range(len(i)):
				if l[j] != i[j]:
					l[j] = -1

			new_domain.append(i)
	# print(l, new_domain)
	# print()
	return (l, new_domain)

def think(arr, rows, column):

	while True:
		change = False
		new_row_domain = []
		new_col_domain = []
		for i in range(n):
			l = []
			for j in range(m):
				l.append(arr[i][j])
			ret  = inference(rows[i], l, i, 'r')
			if len(ret[0]) == 0:
				return ([], [])
			new_row_domain.append(ret[1])
			for j in range(m):
				if arr[i][j] != ret[0][j]:
					change = True
				arr[i][j] = ret[0][j]

		for i in range(m):
			l = []
			for j in range(n):
				l.append(arr[j][i])
			ret = inference(column[i], l, i, 'k')
			if len(ret[0]) == 0:
				return ([], [])
			new_col_domain.append(ret[1])
			for j in range(n):
				if arr[j][i] != ret[0][j]:
					change = True
				arr[j][i] = ret[0][j]
		if change is False:
			break

	return (new_row_domain, new_col_domain)

def print_ans(arr):
	for i in range(n):
		for j in range(m):
			if arr[i][j] == -1:
				print('O', end='')
			if arr[i][j] == 0:
				print('.', end='')
			if arr[i][j] == 1:
				print('#', end='')
		print()
	print()

def consistent(val, l):
	for i in range(len(l)):
		if val[i] == l[i] or l[i] == -1:
			continue
		else:
			return False
	return True

memo = dict()

def backtrack(arr, rows, columns, deep):
	# if str(arr) in memo:
	# 	return
	# memo[str(arr)] = 1
	print_ans(arr)
	best = 1000000000
	next = -1
	which = 0
	if deep % 2 == 0:
		for i in range(n):
			if len(rows[i]) < best and len(rows[i]) != 1:
				next = i
				best = len(rows[i])
	else:
		for i in range(m):
			if len(columns[i]) < best and len(columns[i]) != 1:
				next = i
				best = len(columns[i])
				which = 1

	if next == -1:
		for i in range(n):
			for j in range(m):
				if arr[i][j] == -1:
					file_out.write("O")
				if arr[i][j] == 0:
					file_out.write(".")
				if arr[i][j] == 1:
					file_out.write("#")
			file_out.write("\n")
		exit(0)
	new = deepcopy(arr)

	print(which, next, best)
	if which == 0:
		for r in rows[next]:
			new[next] = r
			print_ans(new)
			domains = think(new, rows, columns)
			if domains[0] != []:
				backtrack(new, domains[0], domains[1], deep + 1)
			new = deepcopy(arr)
	else:
		for c in columns[next]:
			for i in range(n):
				new[i][next] = c[i]
			# print_ans(new)
			# print_ans(arr)
			domains = think(new, rows, columns)
			if domains[0] != []:
				backtrack(new, domains[0], domains[1], deep + 1)
			new = deepcopy(arr)



def give_domain(length, specifications):
	placki = []
	for i in specifications:
		for j in range(i):
			placki.append(1)
		placki.append(0)
	placki.pop()

	insert_indices = [i + 1 for i, x in enumerate(placki) if x == 0]
	insert_indices.extend([0, len(placki)])
	combinations = itertools.combinations_with_replacement(insert_indices, length - len(placki))

	for c in combinations:
		result = placki[:]
		insert_positions = list(c)
		insert_positions.sort()
		offset = 0
		for index in insert_positions:
			result.insert(index + offset, 0)
			offset += 1
		domain.append(result)

file = open("zad_input.txt")
file_out = open("zad_output.txt", 'w')
n, m = file.readline().split()
n = int(n)
m = int(m)
print(n, m)
arr = [-1] * n
for i in range(n):
    arr[i] = [-1] * m

for i in range(n):
	l = map(int, file.readline().split())
	domain = []
	t = []
	give_domain(m, l)
	row_var.append(list(l))
	row_domain.append(list(domain))
	lw += len(domain)

for i in range(m):
	l = map(int, file.readline().split())
	domain = []
	t = []
	give_domain(n, l)
	col_var.append(list(l))
	col_domain.append(list(domain))
	lk += len(domain)

backtrack(arr, row_domain, col_domain, 0)
