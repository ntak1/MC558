import os
import sys

for i in range(8):
    infname = "testes/arq" + "{}".format(i+1) + ".in"
    resfname = "testes/arq" + "{}".format(i+1) + ".res"
    outfname = "testes/arq" + "{}".format(i+1) + ".out"

    print("TEST", i+1)
    os.system("./puzzle < {} > {}".format(infname,outfname))
    os.system("diff {} {}".format(outfname,resfname))

print("Fim")
