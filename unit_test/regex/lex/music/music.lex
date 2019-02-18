#include "../comm/digit.lex"

随便 	 = 随便|随意|随机;
给		 = 给|帮|为|替;
我		 = 我|俺|大爷|爷|咱|咱们|老娘|本小姐|本大小姐|我们|本人|给朕;
要		 = 想|要|想要|希望|喜欢;

操作     = (<播放>|<查询>|<推荐>|<有>);
一首	 = (一|几)?(首|个|段|下|些|点);
播放	 = 播|放|唱|听|播放|点播|(来<一首>);
查询	 = 找|搜|找找|搜搜|搜索|搜寻|查找|查查|查|查询|列出|搜一搜|找一找|查一查;
推荐	 = 推荐;
唱		 = 唱|演唱|演奏|弹奏;
有       = 有|还有|有没有|有木有|有么有|是否有;

心情	 = 怀旧|清新|浪漫|性感|伤感|治愈|放松|悲伤|悠扬|温暖|孤独|感动|兴奋|快乐|安静|思念|欢快|爱情|忧伤|嗨爆了|劲爆|抒情|动感;
类别	 = 流行|民谣|电子|说唱|爵士|乡村|古典|民族|英伦|金属|朋克|蓝调|原声|雷鬼|拉丁|另类|独立|嘻哈|古风|网络|咖啡馆|夜店|广场舞|重金属;
乐器	 = 钢琴|小提琴|吉他|长笛|萨克斯|单簧管|口琴|二胡|琵琶|唢呐|古筝;
其他	 = 胎教;
标签	 = <心情>|<类别>|<乐器>|<其他>;
歌手性别 =(男|女)(性|生|歌手|艺人|音乐家);

歌曲类别 = 京剧|评剧|豫剧|越剧|洗脑歌|黄梅戏|轻音乐|儿歌|钢琴曲|情歌|交响乐|摇滚|摇篮曲;
歌		 = 歌|歌曲|专辑|曲|音乐|乐;
歌曲类别2 = 主题曲|片尾曲|片头曲|主打曲|插曲|主打歌;

泛指人群 = 全家人|年轻人|青少年|小朋友|小孩|宝宝|儿童|老年人|老人|60后|70后|80后|90后|00后|六零后|七零后|八零后|九零后|零零后|大学生|高中生|初中生|小学生|中学生;
具体年龄 = <一百以内数字>岁(的)?<泛指人群>;
人群	 = <具体年龄>|<泛指人群>;

###### 对应原来的music_inform1: [<主观意愿>] ([<播放操作>] <歌手名> <唱的歌>)|(<播放操作> <歌手名> [<唱的歌>]) [<结尾礼貌>]
export inform10 = <操作><一首>?(&<music_artist>)(<唱>的?|的)(<歌曲类别>) => request(artist="$1", tag="$3") <0.9>;

###### 对应原来的music_inform4: [<主观意愿>] [<播放操作>] [<歌手名>] [<唱的>] <歌曲名>|<专辑名> <这首歌> [<结尾礼貌>];
###### 跟inform3的区别是,<播放操作>是可选, 但<这首歌>必选, 例如: 告白气球这首歌
export inform40 = ^(&<music_title>)(这|那)(首|曲|个|种)(<歌曲类别>) => request(title="$1", tag="$4") <0.9>;
export inform41 = ^(&<music_title>)(这|那)(首|曲|个|种)<歌> 		  => request(title="$1") <0.9>;
export inform42 = ^(&<music_title>)(可以|能|请)?<操作>		  => request(title="$1") <0.9>;
export inform43 = ^(&<music_title>)(怎么唱|((你会唱|你有)(吗|么|嘛))|(可以|能|请)?<操作><一首>)$		  => request(title="$1") <0.9>;

