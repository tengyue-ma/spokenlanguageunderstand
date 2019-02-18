#!/usr/bin/env python
# coding=utf-8

import sys
import codecs
import time

'''
用来解析slu的正确标记文件或者测试结果文件一行, 将有关格式的东西放在该函数处理
is_lab为True表示标记，为False表示测试结果
'''
class Semantic:
    def __init__(self, text, is_lab):
        self.text = text
        self.is_lab = is_lab
        self.idx = 0 if is_lab else 1
        valid_cols = [[3, 4], [6, 7]]
        cols_num_if_have_slots = [4, 7]
        cols = text.split('\t')
        if len(cols) not in valid_cols[self.idx]:
            self.is_valid = False
            return
        self.is_valid = True
        self.domain = cols[0]
        self.intent = cols[1]
        self.query = cols[2]
        self.slot_names = []
        self.slots = []
        if len(cols) == cols_num_if_have_slots[self.idx] and '=' in cols[3]:
            slots = cols[3]
            slots_cols = slots.split(',')
            for i in range(len(slots_cols)):
                pair = slots_cols[i].split('=')
                slot_name = pair[0]
                slot_value = pair[1]
                self.slots.append((slot_name, slot_value))
                self.slot_names.append(slot_name)
        if not self.is_lab:
            # match_mode: regex/model
            # time_cost_str: 耗时如“100ms”
            time_cost_str = ''
            if len(cols)==7:
                self.match_mode = cols[5]
                time_cost_str = cols[6]
            elif len(cols)==6:
                self.match_mode = cols[4]
                time_cost_str = cols[5]
            self.time_cost = float(time_cost_str[0:-2])
            # text内容不保留测试时间
            index = text.rfind('\t')
            self.text = text[0:index]

'''
展示序列标注的badcase的格式
'''
def display(query, slot_pair_list):
    a = []
    for pair in slot_pair_list:
        a.append(pair[0] + '=' + pair[1])
    return query + '\t' + ','.join(a)

'''
统计slot_name,slot_value对的出现次数
input:
    [(focus, 热), (focus, 热)]
output:
    {'focus:热':2}
'''
def get_slots_count(slots, consider_multi=True):
    m = {}
    for pair in slots:
        slot_name = pair[0]
        slot_val = pair[1]
        key = slot_name + ':' + slot_val
        if key in m and consider_multi:
            m[key] += 1
        else:
            m[key] = 1
    return m


