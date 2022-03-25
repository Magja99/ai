def opt_dist(s):
    l = []
    dl = ord(s[len(s) - 1]) - 48
    s = s[:-1]
    pref = [0]
    j = 0
    ans = 1000000
    global w

    for i in s:
        if i == '0' or i == '1':
            l.append(i)

    for i in range(len(l)): # liczenie sum prefiksowych by wiedzieć ile jedynek jest po naszej lewej/prawej
        if l[i] == '1':
            pref.append(pref[i] + 1)
        else:
            pref.append(pref[i])

    for i in range(len(l)): # gąsiennica - będąc w przedziale <i, j> sprawdzamy co możemy zmienić w środku i po bokach
        while j - i + 1 < dl: # potem przesuwamy się do <i + 1, j + 1> i znowu
            j += 1
        if j >= len(l):
            break
        ans = min(ans, pref[i] + pref[len(l)] - pref[j + 1] + dl - (pref[j + 1] - pref[i]))

    w.write(str(ans))
    w.write("\n")

f = open("zad4_input.txt")
w = open("zad4_output.txt", "w+")

line = f.readline()
while line:
    line = line[:-1]
    opt_dist(line)
    line = f.readline()