###### 对应原来的music_inform5: [<主观意愿>] [<播放操作>] <歌手名> [<唱的>] <歌曲名>|<专辑名> [<这首歌>] [<结尾礼貌>]
###### 歌手名+歌曲名,例如: 周杰伦的告白气球
export inform50 = <操作><一首>?(&<music_artist>)(<唱>?的)?(&<music_title>) => request(artist="$1", title="$3") <0.9>;
export inform51 = <操作><一首>?(&<music_artist>)<唱>?的?(&<music_album>) => request(artist="$1", album="$2") <0.9>;
export inform52 = ^(&<music_artist>)<唱>?的(&<music_title>)			 => request(artist="$1", title="$2") <0.9>;
export inform52 = ^(&<music_artist>)(&<music_album>)			 => request(artist="$1", album="$2") <0.9>;

###### 对应原来的music_inform6: [<主观意愿>] [<播放操作>] <歌手名> [<和>] <歌手名1> [<合唱>] <唱的歌> [<结尾礼貌>]
###### 歌手名+歌手名,例: 周杰伦和蔡依林合唱的歌
export inform60 = <操作><一首>?(&<music_artist>)(和|与|跟)?(&<music_artist>)(合|一起|一块|联袂)?(<唱>的?|的)(<歌曲类别>) => request(artist="$1", artist1="$3", tag="$6")<0.9>;
export inform61 = <操作><一首>?(&<music_artist>)(和|与|跟)?(&<music_artist>)(合|一起|一块|联袂)?(<唱>的?|的)<歌>  => request(artist="$1", artist1="$3")<0.9>;
export inform62 = ^(&<music_artist>)(和|与|跟)?(&<music_artist>)(合|一起|一块|联袂)?(<唱>的?|的)(<歌曲类别>) => request(artist="$1", artist1="$3", tag="$6")<0.9>;
export inform63 = <操作><一首>?(&<music_artist>)(和|与|跟)?(&<music_artist>)(合|一起|一块|联袂)?(<唱>的?|的)<歌> 		 => request(artist="$1", artist1="$3")<0.9>;

###### 对应原来的music_inform7: [<主观意愿>] [<播放操作>] <其他槽位> [<其他槽位>] <歌曲> [<结尾礼貌>]
export inform70 = <操作><一首>?(&<music_region>)的?(<歌手性别>)(<唱>的?|的)(<歌曲类别>) => request(region="$1", gender="$2", tag="$4") <0.9>;
export inform71 = <操作><一首>?(&<music_region>)的?(<歌手性别>)(<唱>的?|的)<歌> 	=> request(region="$1", gender="$2") <0.9>;
export inform72 = (&<music_region>)的?(<歌手性别>)?<唱>?的?(<歌曲类别>) 		=> request(region="$1", gender="$2", tag="$3") <0.9>;
export inform78 = (&<music_region>)的?((<标签>)的)?(<歌曲类别>) => request(region="$1", tag="$2", tag="$3") <0.9>;
export inform73 = (&<music_region>)的?(<歌手性别>)?<唱>?的?<歌> => request(region="$1", gender="$2") <0.9>;

export inform74 = <操作><一首>?(<标签>)的?(&<music_language>)?(<唱>的?|的)(<歌曲类别>) => request(tag="$1", language="$2", tag1="$4") <0.9>;
export inform75 = <操作><一首>?(<标签>)(&<music_language>)?(<唱>的?|的)<歌> 	=> request(tag="$1", language="$2") <0.9>;
export inform75 = <操作><一首>?(<标签>)的?(<歌>|<歌曲类别>) 	=> request(tag="$1") <0.9>;
export inform75 = (<标签>)的?(<标签>) 	=> request(tag="$1", tag="$2") <0.9>;



# 单元测试, 当前domain有重复且分数不同的情况，勿动
export inform = (播放)?(周杰伦)的?(青花瓷) => request(artist="$2", title="$3", domain="music") <0.8>;

export inform = (<操作>)*?(周杰伦)的?(青花瓷) => request(artist="$2", title="$3", domain="music") <0.9>;
export inform = (<操作>)+?(周杰伦)的?(青花瓷) => request(artist="$2", title="$3", domain="music") <0.8>;


