#####################
######变量定义#######
#####################

#include "../comm/digit.lex"

主观意愿 = 听|要|要听|想听|想要听|欣赏|想欣赏;
请求前缀 = 麻烦|请|能|可以|能不能|可不可以;
引出介词 = 给|为|替|帮;
第一人称 = 我|咱|俺|孤|朕|寡人|在下|洒家|本座|老夫|哀家|人家|爷|大爷|老娘|本小姐|本大小姐|本人|我们|咱们;
播放 	= 放|播放|来|播|唱|点播;
单位 	= (一|几)?(首|支|段|点|些);
有	= 有|还有|有没有|有木有|有么有|是否有;
语气词	= 吧|么|吗|嘛|呢;
反问词	= 难道|不会|莫非|难不成|怎么|怎么会|怎能;

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
# 数字排名	= (排名)?(前|top|TOP)<一百以内数字>;
# 排序	= 最?(新|流行|热门);



######################
######expression######
######################

# [title]
# 独词句:告白气球
# 祈使句-主体为人:(我) (听)/(要)/(要听)/(想听)/(想要听) (首)/(一首) 告白气球
#		我要听首告白气球
#		(一首) 告白气球
# 祈使句-主体为机器:(给我) (放)/(播放)/(来) (首)/(一首) 告白气球
# 疑问句:(还)有告白气球吗?/有告白气球吧?
# 	(有没有)/(是否有)告白气球?
# 反问句:难道没有告白气球吗?/不会没有告白气球吧?
# export inform00 = <第一人称>?<主观意愿>?<单位>?(&<music_title>)  =>  request(title="$1")<0.3>;
# export inform00 = <请求前缀>?(<引出介词><第一人称>)?<播放>?<单位>?(&<music_title>)  =>  request(title="$2")<0.3>;
# export inform00 = (<第一人称>?<主观意愿>?|(<请求前缀>?(<引出介词><第一人称>)?<播放>?))<单位>?(&<music_title>)  =>  request(title="$2")<0.3>;

# 上一轮:周杰伦的歌
# 告白气球
# 告白气球吧
# 一首告白气球 ×
# 一首告白气球吧 ×
export inform00 = ^(&<music_title>)<语气词>?$  =>  request(title="$1")<0.3>;
export inform00 = ^<有>(&<music_title>)<语气词>?$  =>  request(title="$1")<0.3>;
export inform00 = ^<反问词>没有(&<music_title>)<语气词>?$  =>  request(title="$1")<0.3>;

# [artist]的
# 独词句:周杰伦的
# 祈使句-主体为人:(我) (听)/(要)/(要听)/(想听)/(想要听)周杰伦的
# 祈使句-主体为机器:(给我) (放)/(播放)/(来) (首)/(一首) 周杰伦的
# 疑问句:(还)有周杰伦的吗?有周杰伦的吗?
#	(有没有)/(是否有)周杰伦的?
# 反问句:难道没有周杰伦的吗?不会没有周杰伦的吧?
# export inform01 = <第一人称>?<主观意愿>?<单位>?(&<music_artist>)的  =>  request(artist="$1")<0.3>;
# export inform01 = <请求前缀>?(<引出介词><第一人称>)?<播放>?<单位>?(&<music_artist>)的  =>  request(artist="$2")<0.3>;
export inform01 = ^换?<单位>?(&<music_artist>)的<语气词>?$  =>  request(artist="$1")<0.3>;
export inform01 = ^<有>(&<music_artist>)的<语气词>?$  =>  request(artist="$1")<0.3>;
export inform01 = ^<反问词>没有(&<music_artist>)的<语气词>?$  =>  request(artist="$1")<0.3>;

# [album](专辑)里(面)的(歌)
# 独词句:依然范特西(专辑)里(面)的(歌)
# 祈使句-主体为人: (听)/(要)/(要听)/(想听)/(想要听) (首)/(一首) 依然范特西(专辑)里(面)的(歌)
# 祈使句-主体为机器:(给我) (放)/(播放)/(来) (首)/(一首) 依然范特西(专辑)里(面)的(歌)
# 疑问句:(还)有依然范特西(专辑)里(面)的(歌)吗?/有依然范特西(专辑)里(面)的(歌)吧? 
#	(有没有)/(是否有)依然范特西(专辑)里(面)的(歌)?
# 反问句:难道没有依然范特西(专辑)里(面)的(歌)?不会没有依然范特西(专辑)里(面)的(歌)?
# export inform02 = (<第一人称>?<主观意愿>?|(<请求前缀>?(<引出介词><第一人称>)?<播放>?))<单位>?(&<music_album>)(专辑)?里?面?的?  =>  request(album="$2")<0.3>;
export inform02 = ^(&<music_album>)(专辑)?里?面?的?<语气词>?$  =>  request(album="$1")<0.3>;
export inform02 = ^<有>(&<music_album>)(专辑)?里?面?的?<语气词>?$  =>  request(album="$1")<0.3>;
export inform02 = ^<反问词>没有(&<music_album>)(专辑)?里?面?的?<语气词>?$  =>  request(album="$1")<0.3>;

