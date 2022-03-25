def bigrams(a, b):
	dict_a = dict()
	dict_b = dict()
	for i in range(1, len(a)):
		if (a[i - 1], a[i]) not in dict_a:
			dict_a[(a[i - 1], a[i])] = 1

	for i in range(1, len(b)):
		if (b[i - 1], b[i]) not in dict_b:
			dict_b[(b[i - 1], b[i])] = 1

	if dict_b == dict_a:
		return True
	else:
		return False

def similar(pattern, l):
	same_size = filter(lambda i: len(i) == len(pattern) and bigrams(pattern, i), l)
	print(list(same_size))

similar([1, 2, 1], [[2, 1, 2], [1, 2, 3, 1], [2]])
