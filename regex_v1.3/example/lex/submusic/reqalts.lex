#####################
######变量定义#######
#####################

#include "../comm/digit.lex"

主观意愿 = 听|要|要听|想听|想要听|欣赏|想欣赏;
请求前缀 = 麻烦|请|能|可以|能不能|可不可以|是否可以;
引出介词 = 给|为|替|帮;
第一人称 = 我|咱|俺|孤|朕|寡人|在下|洒家|本座|老夫|哀家|人家|爷|大爷|老娘|本小姐|本大小姐|本人|我们|咱们;
第三人称 = 他|她;
播放 	= 放|播放|来|播|唱|点播;
单位 	= (一|几)?(首|支|段|点|些);
有	= 还?(有|有没有|有木有|有么有|是否有);
语气词	= 吧|么|吗|嘛|呢;
反问词	= 难道|不会|莫非|难不成|怎么|怎么会|怎能;
其他	= 别|别的|其他|其他的|其余|其余的|另外|另外的;
相似	= (类似|相似|相近|相同)的?;
歌	= 歌|歌曲|音乐|作品;

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
称呼	= 人|歌手|艺人|明星|音乐家;


######################
######expression######
######################
# switch title (remain artist)
# 祈使句: (请求前缀:麻烦|请|能不能|可不可以) (引出介词:给|为) (受事主体:我) (动词:放|播放|换|来) (单位:首|一首) (领属:他|她|&<music_artist>) (唱|演唱) 的 (alternative:其他) (后置修饰:类似|相关|相同) 的 (领域:歌|音乐|歌曲) 
# 疑问句: (询问:还有|有没有) (领属:他|她|&<music_artist>) (唱|演唱) 的 (alternative:其他) (后置修饰:类似|相关|相同) 的 (领域:歌|音乐|歌曲) (语气词:吗|么|嘛|吧)
# 疑问句-疑问中置: (领属:他|她|&<music_artist>) (询问:还有|有没有) (alternative:其他) (后置修饰:类似|相关|相同) 的 (领域:歌|音乐|歌曲) (语气词:吗|么|嘛|吧)
# 疑问词-疑问后置: (领属:他|她|&<music_artist>) (唱|演唱) 的 (alternative:其他) (后置修饰:类似|相关|相同) 的 (领域:歌|音乐|歌曲) (询问:还有|有没有) (语气词:吗|么|嘛|吧)
# 换一首他的歌
# 换一首这个歌手的歌
# 换一首周杰伦的歌
export reqalts00 = ^<请求前缀>?(<引出介词><第一人称>)?<播放>?<单位>?<第三人称>(唱|演唱)?过?的?(<其他>|另一首)<相似>?的?<歌>?$  =>  request(intent="reqalts", artist="OLD_VAL")<0.6>;
export reqalts00 = ^<有><第三人称>(唱|演唱)?过?的?<其他><相似>?的?<歌>?<语气词>?$  =>  request(intent="reqalts", artist="OLD_VAL")<0.6>;
export reqalts00 = ^<第三人称>(还|<有>)?(唱|演唱)?过?的?<其他>?(什么)?<相似>?的?<歌>?<语气词>?$  =>  request(intent="reqalts", artist="OLD_VAL")<0.6>;
# export reqalts00 = <第三人称>(唱|演唱)?过?的?<其他><相似>?的?<歌>?<有><语气词>?  =>  request(intent="reqalts", artist="OLD_VAL")<0.8>;
# 换一首他的歌 | 换歌手 | 换个歌手唱的
export reqalts00 = ^<请求前缀>?(<引出介词><第一人称>)?换<单位>?<第三人称>(唱|演唱)?过?的?(<其他>|另一首)?<相似>?的?<歌>?$  =>  request(intent="reqalts", artist="OLD_VAL")<0.6>;
# 这个歌手
export reqalts00 = ^<请求前缀>?(<引出介词><第一人称>)?<播放>?<单位>?这(个|名)<称呼>(唱|演唱)?过?的?(<其他>|另一首)<相似>?的?<歌>?$  =>  request(intent="reqalts", artist="OLD_VAL")<0.6>;
export reqalts00 = ^<有>这(个|名)<称呼>(唱|演唱)?过?的?<其他><相似>?的?<歌>?<语气词>?$  =>  request(intent="reqalts", artist="OLD_VAL")<0.6>;
export reqalts00 = ^这(个|名)<称呼>(还|<有>)?(唱|演唱)?过?的?<其他>?(什么)?<相似>?的?<歌>?<语气词>?$  =>  request(intent="reqalts", artist="OLD_VAL")<0.6>;
export reqalts00 = ^<请求前缀>?(<引出介词><第一人称>)?换<单位>?这(个|名)<称呼>(唱|演唱)?过?的?(<其他>|另一首)?<相似>?的?<歌>?$  =>  request(intent="reqalts", artist="OLD_VAL")<0.6>;
# (&<music_artist>)
export reqalts00 = ^<请求前缀>?(<引出介词><第一人称>)?<播放>?<单位>?(&<music_artist>)(唱|演唱)?过?的?(<其他>|另一首)<相似>?的?<歌>?$  =>  request(intent="reqalts", artist="$2")<0.6>;
export reqalts00 = ^<有>(&<music_artist>)(唱|演唱)?过?的?<其他><相似>?的?<歌>?<语气词>?$  =>  request(intent="reqalts", artist="$1")<0.6>;
export reqalts00 = ^(&<music_artist>)(还|<有>)(唱|演唱)?过?的?<其他>?(什么)?<相似>?的?<歌>?<语气词>?$  =>  request(intent="reqalts", artist="$1")<0.6>;
export reqalts00 = ^<请求前缀>?(<引出介词><第一人称>)?换<单位>?(&<music_artist>)(唱|演唱)?过?的?(<其他>|另一首)<相似>?的?<歌>?$  =>  request(intent="reqalts", artist="$2")<0.6>;
# 换张学友的歌 × | 换张学友的其他歌 OK | 换张学友的 reqalts
# 换一首 control
# 换一首歌 switch title and artist unreserved
export reqalts00 = ^<请求前缀>?(<引出介词><第一人称>)?换<单位>?(<其他>|另一首)?<相似>?的?<歌>$  =>  request(intent="reqalts")<0.5>;
# 还有别的吗 | 还有别的歌吗
export reqalits00 = ^<有>别的<歌>?<语气词>?$  =>  request(intent="reqalts")<0.5>;


