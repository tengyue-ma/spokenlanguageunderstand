#include "../comm/digit.lex"
播放    = 播|播放|放|听|读|讲|讲讲|讲一讲|说|说说|说一说|来<一个>;
查询	= 有没有|有木有|有么有|是否有|是否有|有|还有|找找|找|搜|搜搜|搜索|搜寻|查找|查查|查一查|查|查询|列出|搜一搜|找一找;
推荐	= 推荐;
一个	= (一|几|两|一两)?(首|段|个|则|下);
操作	= <播放>|<查询>|<推荐>;
独立故事类别   = 神话|童话|寓言|传说|成语故事|故事;
故事标签= 绘本|怀旧|清新|浪漫|伤感|治愈|放松|悲伤|温暖|孤独|感动|兴奋|快乐|安静|思念|欢快|爱情|忧伤|嗨爆了|睡前|言情|耽美同人|耽美|童趣|神奇|神秘|历史|科幻|外国;
泛指人群=全家人|年轻人|青少年|小朋友|小孩|小孩子|宝宝|儿童|老年人|老人|60后|70后|80后|90后|00后|六零后|七零后|八零后|九零后|零零后|大学生|高中生|初中生|小学生|中学生;


###### inform0: [<主观意愿>] [<播放操作>] <故事> [<结尾礼貌>]
###### 没有slot或只有tag, 例如: 我想听故事|我想听神话故事
export inform00 = <操作><一个>?(小|短篇|长篇)?的?(<独立故事类别>) => request(tag="$2") <0.3>;


###### inform1: [<主观意愿>] [<播放操作>] <故事作者> <写的故事> [<结尾礼貌>]
###### 包含作者名,如: 安徒生写的故事
export inform10 = <操作><一个>?(&<story_author>)(写|创作)?的(小|短篇|长篇)?(<独立故事类别>) => request(author="$1", tag="$4") <0.4>;


###### inform2: <故事名_单独点播>
###### 单独说故事名, 例如: 卖火柴的小女孩
export inform20 = ^(&<story_title_single>)$ => request(title="$1") <0.3>;
#export inform21 = (&<story_title>)(可以|能不能|你会|能)?(给我)?<操作> => request(title="$1") <0.3>;

#这里用了music_language
export inform22 = <操作><一个>?(&<music_language>)版?的?(&<story_title>)的?(<独立故事类别>)? => request(tag="$1",title="$2") <0.4>;
export inform22 = <操作><一个>?(&<story_title>)的?(<独立故事类别>)? => request(title="$1") <0.3>;
export inform22 = ^(&<story_title>)的?(<独立故事类别>)? => request(title="$1") <0.3>;
export inform22 = <操作><一个>?(&<story_album>)的?(<独立故事类别>)? => request(album="$1") <0.3>;
export inform22 = ^(&<story_album>)的?(<独立故事类别>)? => request(album="$1") <0.3>;


###### inform3: [<主观意愿>] [<播放操作>] [<故事作者>] [<写的>] <故事名>|<专辑名> [<这个故事>|<故事>] [<结尾礼貌>]
###### 作者+故事名,例如: 安徒生写的卖火柴的小女孩
export inform30 = (&<story_author>)(写|创作)?的?(&<story_title>)的?(<独立故事类别>)? => request(author="$1", title="$3") <0.4>;
export inform31 = (&<story_author>)(写|创作)?的?(&<story_album>)的?(<独立故事类别>)? => request(author="$1", album="$3") <0.4>;


###### inform4: [<主观意愿>] [<播放操作>] [<主人公>] <主人公名> [<和> <主人公名1>] <故事> [<结尾礼貌>]
###### 按角色播放
export inform40 = (主角|主人公)(是|有)(&<story_character>)(和|与|跟)?(&<story_character>)?的(小|短篇|长篇)?(<独立故事类别>) => request(character="$3", character1="$5", tag="$7") <0.5>;
export inform42 = (&<story_character>)(和|与|跟)?(&<story_character>)?的(小|短篇|长篇)?(<独立故事类别>) => request(character="$1", character1="$3", tag="$5") <0.5>;


###### inform5: [<主观意愿>] [<播放操作>] <其他槽位> [的|<和>] [<其他槽位1>] <故事> [<结尾礼貌>]
###### 其他slot,允许两个slot组合,例如: 希腊神话
export inform50 = (<故事标签>|&<music_region>|&<music_language>)(和|与|且|并且)?(<故事标签>|&<music_region>|&<music_language>)?的?(小|短篇|长篇)?(<独立故事类别>) => request(tag="$1", tag1="$3", tag2="$5") <0.5>;


###### inform6: [<主观意愿>] [<播放操作>] <适合人群> <故事> [<结尾礼貌>]
###### 适合人群,例如: 适合小孩听的故事
export inform60 = (适合|合适|给)(<一百以内数字>岁的?<泛指人群>|<泛指人群>)的?(<独立故事类别>) => request(tag="$2", tag1="$3") <0.4>;

export inform62 = (<一百以内数字>岁的?<泛指人群>|<泛指人群>)(听|欣赏)?的?(<独立故事类别>) => request(tag="$1", tag1="$3") <0.4>;

