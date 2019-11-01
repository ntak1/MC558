import os
import sys

for i in range(5):
    infname = "testes/arq" + "{0:02d}".format(i+1) + ".in"
    resfname = "testes/arq" + "{0:02d}".format(i+1) + ".res"
    outfname = "testes/arq" + "{0:02d}".format(i+1) + ".out"

    os.system("./pill < {} > {}".format(infname,outfname))
    os.system("diff {} {}".format(outfname,resfname))

print("Fim")
