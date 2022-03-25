import sys
import copy
import itertools
import cProfile
import time

row_var = []
row_domain = []
col_var = []
col_domain = []
lk = 0
lw = 0

# def AC3(X, D): #X[0] - rows, X[1] - columns, same with Domains
#
# 	queue = []
#
# 	def Revise(v): #returns true if we updated the row/col_domain
# 		revised = False
# 		#print(v[2], v[3], "\n", sep = " ")
# 		for x in D[v[2][1]][v[2][0]]:
# 			satisfied = False
# 			for y in D[v[3][1]][v[3][0]]:
# 				if x[v[3][0]] == y[v[2][0]]:
# 					satisfied = True
# 					break
# 			if satisfied == False:
# 				D[v[2][1]][v[2][0]].remove(x)
# 				revised = True
# 			#	print(x, D[v[3][1]][v[3][0]])
# 		return revised
#
# 	for i in range(n):
# 		for j in range(m):
# 			queue.append((X[0][i], X[1][j], (i, 0), (j, 1)))
#
# 	#queue = [(X[0][0], X[1][0], (0, 0), (0, 1))] # X0, X1, (num, 0-row, 1-column)
# 	while len(queue) != 0:
# 		v = queue.pop(0)
# 		if Revise(v):
# 			if len(D[0]) == 0:
# 				return False # imposible to solve nonogram
# 			k = m
# 			if v[2][1] == 1:
# 				k = n
# 			for i in range(k):
# 				if v[2][1] == 0:
# 					if i != v[3][0]: # if column differs from the one we revised
# 						queue.append((X[1][i], v[0], (i, 1), v[2])) # column, our row, (num, column), (num row)
# 				if v[2][1] == 1: #if we revised column with rows
# 					if i != v[3][0]: # if row differs from the one we revised
# 						#print(k)
# 						queue.append((X[0][i], v[0], (i, 0), v[2]))
#
# 	lw = 0
# 	#print("WIERSZE: ")
# 	for i in D[0]:
# 		lw += len(i)
# 		#print(i)
#
# 	lk = 0
# 	#print("KOLUMNY: ")
# 	for i in D[1]:
# 		lk += len(i)
# 		#print(i)
#
# 	print("WIERSZE: ", lw, "\n", sep='')
# 	print("KOLUMNY: ", lk, "\n", sep='')
# 	return True

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

def inference(domain, arr, d, e):
	l = []
	new_domain = []
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

	return (l, new_domain)

file = open("zad_input.txt")
file_out = open("zad_output.txt", 'w')
n, m = file.readline().split()
n = int(n)
m = int(m)

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

while True:
	change = False
	for i in range(n):
		l = []
		for j in range(m):
			l.append(arr[i][j])
		ans, row_domain[i]  = inference(row_domain[i], l, i, 'r')
		for j in range(m):
			if arr[i][j] != ans[j]:
				change = True
			arr[i][j] = ans[j]

	for i in range(m):
		l = []
		for j in range(n):
			l.append(arr[j][i])
		ans, col_domain[i]  = inference(col_domain[i], l, i, 'k')
		for j in range(n):
			if arr[j][i] != ans[j]:
				change = True
			arr[j][i] = ans[j]

	for i in range(n):
		for j in range(m):
			if arr[i][j] == -1:
				print('O', end='')
			if arr[i][j] == 0:
				print('.', end='')
			if arr[i][j] == 1:
				print('#', end='')
		print()
	if change is False:
		for i in range(n):
			for j in range(m):
				if arr[i][j] == -1:
					file_out.write("O")
				if arr[i][j] == 0:
					file_out.write(".")
				if arr[i][j] == 1:
					file_out.write("#")
			file_out.write("\n")
		break