# switch artist (remain title or not and title is fixed or not)
# 泛指: (alternative:其他) 人 (动词:唱|演唱) 的 (这首歌|&<music_title>)
#	(这首歌|&<music_title>) 的 (alternative:其他) 版本
# 特指: (&<music_artist>) (动词:唱|演唱) 的 (这首歌|&<music_title>)	× (guixing说不属于sub-music)
# 周杰伦唱的这首歌 ?
# 祈使句: (请求前缀:麻烦|请|能不能|可不可以) (引出介词:给|为) (受事主体:我) (动词:放|播放|换|来) (单位:首|一首) (alternative:其他) 人 (动词:唱|演唱) 的 (这首歌|&<music_title>)
# 祈使句: (请求前缀:麻烦|请|能不能|可不可以) (引出介词:给|为) (受事主体:我) (动词:放|播放|换|来) (这首歌|&<music_title>) 的 (alternative:其他) 版本
# 疑问句: (询问:还有|有没有) (alternative:其他) 人 (动词:唱|演唱) 的 (这首歌|&<music_title>) (语气词:吗|么|嘛|吧)
# 疑问句: (询问:还有|有没有) (这首歌|&<music_title>) 的 (alternative:其他) 版本 (语气词:吗|么|嘛|吧)
# 疑问句-疑问后置: (alternative:其他) 人 (动词:唱|演唱) 的 (这首歌|&<music_title>) (询问:还有|有没有) (语气词:吗|么|嘛|吧)
# 疑问句-疑问后置: (这首歌|&<music_title>) 的 (alternative:其他) 版本 (询问:还有|有没有) (语气词:吗|么|嘛|吧)
# 换个歌手
# 换个歌手唱的这首歌
export reqalts01 = ^<请求前缀>?(<引出介词><第一人称>)?<播放>?<单位>?<其他>(人|<称呼>)(唱|演唱)?过?的这首<歌>$  =>  request(intent="reqalts", artist="DENY", title="OLD_VAL")<0.6>;
export reqalts01 = ^<请求前缀>?(<引出介词><第一人称>)?<播放>?这首<歌>的?(<其他>|另一种|另一个)版本$  =>  request(intent="reqalts", artist="DENY", title="OLD_VAL")<0.6>;
export reqalts01 = ^<有><其他>(人|<称呼>)(唱|演唱)?过?的这首<歌><语气词>?$  =>  request(intent="reqalts", artist="DENY", title="OLD_VAL")<0.6>;
export reqalts01 = ^<有>这首<歌>的?(<其他>|另一种|另一个)版本<语气词>?$  =>  request(intent="reqalts", artist="DENY", title="OLD_VAL")<0.6>;
export reqalts01 = ^<请求前缀>?(<引出介词><第一人称>)?换个?<称呼>(唱|演唱)?过?的?$  =>  request(intent="reqalts", artist="DENY")<0.6>;
export reqalts01 = ^<请求前缀>?(<引出介词><第一人称>)?换个?<称呼>(唱|演唱)?过?的这首<歌>$  =>  request(intent="reqalts", artist="DENY", title="OLD_VAL")<0.6>;
export reqalts01 =  ^(<请求前缀>?(<引出介词><第一人称>)?<播放>?|<有>?)(<其他>|另一种|另一个)版本的?这首<歌><语气词>?$  =>  request(intent="reqalts", artist="DENY", title="OLD_VAL")<0.6>;

