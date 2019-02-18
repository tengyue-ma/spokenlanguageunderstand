#!/usr/bin/env python
# coding=utf-8

import sys
import codecs

reload(sys)
sys.setdefaultencoding("utf-8")

def get_classes(lab_fn, tst_fn):
    """
    获取所有的domains、intents、slots
    举例：domains：  music、poem、story、other
         intents：  inform、request、deny、
         keys：  artist、title、album、

    @lab_fn: 标注文件,以tab键分割，有四列或三列(domain intent query slots),slots以逗号分割
             music	inform	喜羊羊与灰太狼主题曲怎么唱呢  program=喜羊羊与灰太狼,tag=主题曲
    @tst_fn: SLU解析结果文件,以tab键分割,有七列或六列(domain intent query slots score match_mode match_time)
             music	inform	喜羊羊与灰太狼主题曲怎么唱呢  program=喜羊羊与灰太狼,tag=主题曲	1.000000    regex    60.34ms
    """
    lab = codecs.open(lab_fn, "r", "utf-8")
    tst = codecs.open(tst_fn, "r", "utf-8")
    domains = []; intents = []; keys = [];

    line_num = 0
    while True:
        line_lab = lab.readline(); line_tst = tst.readline()
        if not (line_lab or line_tst):
            break
        line_num += 1

        #去除首尾空格、以tab键分割
        line_lab = line_lab.strip(); line_tst = line_tst.strip()
        cols_lab = line_lab.split('\t'); cols_tst = line_tst.split('\t')

        if len(cols_lab) != 4 and len(cols_lab) != 3:
            print("lab_file format error, line num: %f" % (line_num))
            break
        if len(cols_tst) != 7 and len(cols_tst) != 6:
            print("tst_file format error, line num: %f" % (line_num))
            break

        #1.判断query列是否相同,不同报错
        if cols_lab[2] != cols_tst[2]:
            print("lab_file and tst_file query different, line num: %f" % (line_num))

        #获取domains和intents
        domain_lab = cols_lab[0]; domain_tst = cols_tst[0]
        intent_lab = cols_lab[1]; intent_tst = cols_tst[1]

        #获取slots
        slots_lab_cols = []; slots_tst_cols = []
        if len(cols_lab) == 4:
            if "=" in cols_lab[3]:
                slots_lab = cols_lab[3]
                slots_lab_cols = slots_lab.split(',')
        if len(cols_tst) == 7:
            if "=" in cols_tst[3]:
                slots_tst = cols_tst[3]
                slots_tst_cols = slots_tst.split(',')

        #append
        if domain_lab not in domains:
            domains.append(domain_lab)
        if domain_tst not in domains:
            domains.append(domain_tst)

        if intent_lab not in intents:
            intents.append(intent_lab)
        if intent_tst not in intents:
            intents.append(intent_tst)

        for i in range(len(slots_lab_cols)):
            key = slots_lab_cols[i].split('=')[0]
            if key not in keys:
                keys.append(key)
        for i in range(len(slots_tst_cols)):
            key = slots_tst_cols[i].split('=')[0]
            if key not in keys:
                keys.append(key)

    lab.close()
    tst.close()
    return domains, intents, keys

def safe_div(a,b):
    if b == 0:
        return 'inf'
    else:
        return str(float(a)/float(b))

def eval_confusion(matrix):
    """
    计算混淆矩阵
    """
    out = []
    for i in range(len(matrix)):
        tp = matrix[i][i]
        fn = sum(matrix[i]) - tp
        fp = sum(list(zip(*matrix))[i]) - tp
        tn = sum(sum(matrix,[])) - tp - fn - fp
        acc = safe_div(tp + tn, tp + fn + fp + tn)
        p = safe_div(tp, tp + fp)
        r = safe_div(tp, tp + fn)
        f1 = safe_div(2*tp, 2*tp + fp + fn)

        #转换成百分数，并保留两位小数
        acc = "%.2f%%" % (float(acc) * 100)
        p = "%.2f%%" % (float(p) * 100)
        r = "%.2f%%" % (float(r) * 100)
        f1 = "%.2f%%" % (float(f1) * 100)

        out.append([acc, p, r, f1])
    return out

