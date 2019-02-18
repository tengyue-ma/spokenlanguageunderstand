#!/usr/bin/env python
# coding=utf-8

import sys
import codecs

reload(sys)
sys.setdefaultencoding("utf-8")

def filecmd2pipe(ifn, ofn):
    with open(ifn, "r") as inf, \
            codecs.open(ofn, "w", "utf-8") as ouf:
        while True:
            line = inf.readline()
            if not line:
                break
            line = line.strip()
            cols = line.split()
            ouf.write("*"+str(len(cols)) + "\r\n")
            for i in range(len(cols)):
                ouf.write("$"+str(len(str(cols[i])))+"\r\n")
                ouf.write(cols[i].strip()+"\r\n")

if __name__=="__main__":
    if len(sys.argv)<3:
        print("Usage:")
        print("\tpython %s  indir  oudir" % sys.argv[0])
    else:
        filecmd2pipe(sys.argv[1], sys.argv[2])