# 其他人唱的青花瓷
# 青花瓷的其他版本
# 换个歌手唱的青花瓷
export reqalts01 = ^<请求前缀>?(<引出介词><第一人称>)?<播放>?<单位>?<其他>(人|<称呼>)(唱|演唱)?过?的(&<music_title>)$  =>  request(intent="reqalts", artist="DENY", title="$4")<0.6>;
export reqalts01 = ^<请求前缀>?(<引出介词><第一人称>)?<播放>?(&<music_title>)的?(<其他>|另一种|另一个)版本$  =>  request(intent="reqalts", artist="DENY", title="$2")<0.6>;
export reqalts01 = ^<有><其他>(人|<称呼>)(唱|演唱)?过?的(&<music_title>)<语气词>?$  =>  request(intent="reqalts", artist="DENY", title="$3")<0.6>;
export reqalts01 = ^<有>(&<music_title>)的?(<其他>|另一种|另一个)版本<语气词>?$  =>  request(intent="reqalts", artist="DENY", title="$1")<0.6>;
export reqalts01 = ^<请求前缀>?(<引出介词><第一人称>)?换个?<称呼>(唱|演唱)?过?的(&<music_title>)$  =>  request(intent="reqalts", artist="DENY", title="$3")<0.6>;
export reqalts01 = ^(<请求前缀>?(<引出介词><第一人称>)?<播放>?|<有>?)(<其他>|另一种|另一个)版本的?(&<music_title>)<语气词>?$  =>  request(intent="reqalts", artist="DENY", title="$3")<0.6>;


