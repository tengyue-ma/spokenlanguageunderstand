#########################
#author:zhouleilei      #
#date:2018.4.28         #
#########################

#include "../music/music_public.lex"
确认	 = 你?<难道>?<确不确认>;

难道	 = 难道|到底|应该;

确不确认 = (<会不会>|<有没有>|<能不能>|<可不可以>|<知不知道>)<什么>?<形容>?;
会不会	 = 会不会|是不是会|是不是不会|不会|会;
有没有	 = 有没有|有木有|有么有|是否有|这有|这里有|还有|有|没有|是不是有|是不是没有;
能不能	 = 能不能|能不能唱|是不是能唱|是不是不能唱|能唱|不能唱|能|能否;
可不可以 = 可不可以|可以;
知不知道 = 知不知道|知道;
喜不喜欢 = 喜欢|喜爱|爱|喜不喜欢|喜不喜爱|爱不爱;
疑问助词 = 呢|啊|吧|呀|啦|吗|嘛|么|了|的|咯|哇|哈;
感叹词	 = 天?<疑问助词>;
我喜欢	 = <我><程度>?<喜欢><疑问助词>?;
我不喜欢 = <我><不><程度>?<喜欢><疑问助词>?;
不		 = 不|讨厌;

歌声	 = 歌|声音|歌声|嗓音|老歌;
唱歌	 = (唱|演唱|演奏|弹奏)(歌声);
这		 = (这|那)(首|个)?;

人称	 = 你|他|她|它;
介绍	 = 介绍;
一下	 = 一下;

诗词形容	 = <程度>?(好听|不错|棒|动听|热门|著名|赞|美|美妙|经典|凄凉|意境)的?;

###### 问候
export other = ^(早安|早上好|晚安|晚上好)<疑问助词>?$ => request(intent="other") <2.0>;

###### 1、model badcase
export other = ^(我|蝉|过年了|天气好么|继续换|换|你听过什么歌|这首歌没听过|别叫了难听死了|别叫|难听死)<疑问助词>?$ => request(intent="other") <2.0>;
export other = ^(<确认>)<疑问助词>?$ => request(intent="other") <2.0>;

###### 2、类似control的说法
###### 音量相关
export other = ^(再来一个|播放一首|不来一遍)<疑问助词>?$ => request(intent="other") <2.0>;
export other = ^(静音)<疑问助词>?$ => request(intent="other") <2.0>;

###### 3、对music的评价
###### 告白气球很赞
###### 刘德华的歌声很赞|刘德华唱歌很好听
###### 刘德华冰雨很赞
###### 这首歌很好听啊
###### 他唱的好听吗
export other = ^(&<music_title>)<形容><疑问助词>?<我喜欢>?$ => request(intent="other") <2.0>;
export other = ^(&<music_artist>)的?(<唱歌>|<歌声>)?<形容><疑问助词>?<我喜欢>?$ => request(intent="other", person="$1") <2.0>;
export other = ^(&<music_artist>)<唱>?的?(&<music_title>)的?<形容><疑问助词>?<我喜欢>?$ => request(intent="other", person="$1") <2.0>;
export other = ^<感叹词>?<这>(<歌曲类别>:tag|<歌>)?<形容><疑问助词>?<我喜欢>?$ => request(intent="other") <2.0>;
export other = ^<人称>?唱的?<形容><疑问助词>?$ => request(intent="other") <2.0>;

###### 对诗词的评价
export other = ^<感叹词>?<这>(诗词|诗|词)?的?(意境|描写)?的?<诗词形容><疑问助词>?<我喜欢>?$ => request(intent="other") <2.0>;

###### 4、对artist的评价
###### 我喜欢周杰伦
###### 我不喜欢周杰伦
export other = ^<我喜欢>(&<music_artist>)<疑问助词>?$ => request(intent="other", person="$1") <2.0>;
export other = ^<我不喜欢>(&<music_artist>)<疑问助词>?$ => request(intent="other", person="$1") <2.0>;

