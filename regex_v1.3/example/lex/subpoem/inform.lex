#include "../comm/digit.lex"

#### --------------------通用变量---------------------- ####

我 =  我|俺|大爷|爷|咱|咱们|老娘|本小姐|本大小姐|我们|本人|朕;

他她 = 他|她|这个<诗人>;
一首 = (一|几)?(首|个|段|下|些);
诗人 = 诗人|作者|人;

没有 = 没|没有|么有|木有;
与 = 与|和|跟;
关于 = 关于|有关|相关;
作品 = 作品;

#### --------------------人群槽位---------------------- ####

具体年龄 = <一百以内数字>岁的?<泛指人群>;
泛指人群 = 全家人|年轻人|青少年|小朋友|小孩|小孩子|宝宝|儿童|老年人|老人|60后|70后|80后|90后|00后|６０后|７０后|８０后|９０后|００后|六零后|七零后|八零后|九零后|零零后|大学生|高中生|初中生|小学生|中学生;
人群	 = <具体年龄>|<泛指人群>;

#### --------------------动词变量---------------------- ####

给 = 给|帮|为|替;
想 = 想|要|想要;
播放 = 背|播|放|播放|来|循环播放|朗诵|说;
描述 = 写|描述|描绘|描写|描摹|记叙|记述|表达|抒写|抒发|感叹|歌颂|咏叹|歌咏|咏颂;
属于 = 属于|是|算|算是;
推荐 = 推荐;
适合 = 适合|面向|针对;
可以 = 能|可以;
能不能 = 能不能|可不可以|可以不可以|能不能够|能否|可否;
听学 = 听|学|学习;
听一听 = 听(一?听)?;
创作 = (写|创作)过?的?;

#### --------------------其他槽位---------------------- ####

其他槽位  = (<朝代>|<意象>|<心情>|<主题>|<题材>|<流派>|<经典>);

动物意象 = 燕|燕子|雁|大雁|鸿|鸿雁|鸿鹄|蝉|寒蝉|杜鹃|子规|杜宇|鹧鸪|乌鸦|蟋蟀|鸳鸯|精卫|青鸟|莺|黄莺|猿|猿啼|飞鸟|鱼|鹰|狗|犬|鸡|鸡犬|马|骥|骏;
花意象 = 花|花开|花落|梅|梅花|菊|菊花|莲|荷|荷花|莲花|杨花|梨花|桃花|杏花|丁香|落红|落花;
植物意象 = <花意象>|草|芳草|柳|柳树|柳枝|梧桐|梧|红豆|豆蔻|松|柏|松柏|竹|竹子;
山川意象 = 海|大海|川|江|长江|大江|河|大河|黄河|湖|溪|溪水|溪流|山|五岳|泰山|嵩山|华山|衡山|恒山|庐山|黄山|丘|峰|山峰|山崖;
自然意象  = 月|月亮|月光|雪|雪花|风雪|风|秋风|春风|雨|春雨|秋雨|冰|霜|秋霜|雷|冬雷|春雷;

意象 = <动物意象>|<花意象>|<植物意象>|<山川意象>|<自然意象>;

愉快心情 = 快乐|喜悦|惊喜|欢喜|欢乐|欢快|欢欣|愉悦|愉快|开心|高兴|欣喜|喜爱|喜欢|恬适|恬静|怡然自得|喜出望外|欣喜万分|欣喜若狂;
伤感心情 = 伤感|忧伤|闺怨|幽怨|忧思|幽思|感伤|难过|悲伤|悲恸|悲痛|痛苦|苦痛|壮志未酬|忧心忡忡|怀古伤今;
其他心情 = 感动|思念|离愁|想念|挂念|牵挂|思乡|怀乡|爱国|思念家乡|相思|去国怀乡|离愁别绪;

心情  = (<愉快心情>|<伤感心情>|<其他心情>)(之情)?;

主题 = 父爱|母爱|被贬|战争|爱国|亲情|爱情|友情;
题材 = (咏物|咏怀|边塞|闺怨|送别|离别|赠别|叙事|哲理|说理|怀古|爱国)(诗|词)?;
流派 = (山水田园|田园山水|山水|田园|婉约|豪放|边塞|六朝|桐城|浪漫|现实|浪漫主义|现实主义)(诗|词)?派?;
朝代 = (夏|商|周|西周|东周|秦|西楚|东晋|西晋|南|北|南北|汉|东汉|西汉|东汉末年|三国|魏|蜀|吴|越|隋|唐|宋|北宋|南宋|西夏|元|明|清|近|近现|现)(代|朝|国)?(时期)?;
经典 = 经典;

#### -------------------辅助变量---------------------- ####


