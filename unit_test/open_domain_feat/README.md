149行	_slot_type == NULL，为了测词典为空，再写一个conf，需要改原来的代码，否则会exit(-1)
202行	wordseg(), postag(), ner(), 调用ltp的函数，返回值不会小于零