def print_result(domains, intents, domain_confusion, intent_confusion, domain_result, intent_result):
    """
    打印评估结果
    """
    print('\n-*-domain confusion matrix-*-\n')
    print('matrix\t' + str(domains) + '\t\n')
    for i in range(len(domain_confusion)):
        print(domains[i] + '\t' + str(domain_confusion[i]))

    print('\n-*-intent confusion matrix-*-\n')
    print('matrix\t' + str(intents) + '\t\n')
    for i in range(len(intent_confusion)):
        print(intents[i] + '\t' + str(intent_confusion[i]))

    print('\n-*-domain result-*-\n')
    print('Evaluate\tAccuracy\tPrecision\tRecall\tF1\n')
    for i in range(len(domain_result)):
        print(domains[i] + ':\t' + '\t'.join(domain_result[i]))

    print('\n-*-intent result-*-\n')
    print('Evaluate\tAccuracy\tPrecision\tRecall\tF1\n')
    for i in range(len(intent_result)):
        print(intents[i] + ':\t' + '\t'.join(intent_result[i]))

def eval_all(lab_fn, tst_fn):
    """
    得到评估结果

    @lab_fn: 标注文件,以tab键分割，有四列或三列(domain intent query slots),slots以逗号分割
             music	inform	喜羊羊与灰太狼主题曲怎么唱呢  program=喜羊羊与灰太狼,tag=主题曲
    @tst_fn: SLU解析结果文件,以tab键分割，有六列或五列(domain intent query slots score match_mode)
             music	inform	喜羊羊与灰太狼主题曲怎么唱呢  program=喜羊羊与灰太狼,tag=主题曲	1.000000    regex
    """

    #得到标注文件和解析结果文件的所有domains、intents、slots
    lists = get_classes(lab_fn, tst_fn)
    domains = lists[0]
    intents = lists[1]
    keys = lists[2]

    # 初始化混淆矩阵
    domain_confusion = [[0 for i in domains] for j in domains]
    intent_confusion = [[0 for i in intents] for j in intents]
    #slot_count = [[[0 for i in keys] for j in domains] for k in range(2)]  # 第一行表示slot各类在标准答案中的个数，第二行表示slot各类预测正确的个数
    #slot_match_all = [[0 for i in domains] for j in range(2)]              # 各个domain的双正确样例数      整条预测正确的样例数

    #计算混淆矩阵
    lab = codecs.open(lab_fn, "r", "utf-8")
    tst = codecs.open(tst_fn, "r", "utf-8")
    while True:
        line_lab = lab.readline(); line_tst = tst.readline()
        if not (line_lab or line_tst):
            break

        #去除首尾空格、以tab键分割
        line_lab = line_lab.strip(); line_tst = line_tst.strip()
        cols_lab = line_lab.split('\t'); cols_tst = line_tst.split('\t')

        domain_lab = cols_lab[0]; domain_tst = cols_tst[0]
        intent_lab = cols_lab[1]; intent_tst = cols_tst[1]

        domain_index_lab=domains.index(domain_lab)
        domain_index_tst=domains.index(domain_tst)
        intent_index_lab=intents.index(intent_lab)
        intent_index_tst=intents.index(intent_tst)

        domain_confusion[domain_index_lab][domain_index_tst] += 1                     # confusion_matrix中第i行第j列表示真实为第i类预测成了第j类的数目

        if domain_lab == domain_tst:
            intent_confusion[intent_index_lab][intent_index_tst] += 1                 # 只计算domain分类正确的情况下的intent

    #计算评估结果
    domain_result = eval_confusion(domain_confusion)
    intent_result = eval_confusion(intent_confusion)

    #打印结果
    print_result(domains, intents, domain_confusion, intent_confusion, domain_result, intent_result)


if __name__ == '__main__':
    eval_all(sys.argv[1], sys.argv[2])