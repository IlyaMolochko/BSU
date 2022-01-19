n = 1000
with open("in.txt", "w") as fout:
    for i in range(1, n):
        fout.write("id%d\n" % (i))
