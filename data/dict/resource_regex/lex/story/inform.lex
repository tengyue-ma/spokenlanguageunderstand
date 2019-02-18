#include "../comm/digit.lex"

播放    = 播|播放|放|(我?(要|想))?听|读|讲|讲讲|讲一讲|说|说说|说一说|来<一个>;
查询	= 有没有|有木有|有么有|是否有|是否有|还有|找找|搜|搜搜|搜索|搜寻|查找|查查|查一查|查|查询|列出|搜一搜|找一找|你有|有吗;
推荐	= 推荐;
一个	= (一|几|两|一两)?(段|个|则|下|首);
操作	= <播放>|<查询>|<推荐>;
独立故事类别   = 神话|童话|寓言|传说|小说;
故事    =  小?故事;
故事标签= 绘本|怀旧|清新|浪漫|伤感|治愈|放松|悲伤|温暖|孤独|感动|兴奋|快乐|安静|思念|欢快|爱情|忧伤|嗨爆了|睡前|言情|耽美同人|耽美|童趣|神奇|神秘|历史|科幻|外国|吃饭|夜晚|开心|高兴|愉快|国学|晚安|摇篮;
泛指人群=全家人|年轻人|青少年|小朋友|小孩|小孩子|宝宝|儿童|老年人|老人|60后|70后|80后|90后|00后|六零后|七零后|八零后|九零后|零零后|大学生|高中生|初中生|小学生|中学生|男孩子|女孩子|男宝宝|女宝宝;
适合人群  = <一百以内数字><一百以内数字>?岁的?<泛指人群>|<泛指人群>|老人和小孩|小孩和老人;
结束语    = <疑问词>|<致谢>|<有没有>|<结尾请求>|我就睡(觉|着)了?;
有没有    = 有没有;
致谢      = (谢谢|感谢)(你|<疑问词>)?;
疑问词    = 吗|呢|啊|咯|呀|吧|么|了;
结尾请求  = 好吗|好嘛|好么|行吗|行嘛|行么|可以吗|可以么|怎么样|如何|好不好;
创作 = 写|写作|创作;
里面 = (全集)?(中|里|里面)?;
给我 = (给|替|帮)(我|俺|本?宝宝);



############ 必含故事名 ############
###### 播放 (作者|专辑)? 的 (中文版)? (故事名)
export inform00 = (&<language>:tag)?<给我>?<操作><一个>?(&<story_author>:author|&<story_album>:album)?(<创作>|<里面>)?的?(&<language>:tag)?版?的?(&<story_title>:title|&<story_album>:album)(的|这个)?(<独立故事类别>:tag|<故事>)?<故事>?<结束语>?$ => request() <1.89>;
###### 播放 (作者|专辑)? 的 (故事名) (中文版)?
export inform00 = (&<language>:tag)?<给我>?<操作><一个>?(&<story_author>:author|&<story_album>:album)?(<创作>|<里面>)?的?(&<story_title>:title|&<story_album>:album)(的|这个)?(<独立故事类别>:tag|<故事>)?<故事>?的?(&<language>:tag)?版?<结束语>?$ => request() <1.89>;
###### (作者|专辑)? 的 (中文版)? (故事名) 播放
export inform00 = [^唱](&<story_author>:author|&<story_album>:album)?(<创作>|<里面>)?的?(&<language>:tag)?版?的?(&<story_title>:title|&<story_album>:album)(的|这个)?(<独立故事类别>:tag|<故事>)?<故事>?<给我>?用?(&<language>:tag)?<操作><一个>?<结束语>?$ => request() <1.89>;
export inform00 = ^(&<story_author>:author|&<story_album>:album)?(<创作>|<里面>)?的?(&<language>:tag)?版?的?(&<story_title>:title|&<story_album>:album)(的|这个)?(<独立故事类别>:tag|<故事>)?<故事>?<给我>?用?(&<language>:tag)?<操作><一个>?<结束语>?$ => request() <1.89>;
###### (作者|专辑)? 的 (故事名) (中文版)? 播放
export inform00 = [^唱](&<story_author>:author|&<story_album>:album)?(<创作>|<里面>)?的?(&<story_title>:title|&<story_album>:album)(的|这个)?(<独立故事类别>:tag|<故事>)?<故事>?的?(&<language>:tag)?版?<给我>?用?(&<language>:tag)?<操作><一个>?<结束语>?$ => request() <1.89>;
export inform00 = ^(&<story_author>:author|&<story_album>:album)?(<创作>|<里面>)?的?(&<story_title>:title|&<story_album>:album)(的|这个)?(<独立故事类别>:tag|<故事>)?<故事>?的?(&<language>:tag)?版?<给我>?用?(&<language>:tag)?<操作><一个>?<结束语>?$ => request() <1.89>;
###### (故事名) 讲的是什么
export inform03 = (&<story_title>:title|&<story_album>:album)是(什么|啥子?)?(<独立故事类别>:tag|小?故事)<故事>?<结束语>?$=> request() <1.89>;
export inform04 = (&<story_title>:title|&<story_album>:album)(的|这个)?(<独立故事类别>:tag|<故事>)?<故事>?(讲|说|内容)的?是?(什么|啥子?)<疑问词>?$=> request() <1.89>;