'''
评估一个领域的标注性能（评估时考虑slots出现多次）
query_lab_list: 正确标记中的原始query的列表
query_tst_list: 预测结果中的原始query的列表
slots_lab_list: 正确标记中的(slotname, slotvalue)这一tuple的列表
slots_tst_list: 预测结果中的(slotname, slotvalue)这一tuple的列表
'''
def slotfilling_evaluate(query_lab_list, query_tst_list, slots_lab_list, slots_tst_list):

    assert(len(query_lab_list) == len(query_tst_list))
    assert(len(slots_lab_list) == len(slots_tst_list))
    assert(len(query_lab_list) == len(slots_lab_list))

    tst_number = len(query_lab_list)
    consider_multi = True

    TP = 0
    FP = 0
    FN = 0
    TP_SLOTS = {}
    FP_SLOTS = {}
    FN_SLOTS = {}
    wrong_cases = []
    SLOT_LIST = set()
    line_id = 0


    for k in range(tst_number):

        query_lab = query_lab_list[k]
        query_tst = query_tst_list[k]
        slots_lab = slots_lab_list[k]
        slots_tst = slots_tst_list[k]

        line_id += 1
        if query_lab != query_tst:
            print(query_lab, ' <=> ',query_tst, 'query不匹配')
            continue
        
        lab_slot_map = get_slots_count(slots_lab, consider_multi)
        pred_slot_map = get_slots_count(slots_tst, consider_multi)
            
        wrong = False

        # traverse slots in label file (to calculate TP and FN)    
        for pair in lab_slot_map:
            slot_name, slot_val = pair.split(":")
            SLOT_LIST.add(slot_name)
            lab_cnt = lab_slot_map[pair]
            pred_cnt = pred_slot_map[pair] if pair in pred_slot_map else 0
            # update TP (真实与预测slot-value pairs完全匹配的次数)
            tmp = min(lab_cnt, pred_cnt)
            TP += tmp
            if slot_name in TP_SLOTS:
                TP_SLOTS[slot_name] += tmp
            else:
                TP_SLOTS[slot_name] = tmp
            # update FN (漏预测次数)
            if pred_cnt < lab_cnt:
                wrong = True
                FN += lab_cnt - pred_cnt
                if slot_name in FN_SLOTS:
                    FN_SLOTS[slot_name] += lab_cnt - pred_cnt
                else:
                    FN_SLOTS[slot_name] = lab_cnt - pred_cnt
        # traverse slots in pred file (to calculate FP)
        for pair in pred_slot_map:
            slot_name, slot_val = pair.split(":")
            SLOT_LIST.add(slot_name)
            lab_cnt = lab_slot_map[pair] if pair in lab_slot_map else 0
            pred_cnt = pred_slot_map[pair]
            # update FP (多预测次数)
            if lab_cnt < pred_cnt:
                wrong = True
                FP += pred_cnt - lab_cnt
                if slot_name in FP_SLOTS:
                    FP_SLOTS[slot_name] += pred_cnt - lab_cnt
                else:
                    FP_SLOTS[slot_name] = pred_cnt - lab_cnt
        if wrong:
            wrong_cases.append(str(line_id) + ', ' + display(query_lab, slots_lab) + '\t<=>\t' + display(query_tst, slots_tst))


    SLOT_LIST = sorted(list(SLOT_LIST))

    precision = {}
    recall = {}
    f1 = {}

    # calculate evaluation metrics and output
    precision['total'] = safe_divide(TP, TP + FP)
    recall['total'] = safe_divide(TP, TP + FN)
    f1['total'] = safe_divide(2 * TP, 2 * TP + FN + FP)


    for slot in SLOT_LIST:
        tp = TP_SLOTS[slot] if slot in TP_SLOTS else 0
        fp = FP_SLOTS[slot] if slot in FP_SLOTS else 0
        fn = FN_SLOTS[slot] if slot in FN_SLOTS else 0
        precision[slot] = safe_divide(tp, tp + fp)
        recall[slot] = safe_divide(tp, tp + fn)
        f1[slot] = safe_divide(2 * tp, 2 * tp + fn + fp)

    return precision, recall, f1, SLOT_LIST, wrong_cases

"""
获取所有的domains, intents, slots
举例 domains:  music, poem, story, other
     intents:  inform, request, deny
     slot_names:  artist, title, album

@lab_fn: 标注文件,以tab键分割，有四列或三列(domain intent query slots),slots以逗号分割
         music	inform	喜羊羊与灰太狼主题曲怎么唱呢  program=喜羊羊与灰太狼,tag=主题曲
@tst_fn: SLU解析结果文件,以tab键分割,有七列或六列(domain intent query slots score match_mode match_time)
         music	inform	喜羊羊与灰太狼主题曲怎么唱呢  program=喜羊羊与灰太狼,tag=主题曲	1.000000    regex    60.34ms
"""
def get_classes(lab_fn, tst_fn):
    lab = codecs.open(lab_fn, "r", "utf-8")
    tst = codecs.open(tst_fn, "r", "utf-8")
    domains = set()
    intents = set()
    slot_names = set()

    files = [lab, tst]
    files_name = ['lab_file', 'tst_file']
    for i in range(len(files)):
        line_num = 0
        while(True):
            line = files[i].readline()
            if not line:
                break
            line_num += 1
            line = line.strip()
            # 解析语义结果
            semantic = Semantic(line, i==0)

            if not semantic.is_valid:
                print(files_name[i] + ' format error, line num : %f' % (line_num))
                exit(0)
            domains.add(semantic.domain)
            intents.add(semantic.intent)
            # slots
            for slot_name in semantic.slot_names:
                slot_names.add(slot_name)
        files[i].close()
    domains = sorted(list(domains))
    intents = sorted(list(intents))
    slot_names = sorted(list(slot_names))
    return domains, intents, slot_names


