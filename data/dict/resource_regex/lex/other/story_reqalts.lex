播放    = 播|播放|放|(我?(要|想))?听|读|讲|讲讲|讲一讲|说|说说|说一说|来<一个>;
查询	= 有没有|有木有|有么有|是否有|是否有|有|还有|找找|找|搜|搜搜|搜索|搜寻|查找|查查|查一查|查|查询|列出|搜一搜|找一找;
推荐	= 推荐;
一个	= (一|几|两|一两)?(段|个|则|下);
操作	= <播放>|<查询>|<推荐>;
独立故事类别    = 神话|童话|寓言|传说; 
非独立故事类别  = 成语; 
结束语    = <疑问词>|<致谢>|<有没有>|<结尾请求>;
有没有    = 有没有;
致谢      = (谢谢|感谢)(你|<疑问词>)?;
疑问词    = 吗|呢|啊|咯|呀|吧|么|了;
结尾请求  = 好吗|好嘛|好么|行吗|行嘛|行么|可以吗|可以么|怎么样|如何|好不好;


###### reqalts0: [<我想知道>] <这个作家>|<故事作者> <还写过> <什么故事> [<结尾礼貌>]
###### 他还有什么故事/他还写过什么故事
export reqalts00 = (这个?(人|作家|作者)|他|她|它)还(有|写|创作)(过|了)?(什么|哪些|啥子|啥)(<独立故事类别>:tag|故事) => request(intent="other") <1.9>;
export reqalts00 = (这个?(人|作家|作者)|他|她|它)还(有|写|创作)(过|了)?(什么|哪些|啥子|啥)(<非独立故事类别>:tag|故事) => request(intent="other") <1.9>;

export reqalts01 = <操作><一个>?(别|其他|其它)的?(<独立故事类别>:tag|故事)<结束语>?$ => request(intent="other") <1.9>;

export reqalts02 = 换<一个>(别|其它|其他)?的?(<独立故事类别>:tag|故事)<结束语>?$ => request(intent="other") <1.9>;