###### 5、你有xxx吗
###### 你会XXX吗
###### 你知道李健吗
###### 你知道世界杯吗
###### 你喜不喜欢周杰伦的歌
###### 你喜欢安徒生童话吗
export other = ^<人称>?<有没有>(家人|孩子|爸爸|妈妈|爷爷|奶奶|毛病)<疑问助词>?$ => request(intent="other") <2.0>;
export other = ^<人称>?<会不会>?(玩|开始)?(干什么|算术|计算|讲笑话|成语接龙|猜谜语)<疑问助词>?$ => request(intent="other") <2.0>;
export other = ^<人称>?<知不知道>(&<music_artist>|&<poem_author>|&<story_author>|世界杯)<疑问助词>?$ => request(intent="other", person="$1") <2.0>;
export other = ^<人称>?<知不知道>(世界杯)<疑问助词>?$ => request(intent="other") <2.0>;
export other = ^<人称>?<喜不喜欢>(&<music_artist>)(的歌)?<疑问助词>?$ => request(intent="other", person="$1") <2.0>;
export other = ^<人称>?<喜不喜欢>(&<story_album>|&<story_title>)(的故事)?<疑问助词>?$ => request(intent="other") <2.0>;

###### 6、问喜好
export other = ^<人称>?喜欢(睡觉|音乐|听歌)<疑问助词>?$ => request(intent="other") <2.0>;
export other = ^<人称>?喜欢吃(饭|苹果|水果|西瓜)<疑问助词>?$ => request(intent="other") <2.0>;

###### 7、描述心情/表情
export other = ^<我>(今天|现在|这几天)?(感到|觉得|感觉)?<程度>?<心情><疑问助词>?$ => request(intent="other") <2.0>;
export other = ^<我>?(今天|现在|这几天)(感到|觉得|感觉)?<程度>?<心情><疑问助词>?$ => request(intent="other") <2.0>;
export other = ^<我>?(今天|现在|这几天)?(感到|觉得|感觉)<程度>?<心情><疑问助词>?$ => request(intent="other") <2.0>;

###### 8、XXX是谁
###### 介绍一下xxx这个人
export other = ^<人称>?<知不知道>?(&<music_artist>|&<poem_author>|&<story_author>)是谁<疑问助词>?$ => request(intent="other", person="$1") <2.0>;
export other = ^请?<介绍><一下>?(&<music_artist>|&<poem_author>|&<story_author>)?(这个人)<疑问助词>?$ => request(intent="other", person="$1") <2.0>;
export other = ^请?<介绍><一下>?(&<music_title>|&<poem_title>|&<story_title>)?(这首歌|这首诗|这个故事)<疑问助词>?$ => request(intent="other") <2.0>;

###### 9、[music_title是]谁唱的
###### music_title的原唱是谁
###### music_artist唱过哪些歌
export other = ^(&<music_title>)?是?谁唱的<疑问助词>?$ => request(intent="other") <2.0>;
export other = ^(&<music_title>)?的?(原唱是)谁?<疑问助词>?$ => request(intent="other") <2.0>;
export other = ^(&<music_artist>)还?(唱过哪些歌|有哪些专辑)<疑问助词>?$ => request(intent="other", person="$1") <2.0>;

###### 10、动画片西游记的主题曲是什么
###### 这首歌是西游记的主题曲吗
###### 春风十里不如你是哪个电视剧的主题曲
export other = ^<动画片>?(&<music_program>)<里面>?的?<主题曲>是什么<疑问助词>?$ => request(intent="other") <2.0>;
export other = ^这首歌是(&<music_program>)<里面>?的?<主题曲><疑问助词>?$ => request(intent="other") <2.0>;
export other = ^(&<music_title>)是哪个<动画片>?的?<主题曲><疑问助词>?$ => request(intent="other") <2.0>;

###### 11、翻译
###### xxx用英文怎么说|xxx的英文名是什么
###### 英文的XXX怎么说
export other = ^(你?知道|请?告诉我)?.*(用|的)?(&<music_language>)(名|名字)?怎么说<疑问助词>?$ => request(intent="other") <2.0>;
export other = ^(&<music_language>)的?.*怎么说<疑问助词>?$ => request(intent="other") <2.0>;

###### 地点，权重低于subweather,例：北京北京|我想去北京|北京欢迎你
export other = ^我?(要|想|想要)?去?(&<loc_country>:loc_country|&<loc_city>:loc_city|&<loc_area>:loc_area|&<loc_poi>:loc_poi)(&<loc_country>:loc_country|&<loc_city>:loc_city|&<loc_area>:loc_area|&<loc_poi>:loc_poi)?(欢迎你)?$ => request(intent="other") <0.5>;

###### 音量，"最低"过拟合
export other = ^快?请?(把|将|让)?(音量|声音)?(调|降)?(到|至|成)?最低<疑问助词>?$ => request(intent="other") <2.0>;

###### phone "要回"过拟合
export other = ^我?(要回)(家|去)?<疑问助词>?$ => request(intent="other") <2.0>;

###### music "我想听"过拟合
export other = ^我(想|要|来)(听)$ => request(intent="other") <2.0>;









































