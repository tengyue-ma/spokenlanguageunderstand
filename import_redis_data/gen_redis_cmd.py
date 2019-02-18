#!/usr/bin/env python
#coding=utf-8

import sys
import codecs

reload(sys)
sys.setdefaultencoding("utf-8")

def gen_redis_cmd(ifn, ofn, prefix=None):
    d = {}
    with codecs.open(ifn, "r", "utf-8") as inf:
        while True:
            line = inf.readline()
            if not line:
                break
            line = line.strip()
            if len(line)<=0:
                continue
            for i in range(1, len(line)):
            	word = line[:i]
            	if word not in d:
            		d[word] = 0
            d[line] = 1
    with codecs.open(ofn, "w", "utf-8") as ouf:
        if prefix is not None:
            for word in d:
                ouf.write("set "+prefix+"_"+word+" " + str(d[word])+"\n")
        else:
            for word in d:
                ouf.write("set "+word+" " + str(d[word])+"\n")

if __name__=="__main__":
    if len(sys.argv)<3:
        print("Usage:")
        print("\tpython %s  indir  outdir  [prefix]" % sys.argv[0])
    elif len(sys.argv)==3:
        gen_redis_cmd(sys.argv[1], sys.argv[2])
    else:
        gen_redis_cmd(sys.argv[1], sys.argv[2], sys.argv[3])