"""
计算混淆矩阵
"""
def eval_confusion(matrix):
    out = []
    macro_F1 = 0.0
    for i in range(len(matrix)):
        tp = matrix[i][i]
        fn = sum(matrix[i]) - tp
        fp = sum(list(zip(*matrix))[i]) - tp
        tn = sum(sum(matrix, [])) - tp - fn - fp
        acc = safe_divide(tp + tn, tp + fn + fp + tn)
        p = safe_divide(tp, tp + fp)
        r = safe_divide(tp, tp + fn)
        f1 = safe_divide(2*tp, 2*tp + fp + fn)

        #转换成百分数，并保留两位小数
        acc = "%.2f%%" % (float(acc) * 100)
        p = "%.2f%%" % (float(p) * 100)
        r = "%.2f%%" % (float(r) * 100)
        macro_F1 += float(f1)
        f1 = "%.2f%%" % (float(f1) * 100)
        out.append([acc, p, r, f1])
    macro_F1 = macro_F1*100 / len(matrix)
    return out, macro_F1

"""
打印domain、intent分类混淆矩阵，以及分类评价指标
"""
def print_clf_confusion_matrix(domains, intents, domain_confusion, intent_confusion, domain_result, intent_result):
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

'''
以字符串返回除法结果，并处理除0的显示问题
'''
def safe_divide(dividend, divisor):
    if divisor == 0:
        return "Nan"
    return round(dividend / divisor, 3)


