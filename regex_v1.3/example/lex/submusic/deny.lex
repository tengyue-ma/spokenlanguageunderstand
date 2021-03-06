#####################
######变量定义#######
#####################

#include "../comm/digit.lex"

第一人称 = 我|咱|俺|孤|朕|寡人|在下|洒家|本座|老夫|哀家|人家|爷|大爷|老娘|本小姐|本大小姐|本人|我们|咱们;
否定	= 不要|不要听|不想听;
能否	= 能|可以|能不能|可不可以|可以不可以|能不能够|能否|可否|是否可以;
引出介词 = 给|为|替|帮;
播放	= 播|放|唱|播放|演唱;
歌	= 歌|歌曲|音乐|作品;
好吗后缀	= 好吗|可以吗|行吗|好不好|可不可以|行不行|好不|行不;
风格	= 流行|摇滚|民谣|电子|舞曲|嘻哈|说唱|轻音乐|爵士|乡村|古典|民族|民族|民俗|英伦|金属|重金属|朋克|蓝调|雷鬼|拉丁|另类|古风|后摇|jazz|hip-hop|街舞舞曲|JAZZ|HIP-HOP;
曲调	= 舒缓|激情|欢快|悠扬|抒情|动感|优雅;
情感	= 怀旧|清新|浪漫|性感|伤感|治愈|放松|悲伤|温暖|孤独|感动|兴奋|快乐|安静|思念|忧伤|嗨|嗨爆|劲爆;
演唱方式	= 唱|演唱|独唱|合唱|男女对唱|情歌对唱|情侣对唱;
演奏方式	= 钢琴|小提琴|吉他|长笛|萨克斯|单簧管|口琴|二胡|琵琶|唢呐|古筝|纯音乐|无伴奏;
方式	= <演唱方式>|<演奏方式>;
影视类	= 主题曲|片头曲|片尾曲|插曲|原声|原声带|影视原声带|电影原声|电视剧原声|主打曲|主打歌;
主题	= 爱情|小清新|暗恋|初恋|失恋|爱国|革命|抗战;
泛指人群	= 胎教|宝宝|儿童|小朋友|小孩|年轻人|中年人|青年人|中老年人|老年人|老人|60后|70后|80后|90后|00后|六零后|七零后|八零后|九零后|零零后|大学生|高中生|初中生|小学生|中学生;
具体年龄	= <一百以内数字>岁(的)?<泛指人群>;
人群	= <具体年龄>|<泛指人群>;
情境	= 清晨|夜晚|学习|工作|午休|下午茶|地铁|驾车|运动|跑步|散步|旅行|酒吧;
戏剧类	= 京剧|评剧|豫剧|越剧|黄梅戏;
性别	= 男|女;
称呼	= 歌手|艺人|明星|音乐家;

我不要	= <第一人称>?<否定>;
能不能不给我播放 = <能否>?(不|别)(<引出介词><第一人称>)?<播放>;




######################
######expression######
######################

# deny artist
# 我不要周杰伦的歌
# 能不能别给我放周杰伦的歌
export deny00 = ^(<我不要>|<能不能不给我播放>)(&<music_artist>)的<歌>?<好吗后缀>?$  =>  request(intent="deny", artist="$2")<0.9>;


# deny album
# 不要叶惠美专辑里面的歌
# 能不能别给我放叶惠美专辑里面的歌
export deny01 = ^(<我不要>|<能不能不给我播放>)(&<music_album>)专辑里?面?的<歌>?<好吗后缀>?$  =>  request(intent="deny", album="$2")<0.9>;


# deny tag
export deny02 = ^(<我不要>|<能不能不给我播放>)(<风格>)(风|风格)?的?<歌>?<好吗后缀>?$  =>  request(intent="deny", tag="$2")<0.9>;
export deny02 = ^(<我不要>|<能不能不给我播放>)(曲调|曲风|曲式|旋律|节奏)?(<曲调>)的?<歌>?<好吗后缀>?$  =>  request(intent="deny", tag="$3")<0.9>;
export deny02 = ^(<我不要>|<能不能不给我播放>)(适合|符合)?(<情感>)(时听)?的?<歌>?<好吗后缀>?$  =>  request(intent="deny", tag="$3")<0.9>;
export deny02 = ^(<我不要>|<能不能不给我播放>)(<方式>)(演奏)?的?<歌>?<好吗后缀>?$  =>  request(intent="deny", tag="$2")<0.9>;
export deny02 = ^(<我不要>|<能不能不给我播放>)(关于|有关|与)?(<主题>)(主题)?(有关|相关)?的?<歌>?<好吗后缀>?$  =>  request(intent="deny", tag="$3")<0.9>;
export deny02 = ^(<我不要>|<能不能不给我播放>)(适合|符合)?(<人群>)听?的?<歌>?<好吗后缀>?$  =>  request(intent="deny", tag="$3")<0.9>;
export deny02 = ^(<我不要>|<能不能不给我播放>)(适合|符合)?(<情境>)(下|时)?听?的?<歌>?<好吗后缀>?$  =>  request(intent="deny", tag="$3")<0.9>;
export deny02 = ^(<我不要>|<能不能不给我播放>)(<戏剧类>)<好吗后缀>?$  =>  request(intent="deny", tag="$2")<0.9>;


# deny region
export deny03 = ^(<我不要>|<能不能不给我播放>)(&<music_region>)的?<歌>?<好吗后缀>?$  =>  request(intent="deny", region="$2")<0.9>;

# deny language
export deny04 = ^(<我不要>|<能不能不给我播放>)(&<music_language>)的?<歌>?<好吗后缀>?$  =>  request(intent="deny", language="$2")<0.9>;


# deny gender
export deny04 = ^(<我不要>|<能不能不给我播放>)(<性别>)(性|生)?<称呼>?(唱|演唱)?的?<歌>?<好吗后缀>?$  =>  request(intent="deny", gender="$2")<0.9>;


# deny region + gender
export deny05 = ^(<我不要>|<能不能不给我播放>)(&<music_region>)(<性别>)(性|生)?<称呼>?(唱|演唱)?的?<歌>?<好吗后缀>?$  =>  request(intent="deny", region="$2", gender="$3")<0.9>;


# deny language + gender
export deny06 = ^(<我不要>|<能不能不给我播放>)(&<music_language>)(<性别>)(性|生)?<称呼>?(唱|演唱)?的?<歌>?<好吗后缀>?$  =>  request(intent="deny", language="$2", gender="$3")<0.9>;
