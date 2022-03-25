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

def Fill_all(l, n): # wszystko zamalowane
	if len(l) == 1 and l[0] == n:
		return True
	return False

def With_gaps(l, n): # wszystko zamalowane z wiadomymi przerwami
	if sum(l) + len(l) - 1 == n:
		return True
	return False

def Our_best(l, n, place): # może możemy pomalować tylko części
	sum_l = sum(l) # suma wszystkich, które mają być zamalowane
	k = n - (sum_l + len(l) - 1) # wszystkie pola - zajęte pola (zamalowane + min ilość przerw) czyli te które się zostały wolne
	part_sum = 0
	ans = [] # przedziały, które możemy zamalować
	for i in range(len(l)):
		if l[i] - k > 0:
			add_f = 0
			add_l = 0
			while True:
				first_start = (part_sum + add_f+ i + 1, part_sum + add_f + i + l[i]) # pierwsze pole od którego możemy zacząć
				last_start = (part_sum +add_l+i + 1 + k, part_sum +add_l + i + l[i] + k) # ostatnie pole od którego możemy zacząć
				paint = (max(first_start[0] - 1, last_start[0] - 1), min(first_start[1] - 1, last_start[1] - 1)) # przedział, który możemy zamalować
				ans.append(paint)
				break
		part_sum += l[i]
	return ans

def paint(place, l): # place = (wiersz/kolumna, nr) l = przedziały do zamalowania
	global arr
	if place[0] == 'w':
		for i in l:
			for j in range(i[0], i[1] + 1):
				arr[place[1]][j] = '#'
	else:
		for i in l:
			for j in range(i[0], i[1] + 1):
				arr[j][place[1]] = '#'

def read(n, place, m):
	ans = []
	for i in range(n):
		l = list(input().split(" "))
		w = [int(x) for x in l if x != ' ' and x != '']

		if Fill_all(w, m):
			paint((place, i), [(1, m)])
		elif With_gaps(w, m):
			t = [(0, w[0] - 1)]
			for j in range(1, len(w)):
				t.append((t[j - 1][1] + 1, t[j - 1][1]  + w[j]))
			paint((place, i), t)
		else:
			t = Our_best(w, m, (place, i))
			if len(t) != 0:
				paint((place, i), t)
		ans.append(list(w))
	return ans


n, m = input().split()
n = int(n)
m = int(m)

arr = ['']*n
for i in range(n):
	arr[i] = ['O'] * m

#arr[0][4] = '#'
rows = read(n, 'w', m)
collumns = read(m, 'k', n)


for i in range(3):
	for j in range(n):
		print(''.join(arr[j]))
	for j in range(len(rows)):
		Inference_w(rows[j], m, ('w', j))

	for j in range(len(collumns)):
		Inference_k(collumns[j], n, ('k', j))
	print()


# TODO: zmienić wznioskowanie tak aby brało pod uwagę aktualny stan
