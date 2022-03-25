def not_proper_move(p):
    if p[0] <= 0 or p[0] >= 9:
        return True
    if p[1] <= 0 or p[1] >= 9:
        return True
    return False

def vis(v):
    global S
    if (v[0], v[1], v[2], v[3]) in S:
        return True
    S.add((v[0], v[1], v[2], v[3]))
    return False

def black_king_move(k):

    for i in range(-1, 2): #jeśli jest w polu rażenia króla
        for j in range(-1, 2):
            if k[2] == (k[0][0] + i, k[0][1] + j):
                return False

    if k[2][0] == k[1][0] or k[2][1] == k[1][1]: #jeśli jest w polu rażenia wieży
        if (k[2][0] - k[1][0])**2 + (k[2][1] - k[1][1])**2 == 1: #może możemy zbić wieżę?
            for i in range(-1, 2): #sprawdzamy czy wieża jest chroniona przez króla
                for j in range(-1, 2):
                    if k[1] == (k[0][0] + i, k[0][1] + j):
                        return False
            return True #wieża nie jest chroniona
        return False # nie możemy zbić wieży

    return True

def print_moves(k, v):
    l = []
    dict[k] = v
    while dict[k] != 0:
        l.append(k)
        k = dict[k]
    l.append(k)
    for i in range(len(l) - 1, -1, -1):
        print(chr(9813), " ", chr(l[i][0][0] + 96), l[i][0][1], " ",
              chr(9814), " ", chr(l[i][1][0] + 96), l[i][1][1], " ",
              chr(9818), " ", chr(l[i][2][0] + 96), l[i][2][1], sep='')

def check_move(k, i, v):
    global Q
    global dict
    global b
    if not_proper_move(k[i]) or vis(k): # czy wyszedł poza planszę lub czy był
        return

    if i == 2 and black_king_move(k) == False: #jeśli ruszył się czarny to sprawdzamy czy mógł iść na pole
        return

    if i == 1: # jeśli był ruch białych to sprawdzamy czy mat
        d = 0
        for i in range(-1, 2):
            for j in range(-1, 2):
                w = (k[0], k[1], (k[2][0] + i, k[2][1] + j), k[3], k[4])
                if not_proper_move(w[2]): #print(k, "zły ruch króla")
                    d += 1
                elif black_king_move(w) == False:
                    d += 1
        if d == 9:
            print_moves(k, v)
            b.write(str(k[4]))
            print(k[4])
            exit()

    if (k[0][0] - k[2][0])**2 + (k[0][1] - k[2][1])**2 <= 2:
        return

    Q.append(k)
    dict[k] = v

def gen(v):
    if v[3] == 1: #jeśli białe się ruszają

        for i in range (v[1][0] + 1, 9): # gen dla wieży ->
            k = (v[0], (i, v[1][1]), v[2], 0, v[4] + 1)
            if k[1] == v[0] or k[1] == v[2]:
                break
            check_move(k, 1, v)
        for i in range (v[1][0] - 1, 0, -1): # gen dla wieży <-
            k = (v[0], (i, v[1][1]), v[2], 0, v[4] + 1)
            if k[1] == v[0] or k[1] == v[2]:
                break
            check_move(k, 1, v)
        for i in range (v[1][1] + 1, 9):
            k = (v[0], (v[1][0], i), v[2], 0, v[4] + 1) #gen dla wieży ^
            if k[1] == v[0] or k[1] == v[2]:
                break
            check_move(k, 1, v)
        for i in range (v[1][1] - 1, 0, -1): #gen dla wieży v
            k = (v[0], (v[1][0], i), v[2], 0, v[4] + 1)
            if k[1] == v[0] or k[1] == v[2]:
                break
            check_move(k, 1, v)

        for i in range(-1, 2):
            for j in range(-1, 2):
                k = ((v[0][0] + i, v[0][1] + j), v[1], v[2], 0, v[4] + 1)
                if k[0] == v[1] or k[0] == v[2]:
                    break
                if i != 0 or j != 0:
                    check_move(k, 0, v)
    else: #jeśli czarny król się rusza
        for i in range(-1, 2):
            for j in range(-1, 2):
                k = (v[0], v[1], (v[2][0] + i, v[2][1] + j), 1, v[4] + 1)
                if k[2] == v[1] or k[2] == v[0]:
                    break
                if i != 0 or j != 0:
                    check_move(k, 2, v)

def BFS(v):
    global Q
    Q.append(v)
    while len(Q) != 0:
        w = Q.pop(0)
        gen(w)

f = open("zad1_input.txt")
b = open("zad1_output.txt", "w+")
line = f.readline()
while line:
    w = line
    white_moves = 0

    if w[0] == 'w':
        white_moves = 1

    wk = (ord(w[6]) - 96, ord(w[7]) - 48)
    wr = (ord(w[9]) - 96, ord(w[10]) - 48)
    bk = (ord(w[12]) - 96, ord(w[13]) - 48)
    start = (wk, wr, bk, white_moves, 0)
    S = {(wk, wr, bk, white_moves)}
    Q = []
    dict = {start : 0}
    line = f.readline()
    BFS(start)
