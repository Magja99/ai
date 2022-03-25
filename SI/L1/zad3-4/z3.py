import random

def two_pairs(L):
    L.sort()
    if L[0][0] == L[1][0] and L[2][0] == L[3][0]:# OOOOX
        return 1
    if L[0][0] == L[1][0] and L[3][0] == L[4][0]: # OOXOO
        return 1
    if L[2][0] == L[3][0] and L[3][0] == L[4][0]: # XOOOO
        return 1
    return 0

def three(L):
    L.sort()
    if L[0][0] == L[1][0] and L[1][0] == L[2][0]:# OOOXY
        return 2
    if L[1][0] == L[2][0] and L[2][0] == L[3][0]:# XOOOY
        return 2
    if L[2][0] == L[3][0] and L[3][0] == L[4][0]:# XYOOO
        return 2
    return 0

def strit(L):
    L.sort()
    for i in range(1, 5):
        if L[i][0] != L[i - 1][0] + 1:
            return 0
    return 3

def color(L):
    L.sort()
    for i in range(1, 5):
        if L[i][1] != L[i - 1][1]:
            return 0
    return 4

def full(L):
    L.sort()
    if L[0][0] == L[1][0] and L[1][0] == L[2][0] and L[3][0] == L[4][0]:# OOOXX
        return 5
    if L[0][0] == L[1][0] and L[2][0] == L[3][0] and L[3][0] == L[4][0]:# XXOOO
        return 5
    return 0

def kareta(L):
    if L[0][0] == L[1][0] and L[1][0] == L[2][0] and L[2][0] == L[3][0]:# OOOOX
        return 6
    if L[1][0] == L[2][0] and L[2][0] == L[3][0] and L[3][0] == L[4][0]:
        return 6
    return 0

def strit_and_color(L):
    if color(L) != 0 and strit(L) != 0:
        return 7
    return 0

def check(Fig, Blot, Foo):
    F = 0
    B = 0
    for i in Foo:
        F = max(F, i(Fig))
        B = max(B, i(Blot))
    if F < B:
        return "Blot"
    else:
        return "Fig"

def play(P1, P2, Foo):
    Fig = []
    Blot = []
    L1 = P1
    L2 = P2
    for i in range(5):
        index_F = random.choice(L1)
        index_B = random.choice(L2)
        Fig.append(index_F)
        Blot.append(index_B)
        L1.remove(index_F)
        L2.remove(index_B)
    if check(Fig, Blot, Foo) == 'Blot':
        return 1
    else:
        return 0

Cards = [i for i in range(2, 15)]
Colors = [1, 2, 3, 4]

Functions = [two_pairs, three, strit, color, full, kareta, strit_and_color]

ans = 0 # ogólnie prawdopodobiństwo
for i in range(100000):
    Player_1 = []
    Player_2 = []
    for i in range(9):
        for j in Colors:
            Player_2.append((Cards[i], j))

    for i in range(9, 13):
        for j in Colors:
            Player_1.append((Cards[i], j))

    ans += play(Player_1, Player_2, Functions)

print(ans / 100000)

# wysokiej karty nie będzie bo Fig ma zawsze wyższe karty niż Blot
# pary nie będzie bo Fig losuje z 4 kart więc zawsze będzie mieć parę i przebije inne pary
# pokera królewskiego nie będzie bo do tego trzeba asa, króla, królowej, jopka i 10

# najwyższy układ jaki może mieć Figurant to kareta, więc aby Blotkarz mógł wygrać musi on mieć strita w kolorze
# dlatego idealna zwycięska talia dla blotkarza będzie składać się z pięciu następujących po sobie kart o tym samym kolorze

ans = 0
for i in range(100000): #test prawdopodobieństwa dla talli ^ (1.0)
    Player_1 = []
    Player_2 = [(2, 1), (3, 1), (4, 1), (5, 1), (6, 1)]

    for i in range(9, 13):
        for j in Colors:
            Player_1.append((Cards[i], j))

    ans += play(Player_1, Player_2, Functions)

print(ans/100000)

ans = 0
for i in range(100000): #test prawdopodobieństwa dla talli gdzie wszystkie kolory są te same
    Player_1 = []
    Player_2 = []
    for i in range(9):
        Player_2.append((Cards[i], 1))
    for i in range(9, 13):
        for j in Colors:
            Player_1.append((Cards[i], j))

    ans += play(Player_1, Player_2, Functions)

print(ans/100000)

ans = 0
for i in range(100000): #test prawdopodobieństwa dla talii gdzie są tylko 8 9 10
    Player_1 = []
    Player_2 = []
    for i in range(6, 9):
        for j in Colors:
            Player_2.append((Cards[i], j))

    for i in range(9, 13):
        for j in Colors:
            Player_1.append((Cards[i], j))

    ans += play(Player_1, Player_2, Functions)

print(ans / 100000)
