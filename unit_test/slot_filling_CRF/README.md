SlotTagger.cpp
64行	没有判断函数的返回值，后续需要代码补上，否则model_path不存在会崩溃
_tagger	为空的情况，就是模型加载失败，所以没有空的情况
140行	tagger->parse()返回值为bool类型，但是不知道什么情况下返回false，crf++源码中没有看到返回false的情况
170行	正常编码情况下，也没有<0的情况出现
174行	ner_decode()里面有进行判断

CRFWrapper.cpp
45行	