疑问前缀 = <我><想>知道|你知道|告诉<我>;
祈使前缀 = (<可以>|<能不能>)?再?<给><我>再?;
人写的 = <诗人>(<创作>|的);
他她写的 = <他她>(<创作>|的);
另一首 = 另?<一首>;

#### -------------------正则规则---------------------- ####

#0a. <poem_title> 独词句
export inform0a1 = ^(&<poem_title>:title)$ => request(intent="inform")<0.9>;

#0b. <poem_title> 祈使句(人)
export inform0b1 = ^(我?<想><听一听>?<一首>?|<听一听><一首>?)(&<poem_title>:title)$ => request(intent="inform")<0.9>;

#0c. <poem_title> 祈使句(机器)
export inform0c1 = ^(<给><我>)?<播放><一首>?(&<poem_title>:title)$ => request(intent="inform")<0.9>;

#0d. <poem_title> 疑问句
export inform0d1 = ^<疑问前缀>?还?(是否|有没|有么)?有(&<poem_title>:title)(吗|么|没|不)?$ => request(intent="inform")<0.9>;
export inform0d2 = ^<疑问前缀>?(&<poem_title>:title)还?(是否|有没|有么)?有(吗|么|没|不)?$ => request(intent="inform")<0.9>;

#0e. <poem_title> 反问句
export inform0e1 = ^(难道|不会|怎么|竟然|都)连?(&<poem_title>:title)(也|都)?<没有>(吗|么|吧)?$ => request(intent="inform")<0.9>;
export inform0e2 = ^(难道|不会|怎么|竟然|都)(也|都)?<没有>(&<poem_title>:title)(吗|么|吧)?$ => request(intent="inform")<0.9>;
export inform0e3 = ^连?(&<poem_title>:title)(难道|不会|怎么|竟然|都)(也|都)?<没有>(吗|么|吧)?$ => request(intent="inform")<0.9>;


#1a. <poem_author> 独词句
export inform0a1 = ^(&<poem_author>:author)(<创作>|的)$ => request(intent="inform")<0.9>;

#1b. <poem_author> 祈使句(人)
export inform0b1 = ^(我?<想><听一听>?<一首>?|<听一听><一首>?)(&<poem_author>:author)(<创作>|的)$ => request(intent="inform")<0.9>;

#1c. <poem_author> 祈使句(机器)
export inform0c1 = ^(<给><我>)?<播放><一首>?(&<poem_author>:author)(<创作>|的)$ => request(intent="inform")<0.9>;

#1d. <poem_author> 疑问句
export inform0d1 = <疑问前缀>?还?(是否|有没|有么)?有(&<poem_author>:author)(<创作>|的)(吗|么|没|不)?$ => request(intent="inform")<0.9>;
export inform0d2 = <疑问前缀>?(&<poem_author>:author)(<创作>|的)还?(是否|有没|有么)?有(吗|么|没|不)?$ => request(intent="inform")<0.9>;

#1e. <poem_author> 反问句
export inform0e1 = ^(难道|不会|怎么|竟然|都)连?(&<poem_author>:author)(<创作>|的)(也|都)?<没有>(吗|么|吧)?$ => request(intent="inform")<0.9>;
export inform0e2 = ^(难道|不会|怎么|竟然|都)(也|都)?<没有>(&<poem_author>:author)(<创作>|的)(吗|么|吧)?$ => request(intent="inform")<0.9>;
export inform0e3 = ^连?(&<poem_author>:author)(<创作>|的)(难道|不会|怎么|竟然|都)(也|都)?<没有>(吗|么|吧)?$ => request(intent="inform")<0.9>;



#2a. <poem_author>的<poem_title> 独词句
export inform1a1 = ^(&<poem_author>:author)(<创作>|的)(&<poem_title>:title)$ => request(intent="inform")<0.9>;

#2b. <poem_author>的<poem_title> 祈使句(人)
export inform1b1 = ^(我?<想><听一听>?<一首>?|<听一听><一首>?)(&<poem_author>:author)(<创作>|的)(&<poem_title>:title)$ => request(intent="inform")<0.9>;

#2c. <poem_author>的<poem_title> 祈使句(机器)
export inform1c1 = ^(<给><我>)?<播放><一首>?(&<poem_author>:author)(<创作>|的)(&<poem_title>:title)$ => request(intent="inform")<0.9>;

#2d. <poem_author>的<poem_title> 疑问句
export inform1d1 = ^<疑问前缀>?还?(是否|有没|有么)?有(&<poem_author>:author)(<创作>|的)(&<poem_title>:title)(吗|么|没|不)?$ => request(intent="inform")<0.9>;
export inform1d2 = ^<疑问前缀>?(&<poem_author>:author)(<创作>|的)(&<poem_title>:title)还?(是否|有没|有么)?有(吗|么|没|不)?$ => request(intent="inform")<0.9>;

