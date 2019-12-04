import os
import sys

for i in range(6):
    infname = "testes/arq" + "{}".format(i+1) + ".in"
    resfname = "testes/arq" + "{}".format(i+1) + ".res"
    outfname = "testes/arq" + "{}".format(i+1) + ".out"

    print("TEST", i+1)
    os.system("./wormhole < {} > {}".format(infname,outfname))
    os.system("diff {} {}".format(outfname,resfname))

print("Fim")