# [tag]歌曲标签
# 风格:流行/摇滚/民谣/电子/舞曲/嘻哈/说唱/轻音乐/爵士/乡村/R&B/古典/民族/民族/民俗/英伦
#	金属/重金属/朋克/蓝调/雷鬼/拉丁/另类/古风/后摇/jazz/hip-hop/街舞舞曲  (风)(风格)
# 曲调:舒缓/激情/欢快/悠扬/抒情/动感/优雅
# 情感:怀旧/清新/浪漫/性感/伤感/治愈/放松/悲伤/温暖/孤独/感动/兴奋/快乐/安静/思念/忧伤/嗨/嗨爆/劲爆
# 演唱方式:唱/演唱/独唱/合唱/男女对唱/情歌对唱/情侣对唱
# 演奏方式:钢琴/小提琴/吉他/长笛/萨克斯/单簧管/口琴/二胡/琵琶/唢呐/古筝/纯音乐/无伴奏
# 影视类:主题曲/片头曲/片尾曲/插曲/原声/原声带/影视原声带/电影原声/电视剧原声/主打曲/主打歌
# 主题:爱情/小清新/暗恋/初恋/失恋/爱国/革命/抗战
# 适合人群年龄范围:胎教/宝宝/儿童/小朋友/小孩/年轻人/中年人/青年人/中老年人/老年人/老人
#	60后/70后/80后/90后/00后/六零后/七零后/八零后/九零后/零零后/大学生/高中生/初中生/小学生/中学生
# 适合情境:清晨/夜晚/学习/工作/午休/下午茶/地铁/驾车/运动/跑步/散步/旅行/酒吧
# 戏剧类:京剧/评剧/豫剧/越剧/黄梅戏
export inform03 = ^(<风格>)(风|风格)?的?<语气词>?$  =>  request(tag="$1")<0.3>;
export inform03 = ^(曲调|曲风|曲式|旋律|节奏)?(<曲调>)的?<语气词>?$  =>  request(tag="$2")<0.3>;
export inform03 = ^(适合|符合)?(<情感>)(时听)?的?<语气词>?$  =>  request(tag="$2")<0.3>;
export inform03 = ^(<方式>)(演奏)?的?<语气词>?$  =>  request(tag="$1")<0.3>;
export inform03 = ^(<影视类>)<语气词>?$  =>  request(tag="$1")<0.3>;
export inform03 = ^(关于|有关|与)?(<主题>)(主题)?(有关|相关)?的?<语气词>?$  =>  request(tag="$2")<0.3>;
export inform03 = ^(适合|符合)?(<人群>)听?的?<语气词>?$  =>  request(tag="$2")<0.3>;
export inform03 = ^(适合|符合)?(<情境>)(下|时)?听?的?<语气词>?$  =>  request(tag="$2")<0.3>;
export inform03 = ^(<戏剧类>)<语气词>?$  =>  request(tag="$1")<0.3>;

# [region]
export inform04 = ^(&<music_region>)的?<语气词>?$  =>  request(region="$1")<0.3>;

# [gender]
# 男歌手/男艺人/男明星/男音乐家/男性歌手/男性艺人/男性明星/男性音乐家
export inform05 = ^(<性别>)(性|生)?<称呼>?(唱|演唱)?的?<语气词>?$  =>  request(gender="$1")<0.3>;

# [program]
export inform06 = ^(&<music_program>)里?的?<语气词>?$  =>  request(program="$1")<0.3>;

# [language]
# 语种:华语/欧美/日语/韩语/粤语/德语/法语/中文/英文/日文/韩文/德文/法文
# 华语/华语的/华语的歌/华语歌
# 英文/英文歌/英文的歌/英文唱的/英文唱的歌/英文演唱的/英文演唱的歌
export inform07 = ^(&<music_language>)(唱|演唱)?的?<语气词>?$  =>  request(language="$1")<0.3>;

# [lyrics]
# 歌词中有XXX的歌
export inform08 = ^歌词中(有|包括|含有)(.*)的歌$  =>  request(lyrics="$2")<0.3>;
# Mark以上正则匹配有问题,且领域有问题

# [rank]
# 排序:最新/最流行
# 排名靠前
# 排名在...之内
export inform09 = ^最?(&<music_rank>)的?<语气词>?$  =>  request(rank="$1")<0.3>;

# [program]的[tag]
# 小美好的片尾曲
# [album]的[tag]
# 依然范特西的主打歌
# [artist]的[tag]
# 张惠妹的主打曲
# export inform10 = (&<music_program>)的?(<影视类>)  =>  request(program="$1", tag="$2")<0.4>;
# export inform10 = (&<music_album>)的?(<影视类>)  =>  request(album="$1", tag="$2")<0.4>;
# export inform10 = (&<music_artist>)的?(<影视类>)  =>  request(artist="$1", tag="$2")<0.4>;

# [language][gender]
# [region][gender]
# 华语男歌手的
# 大陆男歌手的
export inform11 = ^(&<music_language>)(<性别>)(性|生)?<称呼>?(唱|演唱)?的?<语气词>?$  =>  request(language="$1", gender="$2")<0.4>;
export inform11 = ^(&<music_region>)(<性别>)(性|生)?<称呼>?(唱|演唱)?的?<语气词>?$  =>  request(region="$1", gender="$2")<0.4>;

# [rank][language]
# 最流行的英文歌


# 放他的歌|歌名
# 放他的(专辑名)
# 代词解析，“他/她/他们/她们”解析成r_person，“那儿/这儿”
# 例如：“听一下他的歌” => request(r_person="他")；  “那儿天气怎么样”=> request(r_location="那儿")；
# 只覆盖“他/她/他们/她们”，“那儿/这儿”这几种case