独立故事类别	= 神话|童话|寓言|传说;
非独立故事类别	= 成语;


###### reqalts0: [<我想知道>] <这个作家>|<故事作者> <还写过> <什么故事> [<结尾礼貌>]
###### 他还有什么故事/他还写过什么故事
export reqalts00 = (这个?(人|作家|作者)|他|她|它)(还有|还写|还创作)(过|了)?(什么|哪些|啥子|啥)(<独立故事类别>) => request(intent="reqalts", tag="$5")<0.9>;
export reqalts00 = (这个?(人|作家|作者)|他|她|它)(还有|还写|还创作)(过|了)?(什么|哪些|啥子|啥)(<非独立故事类别>)?故事 => request(intent="reqalts", tag="$5")<0.9>;
export reqalts01 = (&<story_author>)(还有|还写|还创作)(过|了)?(什么|哪些|啥子|啥)(<独立故事类别>) => request(author="$1", tag="$5", intent="reqalts")<0.9>;
export reqalts01 = (&<story_author>)(还有|还写|还创作)(过|了)?(什么|哪些|啥子|啥)(<非独立故事类别>)?故事 => request(author="$1", tag="$5", intent="reqalts")<0.9>;
