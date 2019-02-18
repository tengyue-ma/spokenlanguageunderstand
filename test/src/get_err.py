#!/usr/bin/env python
# coding=utf-8

import sys
import codecs
import evaluate

reload(sys)
sys.setdefaultencoding("utf-8")

def get_err(lab_fn, tst_fn):
    """
    该函数比较标注文件和测试结果之间的差异,
    并输出两者的不同的句子,输出格式为:
        标注结果 <=> 测试结果
    @lab_fn: 标注文件,以tab键分割,有四列或三列(domain intent query slots),slots以逗号分割
             music	inform	喜羊羊与灰太狼主题曲怎么唱呢  program=喜羊羊与灰太狼,tag=主题曲
    @tst_fn: SLU解析结果文件,以tab键分割,有七列或六列(domain intent query slots score match_mode match_time)
             music	inform	喜羊羊与灰太狼主题曲怎么唱呢  program=喜羊羊与灰太狼,tag=主题曲	1.000000    regex    60.34ms
    """
    lab = codecs.open(lab_fn, "r", "utf-8")
    tst = codecs.open(tst_fn, "r", "utf-8")

    all_wrong_num = 0
    domain_wrong_num = 0
    line_num = 0

    domain_diff = 0
    intent_diff = 0
    slots_diff = 0
    regex_num = 0
    model_num = 0

    lab_slots_exist = 0
    tst_slots_exist = 0

    print("\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>")
    print("测试集: %s" % lab_fn)
    print("行号, 标注结果 <=> SLU解析结果")

    while True:
        line_lab = lab.readline()
        line_tst = tst.readline()

        #到达文件尾
        if not (line_lab or line_tst):
            break
        line_num += 1

        #去除首尾空格,以tab键分割
        line_lab = line_lab.strip()
        line_tst = line_tst.strip()
        cols_lab = line_lab.split("\t")
        cols_tst = line_tst.split("\t")

        if len(cols_lab) != 4 and len(cols_lab) != 3:
            print("lab_file format error, line num: %f" % (line_num))
            break
        if len(cols_tst) != 7 and len(cols_tst) != 6:
            print("tst_file format error, line num: %f" % (line_num))
            break

        #计算match_mode比例
        if len(cols_tst)==7:
            match_mode = cols_tst[5]
        elif len(cols_tst)==6:
            match_mode = cols_tst[4]

        if match_mode == "regex":
            regex_num +=1
        elif match_mode == "model":
            model_num +=1
        else:
            print("tst_file match_mode wrong, line num: %f" % (line_num))

        domain_lab = cols_lab[0]
        domain_tst = cols_tst[0]
        intent_lab = cols_lab[1]
        intent_tst = cols_tst[1]
        query_lab = cols_lab[2]
        query_tst = cols_tst[2]

        #1.判断query列是否相同,不同报错
        if query_lab != query_tst:
            print("lab_file and tst_file query different, line num: %f" % (line_num))
            print("query_lab: %s" % (query_lab))
            print("query_tst: %s" % (query_tst))
            break

        #2.other 单独判断, other只要domain对就可以了
        if domain_lab == "other" and domain_tst == "other":
            continue

        #3.判断domian是否一样
        if domain_lab != domain_tst:
            domain_diff = 1
        else:
            domain_diff = 0

        #4.判断intent是否一样
        if intent_lab != intent_tst:
            intent_diff = 1
        else:
            intent_diff = 0

        #5.判断slots是否一样
        if len(cols_lab)==3:
            lab_slots_exist = 0
        elif len(cols_lab)==4:
            lab_slots_exist = 1
        if len(cols_tst)==6:
            tst_slots_exist = 0
        elif len(cols_tst)==7:
            tst_slots_exist = 1

        #标注和解析结果都没有slots列
        if (not lab_slots_exist) and (not tst_slots_exist):
            slots_diff = 0
        #标注和解析结果只有一个有slots列
        elif (lab_slots_exist and (not tst_slots_exist)) or ((not lab_slots_exist) and tst_slots_exist):
            slots_diff = 1
        #标注和解析结果都有slots列
        elif lab_slots_exist and tst_slots_exist:
            slots_lab = cols_lab[3].split(',')
            slots_tst = cols_tst[3].split(',')
            slots_lab.sort()
            slots_tst.sort()
            if slots_lab != slots_tst:
                slots_diff = 1
            else:
                slots_diff = 0

        #6.domain/intent/slots只要有一个不一样，就是失败
        if domain_diff or intent_diff or slots_diff:
            all_wrong_num += 1
            #找到最后一个tab键, err中不打印slu解析时间
            index = line_tst.rfind('\t')
            print("%d, %s <=> %s" % (line_num,line_lab,line_tst[0:index]))

        if domain_diff:
            domain_wrong_num +=1

    print("\nDomain Precision	: %.2f%%  (when domain is correct)" % ((line_num-domain_wrong_num)*100.0/line_num))
    print("\nAll Precision	: %.2f%%  (when domain/intent/slots are all correct)" % ((line_num-all_wrong_num)*100.0/line_num))
    print("model match : %.2f%%" % (model_num*100.0/line_num))
    print("regex match : %.2f%%" % (regex_num*100.0/line_num))
    lab.close()
    tst.close()

if __name__=="__main__":
    if len(sys.argv)<3:
        print("Usage:")
        print("\tpython %s  lab_file  tst_file\n" % sys.argv[0])
    else:
        get_err(sys.argv[1], sys.argv[2])
        evaluate.eval_all(sys.argv[1], sys.argv[2])