#2e. <poem_author>的<poem_title> 反问句
export inform1e1 = ^(难道|不会|怎么|竟然|都)连?(&<poem_author>:author)(<创作>|的)(&<poem_title>:title)(也|都)?<没有>(吗|么|吧)?$ => request(intent="inform")<0.9>;
export inform1e2 = ^(难道|不会|怎么|竟然|都)(也|都)?<没有>(&<poem_author>:author)(<创作>|的)(&<poem_title>:title)(吗|么|吧)?$ => request(intent="inform")<0.9>;
export inform1e3 = ^连?(&<poem_author>:author)(<创作>|的)(&<poem_title>:title)(难道|不会|怎么|竟然|都)(也|都)?<没有>(吗|么|吧)?$ => request(intent="inform")<0.9>;




#3a. 其他槽位<tag> 独词句
export inform2a1 = ^(<关于>|<描述>)?(<其他槽位>:tag)的?$ => request(intent="inform")<0.9>;
export inform2a2 = ^<与>(<其他槽位>:tag)<关于>的?$ => request(intent="inform")<0.9>;
export inform2a3 = ^<属于>(<题材>|<流派>|<朝代>|<经典>):tag的?$ => request(intent="inform")<0.9>;

#3b. 其他槽位<tag> 祈使句(人)
export inform2b1 = ^(我?<想><听一听>?<一首>?|<听一听><一首>?)(<关于>|<描述>)?(<其他槽位>:tag)的?$ => request(intent="inform")<0.9>;
export inform2b2 = ^(我?<想><听一听>?<一首>?|<听一听><一首>?)<与>(<其他槽位>:tag)<关于>的?$ => request(intent="inform")<0.9>;
export inform2b3 = ^(我?<想><听一听>?<一首>?|<听一听><一首>?)<属于>(<题材>|<流派>|<朝代>|<经典>):tag的?$ => request(intent="inform")<0.9>;

#3c. 其他槽位<tag> 祈使句(机器)
export inform2c1 = ^(<给><我>)?<播放><一首>?(<关于>|<描述>)?(<其他槽位>:tag)的?$ => request(intent="inform")<0.9>;
export inform2c2 = ^(<给><我>)?<播放><一首>?<与>(<其他槽位>:tag)<关于>的?$ => request(intent="inform")<0.9>;
export inform2c3 = ^(<给><我>)?<播放><一首>?<属于>(<题材>|<流派>|<朝代>|<经典>):tag的?$ => request(intent="inform")<0.9>;

#3d. 其他槽位<tag> 疑问句
export inform2d1 = ^<疑问前缀>?还?(是否|有没|有么)?有(<关于>|<描述>)?(<其他槽位>:tag)的?(吗|么|没|不)?$ => request(intent="inform")<0.9>;
export inform2d2 = ^<疑问前缀>?还?(是否|有没|有么)?有<与>(<其他槽位>:tag)<关于>的?(吗|么|没|不)?$ => request(intent="inform")<0.9>;
export inform2d3 = ^<疑问前缀>?还?(是否|有没|有么)?有<属于>(<题材>|<流派>|<朝代>|<经典>):tag的?(吗|么|没|不)?$ => request(intent="inform")<0.9>;

export inform2d4 = ^<疑问前缀>?(<关于>|<描述>)?(<其他槽位>:tag)的?还?(是否|有没|有么)?有(吗|么|没|不)?$ => request(intent="inform")<0.9>;
export inform2d5 = ^<疑问前缀>?<与>(<其他槽位>:tag)<关于>的?还?(是否|有没|有么)?有(吗|么|没|不)?$ => request(intent="inform")<0.9>;
export inform2d6 = ^<疑问前缀>?<属于>(<题材>|<流派>|<朝代>|<经典>):tag的?还?(是否|有没|有么)?有(吗|么|没|不)?$ => request(intent="inform")<0.9>;

#3e. 其他槽位<tag> 反问句
export inform2e1 = ^(难道|不会|怎么|竟然|都)连?(<关于>|<描述>)?(<其他槽位>:tag)的?(也|都)?<没有>(吗|么|吧)?$ => request(intent="inform")<0.9>;
export inform2e2 = ^(难道|不会|怎么|竟然|都)连?<与>(<其他槽位>:tag)<关于>的?(也|都)?<没有>(吗|么|吧)?$ => request(intent="inform")<0.9>;
export inform2e3 = ^(难道|不会|怎么|竟然|都)连?<属于>(<题材>|<流派>|<朝代>|<经典>):tag的?(也|都)?<没有>(吗|么|吧)?$ => request(intent="inform")<0.9>;