# switch album (remain artist or not)
# 祈使句: (请求前缀:麻烦|请|能不能|可不可以) (引出介词:给|为) (受事主体:我) (动词:放|播放|换|来) (单位:首|一首) (领属:他|她|&<music_artist>) 的 (alternative:其他) (后置修饰:类似|相关|相同) 的 (slot:专辑)
# 疑问句: (询问:还有|有没有) (领属:他|她|&<music_artist>) 的 (alternative:其他) (后置修饰:类似|相关|相同) 的 (slot:专辑) (语气词:吗|么|嘛|吧)
# 疑问句-疑问中置: (领属:他|她|&<music_artist>) (询问:还有|有没有) (alternative:其他) (后置修饰:类似|相关|相同) 的 (slot:专辑)
# 疑问词-疑问后置: (领属:他|她|&<music_artist>) 的 (alternative:其他) (后置修饰:类似|相关|相同) 的 (slot:专辑) (询问:还有|有没有) (语气词:吗|么|嘛|吧)
# 其他专辑
# 他的其他专辑
export reqalts02 = ^<请求前缀>?(<引出介词><第一人称>)?<播放>?<单位>?(<其他>|另一首)<相似>?的?专辑里?的?<歌>?$  =>  request(intent="reqalts", album="DENY")<0.6>;
export reqalts02 = ^<请求前缀>?(<引出介词><第一人称>)?<播放>?<单位>?<第三人称>(唱|演唱)?过?的?(<其他>|另一首)<相似>?的?专辑里?的?<歌>?$  =>  request(intent="reqalts", album="DENY", artist="OLD_VAL")<0.6>;
export reqalts02 = ^<有>(<其他>|另一首)<相似>?的?专辑里?的?<歌>?<语气词>?$  =>  request(intent="reqalts", album="DENY")<0.6>;
export reqalts02 = ^<有><第三人称>(唱|演唱)?过?的?(<其他>|另一首)<相似>?的?专辑里?的?<歌>?<语气词>?$  =>  request(intent="reqalts", album="DENY", artist="OLD_VAL")<0.6>;
# export reqalts02 = <请求前缀>?(<引出介词><第一人称>)?换(<单位>|张)?<其他>?专辑里?的?<歌>?  =>  request(intent="reqalts", album="DENY")<0.6>;
# 换张专辑
export reqalts02 = ^<请求前缀>?(<引出介词><第一人称>)?换(张|<单位>?)<其他>?<相似>?的?专辑里?的?<歌>?$  =>  request(intent="reqalts", album="DENY")<0.6>;
# (换)这个歌手的其他专辑
export reqalts02 = ^<请求前缀>?(<引出介词><第一人称>)?(<播放>?|换?成?)<单位>?这(个|名)<称呼>(唱|演唱)?过?的?(<其他>|另一首)<相似>?的?专辑里?的?<歌>?$  =>  request(intent="reqalts", album="DENY", artist="OLD_VAL")<0.6>;
export reqalts02 = ^<有>这(个|名)<称呼>(唱|演唱)?过?的?(<其他>|另一首)<相似>?的?专辑里?的?<歌>?<语气词>?$  =>  request(intent="reqalts", album="DENY", artist="OLD_VAL")<0.6>;
# (换)周杰伦的其他专辑
export infrom02 = ^<请求前缀>?(<引出介词><第一人称>)?(<播放>?|换?成?)<单位>?(&<music_artist>)(唱|演唱)?过?的?(<其他>|另一首)<相似>?的?专辑里?的?<歌>?$  =>  request(intent="reqalts", album="DENY", artist="$3")<0.6>;
export infrom02 = ^<有>(&<music_artist>)(唱|演唱)?过?的?(<其他>|另一首)<相似>?的?专辑里?的?<歌>?<语气词>?$  =>  request(intent="reqalts", album="DENY", artist="$1")<0.6>;


# switch tag (remain artist)	×
# 祈使句: (请求前缀:麻烦|请|能不能|可不可以) (引出介词:给|为) (受事主体:我) (动词:放|播放|换|来) (单位:首|一首) (领属:他|她|&<music_artist>) 的 (alternative:其他) (后置修饰:类似|相关|相同) 的 (tag:<风格>) (风|风格) 的
# 祈使句: (请求前缀:麻烦|请|能不能|可不可以) (引出介词:给|为) (受事主体:我) (动词:放|播放|换|来) (单位:首|一首) (领属:他|她|&<music_artist>) 的 (alternative:其他) (后置修饰:类似|相关|相同) 的 (曲调|曲风|曲式|旋律|节奏) (tag:<曲调>) 的
# 祈使句: (请求前缀:麻烦|请|能不能|可不可以) (引出介词:给|为) (受事主体:我) (动词:放|播放|换|来) (单位:首|一首) (领属:他|她|&<music_artist>) 的 (alternative:其他) (后置修饰:类似|相关|相同) 的 (适合|符合) (tag:<情感>) (时听) 的
# 祈使句: (请求前缀:麻烦|请|能不能|可不可以) (引出介词:给|为) (受事主体:我) (动词:放|播放|换|来) (单位:首|一首) (领属:他|她|&<music_artist>) 的 (alternative:其他) (后置修饰:类似|相关|相同) 的 (tag:<方式>) (演奏) 的
export reqalts000 = ^(换个|<其他>)风格的?<歌>?$  =>  request(intent="reqalts", tag="DENY")<0.6>;


