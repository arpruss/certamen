from fonts import FONTS

for f in sorted(FONTS):
    print("%s: %d"%(f,len(FONTS[f][32][3])))