export inform2e4 = ^(难道|不会|怎么|竟然|都)(也|都)?<没有>(<关于>|<描述>)?(<其他槽位>:tag)的?(吗|么|吧)?$ => request(intent="inform")<0.9>;
export inform2e5 = ^(难道|不会|怎么|竟然|都)(也|都)?<没有><与>(<其他槽位>:tag)<关于>的?(吗|么|吧)?$ => request(intent="inform")<0.9>;
export inform2e6 = ^(难道|不会|怎么|竟然|都)(也|都)?<没有><属于>(<题材>|<流派>|<朝代>|<经典>):tag的?(吗|么|吧)?$ => request(intent="inform")<0.9>;

export inform2e7 = ^连?(<关于>|<描述>)?(<其他槽位>:tag)的?(难道|不会|怎么|竟然|都)(也|都)?<没有>(吗|么|吧)?$ => request(intent="inform")<0.9>;
export inform2e8 = ^连?<与>(<其他槽位>:tag)<关于>的?(难道|不会|怎么|竟然|都)(也|都)?<没有>(吗|么|吧)?$ => request(intent="inform")<0.9>;
export inform2e9 = ^连?<属于>(<题材>|<流派>|<朝代>|<经典>):tag的?(难道|不会|怎么|竟然|都)(也|都)?<没有>(吗|么|吧)?$ => request(intent="inform")<0.9>;




#4a. 适合人群<tag> 独词句
export inform3a1 = ^(<人群>:tag)<可以>?<听学>的?$ => request(intent="inform")<0.9>;
export inform3a2 = ^<适合>(<人群>:tag)<听学>?的?$ => request(intent="inform")<0.9>;

#4b. 适合人群<tag> 祈使句(人)
export inform3b1 = ^(我?<想><听一听>?<一首>?|<听一听><一首>?)(<人群>:tag)<可以>?<听学>的?$ => request(intent="inform")<0.9>;
export inform3b2 = ^(我?<想><听一听>?<一首>?|<听一听><一首>?)<适合>(<人群>:tag)<听学>?的?$ => request(intent="inform")<0.9>;

#4c. 适合人群<tag> 祈使句(机器)
export inform2c1 = ^(<给><我>)?<播放><一首>?(<人群>:tag)<可以>?<听学>的?$ => request(intent="inform")<0.9>;
export inform2c2 = ^(<给><我>)?<播放><一首>?<适合>(<人群>:tag)<听学>?的?$ => request(intent="inform")<0.9>;

#4d. 适合人群<tag> 疑问句
export inform2d1 = ^<疑问前缀>?还?(是否|有没|有么)?有(<人群>:tag)<可以>?<听学>的?(吗|么|没|不)?$ => request(intent="inform")<0.9>;
export inform2d2 = ^<疑问前缀>?还?(是否|有没|有么)?有<适合>(<人群>:tag)<听学>?的?(吗|么|没|不)?$ => request(intent="inform")<0.9>;

export inform2d3 = ^<疑问前缀>?(<人群>:tag)<可以>?<听学>的?还?(是否|有没|有么)?有(吗|么|没|不)?$ => request(intent="inform")<0.9>;
export inform2d4 = ^<疑问前缀>?<适合>(<人群>:tag)<听学>?的?还?(是否|有没|有么)?有(吗|么|没|不)?$ => request(intent="inform")<0.9>;

#4e. 适合人群<tag> 反问句
export inform2e1 = ^(难道|不会|怎么|竟然|都)连?(<人群>:tag)<可以>?<听学>的?(也|都)?<没有>(吗|么|吧)?$ => request(intent="inform")<0.9>;
export inform2e2 = ^(难道|不会|怎么|竟然|都)连?<适合>(<人群>:tag)<听学>?的?(也|都)?<没有>(吗|么|吧)?$ => request(intent="inform")<0.9>;

export inform2e4 = ^(难道|不会|怎么|竟然|都)(也|都)?<没有>(<人群>:tag)<可以>?<听学>的?(吗|么|吧)?$ => request(intent="inform")<0.9>;
export inform2e5 = ^(难道|不会|怎么|竟然|都)(也|都)?<没有><适合>(<人群>:tag)<听学>?的?(吗|么|吧)?$ => request(intent="inform")<0.9>;

export inform2e7 = ^连?(<人群>:tag)<可以>?<听学>的?(难道|不会|怎么|竟然|都)(也|都)?<没有>(吗|么|吧)?$ => request(intent="inform")<0.9>;
export inform2e8 = ^连?<适合>(<人群>:tag)<听学>?的?(难道|不会|怎么|竟然|都)(也|都)?<没有>(吗|么|吧)?$ => request(intent="inform")<0.9>;