# switch artist (remain region or not)
# 祈使句: (请求前缀:麻烦|请|能不能|可不可以) (引出介词:给|为) (受事主体:我) (动词:放|播放|换|来) (单位:首|一首) (alternative:其他) (slot:region) (称呼:歌手|艺人|明星|音乐家) 唱的
# 疑问句: (询问:还有|有没有) (alternative:其他) (slot:region) (称呼:歌手|艺人|明星|音乐家) 唱的 (语气词:吗|么|嘛|吧)
# 疑问句-疑问后置: (alternative:其他) (slot:region) (称呼:歌手|艺人|明星|音乐家) 唱的 (询问:还有|有没有) (语气词:吗|么|嘛|吧)
# 其他歌手
# 其他欧美歌手|欧美其他歌手
export reqalts03 = ^<请求前缀>?(<引出介词><第一人称>)?<播放>?<单位>?<其他><称呼>唱?的?$  =>  request(intent="reqalts", artist="DENY")<0.6>;
export reqalts03 = ^<请求前缀>?(<引出介词><第一人称>)?(<播放>?<单位>?|换成|换个)?<其他>(&<music_region>)<称呼>唱?的?$  =>  request(intent="reqalts", artist="DENY", region="$3")<0.6>;
export reqalts03 = ^<请求前缀>?(<引出介词><第一人称>)?(<播放>?<单位>?|换成|换个)?(&<music_region>)<其他><称呼>唱?的?$  =>  request(intent="reqalts", artist="DENY", region="$3")<0.6>;
export reqalts03 = ^<有><其他><称呼>唱?的?<语气词>?$  =>  request(intent="reqalts", artist="DENY")<0.6>;
export reqalts03 = ^<有><其他>(&<music_region>)<称呼>唱?的?<语气词>?$  =>  request(intent="reqalts", artist="DENY", region="$1")<0.6>;
export reqalts03 = ^<有>(&<music_region>)<其他><称呼>唱?的?<语气词>?$  =>  request(intent="reqalts", artist="DENY", region="$1")<0.6>;
export reqalts03 = ^<请求前缀>?(<引出介词><第一人称>)?换(成|个)(&<music_region>)<称呼>唱?的?$  =>  request(intent="reqalts", artist="DENY", region="$2")<0.6>;


# switch artist (remain gender)
# 祈使句: (请求前缀:麻烦|请|能不能|可不可以) (引出介词:给|为) (受事主体:我) (动词:放|播放|换|来) (单位:首|一首) (alternative:其他) (slot:gender) (后缀:性|生) (称呼:歌手|艺人|明星|音乐家) 唱的
# 疑问句: (询问:还有|有没有) (alternative:其他) (slot:gender) (后缀:性|生) (称呼:歌手|艺人|明星|音乐家) 唱的 (语气词:吗|么|嘛|吧)
# 疑问句-疑问后置: (alternative:其他) (slot:gender) (后缀:性|生) (称呼:歌手|艺人|明星|音乐家) 唱的 (询问:还有|有没有) (语气词:吗|么|嘛|吧)
export reqalts04 = ^<请求前缀>?(<引出介词><第一人称>)?换(成|个)(<性别>)(性|生)?<称呼>?唱?的?$  =>  request(intent="reqalts", artist="DENY", gender="$3")<0.6>;
export reqalts04 = ^<请求前缀>?(<引出介词><第一人称>)?(<播放>?<单位>?|换成|换个)?<其他>(<性别>)(性|生)?<称呼>?唱?的?$  =>  request(intent="reqalts", artist="DENY", gender="$3")<0.6>;
export reqalts04 = ^<有><其他>(<性别>)(性|生)?<称呼>?唱?的?<语气词>?$  =>  request(intent="reqalts", artist="DENY", gender="$1")<0.6>;


# switch title (program里)
# 祈使句: (请求前缀:麻烦|请|能不能|可不可以) (引出介词:给|为) (受事主体:我) (动词:放|播放|换|来) (单位:首|一首)  (slot:program) 里面的 (alternative:其他) (领域:歌|音乐|歌曲)|(tag:插曲)
# 疑问句: (询问:还有|有没有) (slot:program) 里面的 (alternative:其他) (领域:歌|音乐|歌曲)|(tag:插曲) (语气词:吗|么|嘛|吧)
# 疑问句-疑问后置: (slot:program) 里面的 (alternative:其他) (领域:歌|音乐|歌曲)|(tag:插曲) (询问:还有|有没有) (语气词:吗|么|嘛|吧)
# 小美好里面的其他歌曲
# 小美好里面的其他插曲(tag)
export reqalts05 = ^<请求前缀>?(<引出介词><第一人称>)?<播放>?<单位>?(&<music_program>)里?面?的?<其他><歌>?(<影视类>)?$  =>  request(intent="reqalts", program="$2", tag="$3")<0.6>;
export reqalts05 = ^<有>(&<music_program>)里?面?的?<其他><歌>?(<影视类>)?<语气词>?$  =>  request(intent="reqalts", program="$1", tag="$2")<0.6>;