############ 必含作者 ############
###### 播放 (作者) 的 故事
export inform01 = (&<language>:tag)?<给我>?<操作><一个>?(&<story_author>:author)的?(&<language>:tag)?版?的?(<独立故事类别>:tag|<故事>)<故事>?<结束语>?$ => request() <1.88>;
###### (作者) 的 故事 播放
export inform01 = (&<story_author>:author)的?(&<language>:tag)?版?的?(<独立故事类别>:tag|<故事>)<故事>?<给我>?用?(&<language>:tag)?<操作><一个>?<结束语>?$ => request() <1.88>;



############ 必含角色 ############
###### 播放 (角色) 的 故事
export inform02 = (&<language>:tag)?<给我>?<操作><一个>?(&<story_character>:character)的(<独立故事类别>:tag|<故事>)<故事>?<结束语>?$ => request() <1.87>;
###### (角色) 的 故事 播放
export inform02 = (&<story_character>:character)的(<独立故事类别>:tag|<故事>)<故事>?<给我>?用?(&<language>:tag)?<操作><一个>?<结束语>?$ => request() <1.87>;



############ 必含专辑 ############
###### 已包含在 故事名 的正则中



############ 必含地区 ############
export inform00 = (&<language>:tag)?<给我>?<操作><一个>?(经典的?|著名的?|耳熟能详的?)?(&<story_region>:tag)的?(<故事标签>:tag)?的?(<独立故事类别>:tag|<故事>)<故事>?<结束语>?$ => request() <1.865>;
export inform00 = (&<story_region>:tag)的?(<故事标签>:tag)?的?(<独立故事类别>:tag|<故事>)<故事>?用?(&<language>:tag)?<给我>?<操作><一个>?<结束语>?$ => request() <1.865>;


############ 必含类型 ############
###### 播放 (某类型) 的 故事
export inform02 = (&<language>:tag)?<给我>?<操作><一个>?(经典的?|著名的?|耳熟能详的?)?(<故事标签>:tag)的?(<独立故事类别>:tag|<故事>)<故事>?<结束语>?$ => request() <1.81>;
export inform02 = (<故事标签>:tag)的?(<独立故事类别>:tag|<故事>)<故事>?用?(&<language>:tag)?<给我>?<操作><一个>?<结束语>?$ => request() <1.81>;



############ 必含类型 ############
###### 播放 故事
export inform03 = (&<language>:tag)?<给我>?<操作><一个>?(经典的?|著名的?|耳熟能详的?)?(<独立故事类别>:tag|<故事>)<故事>?<结束语>?$ => request() <1.8>;


export inform04 = (<适合人群>:tag)都?(适合|可以|能)?听的(<独立故事类别>:tag|<故事>)<故事>?<结束语>?$ => request() <1.79>;


###### 单独的故事名, 故事专辑
export inform05 = ^(&<story_title_single>:title)$ => request() <1.99>;
export inform06 = ^((安徒生|格林)童话|伊索寓言)$ => request(album="$1") <1.99>;

