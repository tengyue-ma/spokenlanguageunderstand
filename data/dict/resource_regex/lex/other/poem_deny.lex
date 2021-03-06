#########################
#author:zhouleilei      #
#date:2018.5.19         #
#########################

播放否定 = <请求礼貌>?(<我不想>|<不要给我>)<播放>?;

我不想	 = <我>?<现在>?<程度>?(<不喜欢>|<不想听>)<你>?;
不喜欢	 = (不要|不想要|不喜欢|拒绝|讨厌)<再>?<播放>?;
不想听	 = (不想|停止|别|不|不爱|不是很想|不是要)<再>?<播放>;
程度	 = 好|非常|特别;

不要给我 = <你>?<现在>?<能不能>?<不要><再>?<给我>?;
不要	 = 不要|别;
给我	 = <给><我>?;
给		 = 给|帮|为|替;
我		 = 我|俺|大爷|爷|咱|咱们|老娘|本小姐|本大小姐|我们|本人|朕|我的宝宝|宝宝;
你		 = 你|朋友|机器人;
播放	 = 播|播放|放|总放|唱|听|读|背|朗诵|朗读;
再		 = 再|继续|一直|老;
现在	 = 现在|此时;

请求礼貌 = <你>?<能不能>?也?请?(快|马上)?;
结尾礼貌 = (<疑问助词>?<疑问助词>)|(<好不好><疑问助词>?);
疑问助词 = 吗|呢|啊|咯|呀|吧|么|了|的|啦|吧;
能不能	 = 能不能|能|可以|可不可以|拜托;
好不好	 = 好不好|行不行|好不|行不|如何|可以|好|行|怎么样|可不可以;

诗       =  诗|词|诗词|诗歌|诗句|名诗|诗朗诵|古诗|古诗词;
诗词类别 = 唐诗|宋词|元曲|乐府诗|楚辞|现代诗|五言古诗|五言律诗|五言绝句|七言古诗|七言律诗|七言绝句;
写的  = (写|创作)?的?;
关于  = 关于|有关;

思念  = 月亮|月光|思乡|思念家乡|离别|相思;
心情  = 高兴|开心|伤感|欢快|忧伤|安静|快乐|感动|温暖|悲伤|相爱|爱情;
派别  = 山水田园|田园山水|山水|田园|田园山居|田园派|田园风光|田园风格|抒情|边塞|闺怨|送别|边塞|赠别|怀古|婉约|婉约派|咏物|咏怀|叙事|励志|哲理|唯美;
类别  = 报国|爱国|战争|壮志难酬|远大志向|远大抱负|怀才不遇|国家离乱|国家存亡|爱国情怀|被贬|豪放|父爱|母爱|父亲|母亲|景色|孝顺;
标签  = <思念>|<心情>|<派别>|<类别>;


###### 不要李白的静夜思
###### 不要李白的诗
###### 李白的诗不要放
export deny = ^<播放否定>(&<poem_author>:author)?<写的>(&<poem_title>:title|<诗词类别>:tag|<诗>)<结尾礼貌>?$ => request(intent="other") <2.7>;
export deny = ^<播放否定>(&<poem_author>:author)<写的>(<诗词类别>:tag|<诗>)<结尾礼貌>?$ => request(intent="other") <2.7>;
export deny = ^(&<poem_author>:author)<写的>(<诗词类别>:tag|<诗>)<播放否定><结尾礼貌>?$ => request(intent="other") <2.7>;

######## 不要听 <poem_title>(可选) 这首诗
export deny = ^<播放否定>(&<poem_title>:title)?(这|那)首<诗><结尾礼貌>?$  =>  request(intent="other") <2.7>;

######## 不要听 <tag> 类
export deny = ^<播放否定>(<标签>:tag)的?(<诗>|<诗词类别>:tag)<结尾礼貌>?$  =>  request(intent="other") <2.7>;