# switch title (remain language)
# 祈使句: (请求前缀:麻烦|请|能不能|可不可以) (引出介词:给|为) (受事主体:我) (动词:放|播放|换|来) (单位:首|一首) (alternative:其他) (slot:language) 唱的 (领域:歌|音乐|歌曲)
# 疑问句: (询问:还有|有没有) (alternative:其他) (slot:language) 唱的 (领域:歌|音乐|歌曲) (语气词:吗|么|嘛|吧)
# 疑问句-疑问后置: (alternative:其他) (slot:language) 唱的 (领域:歌|音乐|歌曲) (询问:还有|有没有) (语气词:吗|么|嘛|吧)
export reqalts06 = ^<请求前缀>?(<引出介词><第一人称>)?<播放>?<单位>?<其他>(&<music_language>)唱?的?<歌>?$  =>  request(intent="reqalts", language="$2")<0.6>;
export reqalts06 = ^<请求前缀>?(<引出介词><第一人称>)?<播放>?<单位>?(&<music_language>)唱?的?<其他><歌>?$  =>  request(intent="reqalts", language="$2")<0.6>;
export reqalts06 = ^<有><其他>(&<music_language>)唱?的?<歌>?<语气词>?$  =>  request(intent="reqalts", language="$1")<0.6>;
export reqalts06 = ^<有>(&<music_language>)唱?的?<其他><歌>?<语气词>?$  =>  request(intent="reqalts", language="$1")<0.6>;


# switch title (remain lyrics)
# 祈使句: (请求前缀:麻烦|请|能不能|可不可以) (引出介词:给|为) (受事主体:我) (动词:放|播放|换|来) (单位:首|一首) (alternative:其他) 歌词中 (有|包括|含有) (.*) 的 (领域:歌|音乐|歌曲)
# 疑问句: (询问:还有|有没有) (alternative:其他) 歌词中 (有|包括|含有) (.*) 的 (领域:歌|音乐|歌曲) (语气词:吗|么|嘛|吧)
# 疑问句-疑问后置: (alternative:其他) 歌词中 (有|包括|含有) (.*) 的 (领域:歌|音乐|歌曲) (询问:还有|有没有) (语气词:吗|么|嘛|吧)
export reqalts07 = ^<请求前缀>?(<引出介词><第一人称>)?<播放>?<单位>?<其他>歌词中(有|包括|含有)(.*)的歌$  =>  request(intent="reqalts", lyrics="$3")<0.6>;
export reqalts07 = ^<有><其他>歌词中(有|包括|含有)(.*)的歌<语气词>?$  =>  request(intent="reqalts", lyrics="$2")<0.6>;


# switch title (remain rank)
# 祈使句: (请求前缀:麻烦|请|能不能|可不可以) (引出介词:给|为) (受事主体:我) (动词:放|播放|换|来) (单位:首|一首) (alternative:其他) 最 (slot:rank) 的 (领域:歌|音乐|歌曲)
# 疑问句: (询问:还有|有没有) (alternative:其他) 最 (slot:rank) 的 (领域:歌|音乐|歌曲) (语气词:吗|么|嘛|吧)
# 疑问句-疑问后置: (alternative:其他) 最 (slot:rank) 的 (领域:歌|音乐|歌曲) (询问:还有|有没有) (语气词:吗|么|嘛|吧)
export reqalts08 = ^<请求前缀>?(<引出介词><第一人称>)?<播放>?<单位>?<其他>最?(&<music_rank>)的?<歌>?$  =>  request(intent="reqalts", rank="$2")<0.6>;
export reqalts08 = ^<有><其他>最?(&<music_rank>)的?<歌>?<语气词>?$  =>  request(intent="reqalts", rank="$1")<0.6>;