"""
计算slu评价指标，包括分类和序列标注；输出badcase，包括分类和序列标注，格式为"标注结果 <=> 测试结果"
lab_fn: 标注文件,以tab键分割,有四列或三列(domain intent query slots),slots以逗号分割
         music	inform	喜羊羊与灰太狼主题曲怎么唱呢  program=喜羊羊与灰太狼,tag=主题曲
tst_fn: SLU解析结果文件,以tab键分割,有七列或六列(domain intent query slots score match_mode match_time)
sl_domains: 使用标注模型进行slots解析的领域（排除使用规则进行slots解析的领域）
music	inform	喜羊羊与灰太狼主题曲怎么唱呢  program=喜羊羊与灰太狼,tag=主题曲	1.000000    regex    60.34ms
"""
def slu_evaluate(lab_fn, tst_fn, sl_domains):
    domains, intents, slot_names = get_classes(lab_fn, tst_fn)

    # 初始化混淆矩阵
    domain_confusion = [[0 for i in domains] for j in domains]
    intent_confusion = [[0 for i in intents] for j in intents]
    
    lab = codecs.open(lab_fn, "r", "utf-8")
    tst = codecs.open(tst_fn, "r", "utf-8")

    # domain wrong
    domain_wrong_num = 0
    # domain wrong or intent wrong
    domain_or_intent_wrong_num = 0
    # domain wrong or intent wrong or slots wrong
    domain_or_intent_or_slots_wrong_num = 0
    dw = []
    diw = []

    line_num = 0

    domain_diff = 0
    intent_diff = 0
    slots_diff = 0
    regex_num = 0
    model_num = 0
    tst_invalid_num = 0
    tst_not_found_num = 0
    all_num = 0

    print("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>")
    print("SLU性能评估")
    print("时间戳: " + time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(time.time())))
    print("测试集lab文件: %s" % lab_fn)

    # 读取lab文件
    lab_map = {}
    line_num = 0
    while True:
        line_lab = lab.readline()
        if not line_lab:
            break
        line_lab = line_lab.strip()
        line_num += 1
        semantic = Semantic(line_lab, True)
        if semantic.is_valid:
            lab_map[semantic.query] = semantic
        else:
            print('invalid lab line: ' + line_lab + ', line: ' + str(line_num))

    # 保存每个领域的标注结果
    sl_data = {}
    
    # 花费时间
    time_cost = {'model':0.0, 'regex':0.0, 'total':0.0}

    # 数目
    total_number = {'model':0, 'regex':0, 'total':0}

    # 最大耗时
    max_cost = {'time_cost':-1, 'query':'', 'match_mode':''}
    min_cost = {'time_cost':-1, 'query':'', 'match_mode':''}

    # 读取测试结果文件
    while True:
        line_tst = tst.readline()
        if not line_tst:
            break
        line_tst = line_tst.strip()
        tst_semantic = Semantic(line_tst, False)
        if not tst_semantic.is_valid:
            print('invalid tst line: ' + line_tst)
            tst_invalid_num += 1
            continue
        if tst_semantic.match_mode == 'regex':
            regex_num += 1
        elif tst_semantic.match_mode == 'model':
            model_num += 1
        else:
            print('invalid match_mode: ' + tst_semantic.match_mode + ' in line: ' + line_tst)

        # 未找到正确label，不计入测试
        if tst_semantic.query not in lab_map:
            print('tst query not found in lab file: ' + tst_semantic.query)
            tst_not_found_num += 1
            continue
        all_num += 1

        lab_semantic = lab_map[tst_semantic.query]


       
        # 按正确标记的domain划分结果
        domain = lab_semantic.domain
        tst_domain = tst_semantic.domain
        query_lab = lab_semantic.query
        slots_lab = lab_semantic.slots
        query_tst = tst_semantic.query
        slots_tst = tst_semantic.slots
        if domain == tst_domain:
            if domain not in sl_data:
                sl_data[domain] = [(query_lab, slots_lab, query_tst, slots_tst)]
            else:
                sl_data[domain].append((query_lab, slots_lab, query_tst, slots_tst))

        # 统计耗时（按正确标注的领域划分领域）
        time_cost[tst_semantic.match_mode] += tst_semantic.time_cost
        time_cost['total'] += tst_semantic.time_cost
        total_number[tst_semantic.match_mode] += 1
        total_number['total'] += 1
        if domain not in time_cost:
            time_cost[domain] = {'model':0.0, 'regex':0.0, 'total':0.0}
            total_number[domain] = {'model':0, 'regex':0, 'total':0}
        time_cost[domain][tst_semantic.match_mode] += tst_semantic.time_cost
        time_cost[domain]['total'] += tst_semantic.time_cost
        total_number[domain][tst_semantic.match_mode] += 1
        total_number[domain]['total'] += 1
        if max_cost['time_cost'] < 0 or tst_semantic.time_cost > max_cost['time_cost']:
            max_cost['time_cost'] = tst_semantic.time_cost
            max_cost['query'] = tst_semantic.query
            max_cost['match_mode'] = tst_semantic.match_mode
        if min_cost['time_cost'] < 0 or tst_semantic.time_cost < min_cost['time_cost']:
            min_cost['time_cost'] = tst_semantic.time_cost
            min_cost['query'] = tst_semantic.query
            min_cost['match_mode'] = tst_semantic.match_mode


        # confusion matrix
        domain_index_lab = domains.index(lab_semantic.domain)
        domain_index_tst = domains.index(tst_semantic.domain)
        intent_index_lab = intents.index(lab_semantic.intent)
        intent_index_tst = intents.index(tst_semantic.intent)
        domain_confusion[domain_index_lab][domain_index_tst] += 1                     # confusion_matrix中第i行第j列表示真实为第i类预测成了第j类的数目
        if lab_semantic.domain == tst_semantic.domain:
            intent_confusion[intent_index_lab][intent_index_tst] += 1                 # 只计算domain分类正确的情况下的intent
        
        # badcase
        # other类只要domain正确就OK
        if lab_semantic.domain == 'other' and tst_semantic.domain == 'other':
            continue
        domain_diff = 0
        intent_diff = 0
        slots_diff = 0
        if lab_semantic.domain != tst_semantic.domain:
            domain_diff = 1
        if lab_semantic.intent != tst_semantic.intent:
            intent_diff = 1

        _slots_lab = sorted(slots_lab)
        _slots_tst = sorted(slots_tst)
        if len(_slots_lab) != len(_slots_tst):
            slots_diff = 1
        else:
            for idx in range(len(_slots_lab)):
                if _slots_lab[idx][0] != _slots_tst[idx][0] or _slots_lab[idx][1] != _slots_tst[idx][1]:
                    slots_diff = 1

        if domain_diff:
            domain_wrong_num += 1
            dw.append(lab_semantic.text + " <==> " + tst_semantic.text)
        if domain_diff or intent_diff:
            domain_or_intent_wrong_num += 1
            diw.append(lab_semantic.text + " <==> " + tst_semantic.text)
        if domain_diff or intent_diff or slots_diff:
            domain_or_intent_or_slots_wrong_num += 1
        


    # 打印分类badcase
    print("\n\n>>>>> 领域分类错误 (domain clf badcase)")
    print("行号, 正确结果 <=> SLU解析结果")
    for line in dw:
        print(line)
    
    print("\n\n>>>>> 领域分类错误 or 意图分类错误 (domain or intent clf badcase)")
    print("行号, 正确结果 <=> SLU解析结果")
    for line in diw:
        print(line)

    
    # 计算混淆矩阵
    domain_result, domain_macroF1 = eval_confusion(domain_confusion)
    intent_result, _ = eval_confusion(intent_confusion)
    

    print("\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>")
    print("分类评价指标(clf)")
    # 打印SLU评价指标
    print("【slu系统级领域分类 accuracy && micro-F1】: %.2f%%  " % ((all_num-domain_wrong_num)*100.0/all_num))
    print("【slu系统级领域分类 macro-F1】: %.2f%%  " % (domain_macroF1))
    print("【slu系统级领域及意图分类 accuracy】: %.2f%%  (when domain and intent is correct)" % ((all_num-domain_or_intent_wrong_num)*100.0/all_num))
    print("【slu系统级领域及意图分类、语义项抽取 accuracy】	: %.2f%%  (when domain/intent/slots are all correct)" % ((all_num-domain_or_intent_or_slots_wrong_num)*100.0/all_num))


    print("\n模型解析 vs. 规则解析")
    print("【model match】: %.2f%%" % (model_num*100.0/all_num))
    print("【regex match】: %.2f%%" % (regex_num*100.0/all_num))
    
    print("\n测试集不合法行条数")
    print("【测试结果文件格式不合法行数】: " + str(tst_invalid_num))
    print("【未找到正确标记的测试query数】: " + str(tst_not_found_num))
    

    # 打印分类结果混淆矩阵
    print_clf_confusion_matrix(domains, intents, domain_confusion, intent_confusion, domain_result, intent_result)

    # 打印序列标注评估结果及badcase
    print("\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>")
    print("语义项抽取评价指标(ner)")
    f1_measure = {}
    for domain in domains:
        if domain not in sl_data or len(sl_data[domain]) < 1:
            print('sl_data of domain ' + domain + ' not exist')
            continue

        query_lab_list = []
        query_tst_list = []
        slots_lab_list = []
        slots_tst_list = []
        for i in range(len(sl_data[domain])):
            query_lab, slots_lab, query_tst, slots_tst = sl_data[domain][i]
            query_lab_list.append(query_lab)
            query_tst_list.append(query_tst)
            slots_lab_list.append(slots_lab)
            slots_tst_list.append(slots_tst)

        precision, recall, f1, SLOT_LIST, wrong_cases = slotfilling_evaluate(query_lab_list, query_tst_list, slots_lab_list, slots_tst_list)
        tst_number = len(query_lab_list)

        print("\n\n领域: %s" % domain)
        print("该领域评估所用有效条数(分类正确): " + str(tst_number))
        print('precision: ' + str(precision['total']))
        print('recall: ' + str(recall['total']))
        print('f1: ' + str(f1['total']))
        print('slotname\tprecision\trecall\tf1')
        for slotname in SLOT_LIST:
            print('\t'.join([slotname, str(precision[slotname]), str(recall[slotname]), str(f1[slotname])]))
        print("行号, 正确结果 <=> SLU解析结果")
        for badcase in wrong_cases:
            print(badcase)
        f1_measure[domain] = f1['total']

    print("\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>")
    print("语义项抽取评价指标(ner)")
    # 统计使用标注模型的领域
    _sl_domains = []
    macro_F1 = 0.0
    for domain in sl_domains:
        if domain in f1_measure:
            if f1_measure == 'Nan':
                continue
            macro_F1 += f1_measure[domain]
            _sl_domains.append(domain)
    macro_F1 = safe_divide(macro_F1, len(_sl_domains))
    print('【slu系统级序列标注模型评价指标】macro-F1 (over domains: ' + ' && '.join(_sl_domains) + '): ' + str(macro_F1))

    # 统计other外所有领域
    _sl_domains = []
    macro_F1 = 0.0
    for domain in f1_measure:
        if f1_measure == 'Nan' or domain == 'other':
            continue
        macro_F1 += f1_measure[domain]
        _sl_domains.append(domain)
    macro_F1 = safe_divide(macro_F1, len(_sl_domains))
    print('【slu系统级语义项抽取评价指标】macro-F1 (over domains: ' + ' && '.join(_sl_domains) + '): ' + str(macro_F1))

    print("\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>")
    print("统计系统耗时(模型耗时指先走了规则又走了模型的总时间)")
    # 统计耗时
    print("【总平均耗时】: " + str(safe_divide(time_cost['total'], total_number['total'])) + 'ms')
    print("【模型匹配平均耗时】: " + str(safe_divide(time_cost['model'], total_number['model'])) + 'ms')
    print("【规则匹配平均耗时】: " + str(safe_divide(time_cost['regex'], total_number['regex'])) + 'ms')
    print("【最大耗时】: " + max_cost['query'] + " " + str(max_cost['time_cost']) + "ms " + max_cost['match_mode'])
    print("【最小耗时】: " + min_cost['query'] + " " + str(min_cost['time_cost']) + "ms " + min_cost['match_mode'])


    for domain in domains:
        if domain not in time_cost:
            continue
        print('\ndomain: ' + domain)
        print("\t该领域总平均耗时: " + str(safe_divide(time_cost[domain]['total'], total_number[domain]['total'])) + 'ms')
        tmp = str(safe_divide(time_cost[domain]['model'], total_number[domain]['model']))
        tmp += 'ms' if tmp != 'Nan' else ''
        print("\t该领域模型匹配平均耗时: " + tmp)
        tmp = str(safe_divide(time_cost[domain]['regex'], total_number[domain]['regex']))
        tmp += 'ms' if tmp != 'Nan' else ''
        print("\t该领域规则匹配平均耗时: " + tmp)

    print('\n\n')

    lab.close()
    tst.close()



if __name__=="__main__":

    if len(sys.argv) < 3:
        print("Usage:")
        print("\tpython %s  lab_file  tst_file\n" % sys.argv[0])
    else:
        slu_evaluate(sys.argv[1], sys.argv[2], ['music', 'poem', 'story', 'weather', 'phone', 'nav'])


