87行	atoi(), 不会返回小于零的数
285行	需要pre为空，1、构造函数直接exit()；2、是私有变量不能外部指定为NULL
291行	pre->wordseg()，segmentor_segment()返回值不会小于零（_wsg_engine为空会崩溃）
200行	get_wsg()返回值小于零的情况：1、pre == NULL; 2、pre->wordseg()小于零，参照上两条，都不可以。
209行	get_sip(), 小于零，需要strSplit2simple返回false，传入一些非utf8/gbk的其他格式，不会有这种情况
258行	pad_seq(), 不会返回小于零的数
268行	add_feature(), 不会返回小于零的数