# switch artist (remain region & gender)
# 祈使句: (请求前缀:麻烦|请|能不能|可不可以) (引出介词:给|为) (受事主体:我) (动词:放|播放|换|来) (单位:首|一首) (alternative:其他) (slot:region) (slot:gender) (后缀:性|生) (称呼:歌手|艺人|明星|音乐家) 唱的
# 疑问句: (询问:还有|有没有) (alternative:其他) (slot:region) (slot:gender) (后缀:性|生) (称呼:歌手|艺人|明星|音乐家) 唱的 (语气词:吗|么|嘛|吧)
# 疑问句-疑问后置: (alternative:其他) (slot:region) (slot:gender) (后缀:性|生) (称呼:歌手|艺人|明星|音乐家) 唱的 (询问:还有|有没有) (语气词:吗|么|嘛|吧)
export reqalts09 = ^<请求前缀>?(<引出介词><第一人称>)?换(成|个)(&<music_region>)(<性别>)(性|生)?<称呼>?唱?的?$  =>  request(intent="reqalts", artist="DENY", region="$3", gender="$4")<0.8>;
export reqalts09 = ^<请求前缀>?(<引出介词><第一人称>)?(<播放>?<单位>?|换成|换个)?<其他>(&<music_region>)(<性别>)(性|生)?<称呼>?唱?的?$  =>  request(intent="reqalts", artist="DENY", region="$3", gender="$4")<0.8>;
export reqalts09 = ^<请求前缀>?(<引出介词><第一人称>)?(<播放>?<单位>?|换成|换个)?(&<music_region>)<其他>(<性别>)(性|生)?<称呼>?唱?的?$  =>  request(intent="reqalts", artist="DENY", region="$3", gender="$4")<0.8>;
export reqalts09 = ^<有><其他>(&<music_region>)(<性别>)(性|生)?<称呼>?唱?的?<语气词>?$  =>  request(intent="reqalts", artist="DENY", region="$1", gender="$2")<0.8>;
export reqalts09 = ^<有>(&<music_region>)<其他>(<性别>)(性|生)?<称呼>?唱?的?<语气词>?$  =>  request(intent="reqalts", artist="DENY", region="$1", gender="$2")<0.8>;


# switch artist (remain language & gender)
# 祈使句: (请求前缀:麻烦|请|能不能|可不可以) (引出介词:给|为) (受事主体:我) (动词:放|播放|换|来) (单位:首|一首) (alternative:其他) (slot:language) (slot:gender) (后缀:性|生) (称呼:歌手|艺人|明星|音乐家) 唱的
# 疑问句: (询问:还有|有没有) (alternative:其他) (slot:language) (slot:gender) (后缀:性|生) (称呼:歌手|艺人|明星|音乐家) 唱的 (语气词:吗|么|嘛|吧)
# 疑问句-疑问后置: (alternative:其他) (slot:language) (slot:gender) (后缀:性|生) (称呼:歌手|艺人|明星|音乐家) 唱的 (询问:还有|有没有) (语气词:吗|么|嘛|吧)
export reqalts10 = ^<请求前缀>?(<引出介词><第一人称>)?换(成|个)(&<music_language>)(<性别>)(性|生)?<称呼>?唱?的?$  =>  request(intent="reqalts", artist="DENY", language="$3", gender="$4")<0.8>;
export reqalts10 = ^<请求前缀>?(<引出介词><第一人称>)?(<播放>?<单位>?|换成|换个)?<其他>(&<music_language>)(<性别>)(性|生)?<称呼>?唱?的?$  =>  request(intent="reqalts", artist="DENY", language="$3", gender="$4")<0.8>;
export reqalts10 = ^<请求前缀>?(<引出介词><第一人称>)?(<播放>?<单位>?|换成|换个)?(&<music_language>)<其他>(<性别>)(性|生)?<称呼>?唱?的?$  =>  request(intent="reqalts", artist="DENY", language="$3", gender="$4")<0.8>;
export reqalts10 = ^<有><其他>(&<music_language>)(<性别>)(性|生)?<称呼>?唱?的?<语气词>?$  =>  request(intent="reqalts", artist="DENY", language="$1", gender="$2")<0.8>;
export reqalts10 = ^<有>(&<music_language>)<其他>(<性别>)(性|生)?<称呼>?唱?的?<语气词>?$  =>  request(intent="reqalts", artist="DENY", language="$1", gender="$2")<0.8>;


#换个歌手
#换个歌手的这首歌
#换张专辑
#换这个歌手的其他专辑里的歌
#换个风格
