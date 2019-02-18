#include "../comm/digit.lex"

#### --------------------通用变量---------------------- ####

我 =  我|俺|大爷|爷|咱|咱们|老娘|本小姐|本大小姐|我们|本人|朕;

他她 = 他|她|这个<诗人>;
他她 = 他|她;
一首 = (一|几)?(首|个|段|下|些);
诗人 = 诗人|作者|人;
版本 = 版|版本;

没有 = 没|没有|木有|么有;
其他 = 别|别的|其他|其他的|其余|其余的|另外|另外的;
相似 = (类似|相似|相近|相同)的?;
与 = 与|和|跟;
关于 = 关于|有关|相关;
诗 = 诗|词|诗词|诗歌|作品;

#### --------------------人群槽位---------------------- ####

具体年龄 = <一百以内数字>岁的?<泛指人群>;
泛指人群 = 全家人|年轻人|青少年|小朋友|小孩|小孩子|宝宝|儿童|老年人|老人|60后|70后|80后|90后|00后|６０后|７０后|８０后|９０后|００后|六零后|七零后|八零后|九零后|零零后|大学生|高中生|初中生|小学生|中学生;
人群	 = <具体年龄>|<泛指人群>;

#### --------------------动词变量---------------------- ####

给 = 给|帮|为|替;
想 = 想|要|想要;
播放 = 背|播|放|听|播放|来|循环播放|朗诵|说;
换 = 换|换成;
描述 = 写|描述|描绘|描写|描摹|记叙|记述|表达|抒写|抒发|感叹|歌颂|咏叹|歌咏|咏颂;
属于 = 属于|是|算|算是;
推荐 = 推荐;
适合 = 适合|面向|针对|给;
可以 = 能|可以;
能不能 = 能不能|可不可以|可以不可以|能不能够|能否|可否;
听学 = 听|学|学习;
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

心情  = (<愉快心情>|<伤感心情>|<其他心情>)之情?;

主题 = 父爱|母爱|被贬|战争|爱国|亲情|爱情|友情;
题材 = (咏物|咏怀|边塞|闺怨|送别|离别|赠别|叙事|哲理|说理|怀古|爱国)(诗|词)?;
流派 = (山水田园|田园山水|山水|田园|婉约|豪放|边塞|六朝|桐城|浪漫|现实|浪漫主义|现实主义)(诗|词)?派?;
朝代 = (夏|商|周|西周|东周|秦|西楚|东晋|西晋|南|北|汉|东汉|西汉|东汉末年|三国|魏|蜀|吴|越|隋|唐|宋|北宋|南宋|西夏|元|明|清|近|近现|现)(代|朝|国)?时期?;
经典 = 经典;

#### -------------------辅助变量---------------------- ####

其他相似 = <其他>|<相似>;
疑问前缀 = <我><想>知道|你知道|告诉<我>;
祈使前缀 = (<可以>|<能不能>)?再?<给><我>再?;
还有其他 = (还有(<没有>)?<其他>?)|(还?有(<没有>)?<其他>);
还有其他相似 = (还有(<没有>)?<其他相似>?)|(还?有(<没有>)?<其他相似>);
有其他相似 = 还?有(<没有>)?<其他相似>?;
人写的 = <诗人>(<创作>|的);
他她写的 = <他她>(<创作>|的);
另一首 = 另?<一首>;

#### -------------------正则规则---------------------- ####

#0a.泛指“还有其他的吗”,不含作者,疑问句
export reqalts0a1 = <疑问前缀>?<还有其他相似><版本>?的?<推荐>?的?(吗|没|么)?$ => request(intent="reqalts")<0.9>;
export reqalts0a2 = <疑问前缀>?<其他相似><版本>?的?还?有<没有>?<推荐>?的?(吗|没|么)?$ => request(intent="reqalts")<0.9>;


#0b.泛指“换首其他的”,不含作者,祈使句
export reqalts0b1 = <祈使前缀>?再?(<播放>|<换>)上?<另一首><其他相似>?<版本>?的?(吗|没|么)?$ => request(intent="reqalts")<0.9>;
export reqalts0b3 = <其他相似><版本>?的?<祈使前缀>?(<播放>|<换>)上?<另一首>(吗|没|么)?$ => request(intent="reqalts")<0.9>;


#0c.泛指“有没有其他人写的”,拒绝作者,疑问句
export reqalts0c1 = <疑问前缀>?<还有其他><人写的><推荐>?(吗|没|么)?$ => request(intent="reqalts", author="DENY")<0.9>;
export reqalts0c2 = <疑问前缀>?<其他><人写的>还?有<没有>?<推荐>?的?(吗|没|么)?$ => request(intent="reqalts", author="DENY")<0.9>;


#0d.泛指“换首其他人写的”,拒绝作者,祈使句
export reqalts0d1 = <祈使前缀>?(<播放>|<换>)上?<另一首><其他><人写的>(吗|没|么)?$ => request(intent="reqalts", author="DENY")<0.9>;
export reqalts0d2 = <其他><人写的><祈使前缀>?(<播放>|<换>)上?<另一首>(吗|没|么)?$ => request(intent="reqalts", author="DENY")<0.9>;


#0e.泛指“他还有其他<诗>吗”,继承作者,疑问句
export reqalts0e1 = <疑问前缀>?(<他她>:r_person)还?(有<没有>?|有<没有>?<创作>|<创作>)<其他相似>的?(<诗>)?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts0e2 = <疑问前缀>?还?有<没有>?(<他她>:r_person)(<创作>|的)<其他相似>(<诗>)?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts0e2 = <疑问前缀>?还?有<没有>?<其他相似>(<他她>:r_person)(<创作>|的))(<诗>)?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts0e3 = <疑问前缀>?(<他她>:r_person)(<创作>|的)<其他相似>(<诗>)?还?有<没有>?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts0e3 = <疑问前缀>?<其他相似>(<他她>:r_person)(<创作>|的)(<诗>)?还?有<没有>?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;


#0f.泛指“换首他的其他<诗>”,继承作者,祈使句
export reqalts0f1 = <祈使前缀>?(<播放>|<换>)上?<另一首>(<他她>:r_person)(<创作>|的)<其他相似>(<诗>)?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts0f1 = <祈使前缀>?(<播放>|<换>)上?<另一首><其他相似>(<他她>:r_person)(<创作>|的)(<诗>)?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts0f2 = (<他她>:r_person)(<创作>|的)<其他相似>(<诗>)?<祈使前缀>?(<播放>|<换>)上?<另一首>(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts0f2 = <其他相似>(<他她>:r_person)(<创作>|的)(<诗>)?<祈使前缀>?(<播放>|<换>)上?<另一首>(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;




#1a.还有其他关于<其他槽位>的吗？
export reqalts1a1 = <疑问前缀>?<还有其他相似>(<关于>|<描述>)?(<其他槽位>:tag)的?(吗|没|么)?$ => request(intent="reqalts")<0.9>;
export reqalts1a2 = <疑问前缀>?<还有其他相似><与>(<其他槽位>:tag)<关于>的?(吗|没|么)?$ => request(intent="reqalts")<0.9>;
export reqalts1a3 = <疑问前缀>?<还有其他相似><属于>(<题材>|<流派>|<朝代>|<经典>):tag的?(吗|没|么)?$ => request(intent="reqalts")<0.9>;

export reqalts1a4 = <疑问前缀>?<其他相似>(<关于>|<描述>)?(<其他槽位>:tag)的?还?有(没有)?(吗|没|么)?$ => request(intent="reqalts")<0.9>;
export reqalts1a5 = <疑问前缀>?<其他相似><与>(<其他槽位>:tag)<关于>的?还?有(没有)?(吗|没|么)?$ => request(intent="reqalts")<0.9>;
export reqalts1a6 = <疑问前缀>?<其他相似><属于>(<题材>|<流派>|<朝代>|<经典>):tag的?还?有(没有)?(吗|没|么)?$ => request(intent="reqalts")<0.9>;

export reqalts1a7 = <疑问前缀>?(<关于>|<描述>)?(<其他槽位>:tag)的?<还有其他相似>(吗|没|么)?$ => request(intent="reqalts")<0.9>;
export reqalts1a8 = <疑问前缀>?<与>(<其他槽位>:tag)<关于>的?<还有其他相似>(吗|没|么)?$ => request(intent="reqalts")<0.9>;
export reqalts1a9 = <疑问前缀>?<属于>(<题材>|<流派>|<朝代>|<经典>):tag的?<还有其他相似>(吗|没|么)?$ => request(intent="reqalts")<0.9>;


#1b.换首其他关于<其他槽位>的
export reqalts1b1 = <祈使前缀>?(<播放>|<换>)上?<另一首><其他相似>?(<关于>|<描述>)?(<其他槽位>:tag)的?<其他相似>?(吗|没|么)?$ => request(intent="reqalts")<0.9>;
export reqalts1b2 = <祈使前缀>?(<播放>|<换>)上?<另一首><其他相似>?<与>(<其他槽位>:tag)<关于>的?<其他相似>?(吗|没|么)?$ => request(intent="reqalts")<0.9>;
export reqalts1b3 = <祈使前缀>?(<播放>|<换>)上?<另一首><其他相似>?<属于>(<题材>|<流派>|<朝代>|<经典>):tag的?<其他相似>?(吗|没|么)?$ => request(intent="reqalts")<0.9>;

export reqalts1b4 = <其他相似>?(<关于>|<描述>)?(<其他槽位>:tag)的?<其他相似>?<祈使前缀>?(<播放>|<换>)上?<另一首>(吗|没|么)?$ => request(intent="reqalts")<0.9>;
export reqalts1b5 = <其他相似>?<与>(<其他槽位>:tag)<关于>的?<其他相似>?<祈使前缀>?(<播放>|<换>)上?<另一首>(吗|没|么)?$ => request(intent="reqalts")<0.9>;
export reqalts1b6 = <其他相似>?<属于>(<题材>|<流派>|<朝代>|<经典>):tag的?<其他相似>?<祈使前缀>?(<播放>|<换>)上?<另一首>(吗|没|么)?$ => request(intent="reqalts")<0.9>;


#1c.有没有其他人写的关于<其他槽位>的？
export reqalts1c1 = <疑问前缀>?<还有其他><人写的>(<关于>|<描述>)?(<其他槽位>:tag)的?<诗>?(吗|没|么)?$ => request(intent="reqalts", author="DENY")<0.9>;
export reqalts1c2 = <疑问前缀>?<还有其他><人写的><与>(<其他槽位>:tag)<关于>的?<诗>?(吗|没|么)?$ => request(intent="reqalts", author="DENY")<0.9>;
export reqalts1a3 = <疑问前缀>?<还有其他><人写的><属于>(<题材>|<流派>|<朝代>|<经典>):tag的?<诗>?(吗|没|么)?$ => request(intent="reqalts", author="DENY")<0.9>;

export reqalts1c4 = <疑问前缀>?<其他><人写的>(<关于>|<描述>)?(<其他槽位>:tag)的?<诗>?还?有(没有)?(吗|没|么)?$ => request(intent="reqalts", author="DENY")<0.9>;
export reqalts1c5 = <疑问前缀>?<其他><人写的><与>(<其他槽位>:tag)<关于>的?<诗>?还?有(没有)?(吗|没|么)?$ => request(intent="reqalts", author="DENY")<0.9>;
export reqalts1a6 = <疑问前缀>?<其他><人写的><属于>(<题材>|<流派>|<朝代>|<经典>):tag的?<诗>?还?有(没有)?(吗|没|么)?$ => request(intent="reqalts", author="DENY")<0.9>;

export reqalts1c7 = <疑问前缀>?(<关于>|<描述>)?(<其他槽位>:tag)的?<还有其他><人写的><诗>?(吗|没|么)?$ => request(intent="reqalts", author="DENY")<0.9>;
export reqalts1c8 = <疑问前缀>?<与>(<其他槽位>:tag)<关于>的?<还有其他><人写的><诗>?(吗|没|么)?$ => request(intent="reqalts", author="DENY")<0.9>;
export reqalts1a9 = <疑问前缀>?<属于>(<题材>|<流派>|<朝代>|<经典>):tag的?<还有其他><人写的><诗>?(吗|没|么)?$ => request(intent="reqalts", author="DENY")<0.9>;

export reqalts1c10 = <疑问前缀>?<其他><诗人>还?有<没有>?<创作>(<关于>|<描述>)?(<其他槽位>:tag)的?<诗>?(吗|没|么)?$ => request(intent="reqalts", author="DENY")<0.9>;
export reqalts1c11 = <疑问前缀>?<其他><诗人>还?有<没有>?<创作><与>(<其他槽位>:tag)<关于>的?<诗>?(吗|没|么)?$ => request(intent="reqalts", author="DENY")<0.9>;
export reqalts1a12 = <疑问前缀>?<其他><诗人>还?有<没有>?<创作><属于>(<题材>|<流派>|<朝代>|<经典>):tag的?<诗>?(吗|没|么)?$ => request(intent="reqalts", author="DENY")<0.9>;


#1d.来一首其他人写的关于<其他槽位>的
export reqalts1d1 = <祈使前缀>?(<播放>|<换>)上?<另一首><其他><人写的><相似>?(<关于>|<描述>)?(<其他槽位>:tag)的?<诗>?<其他相似>?(吗|没|么)?$ => request(intent="reqalts",author="DENY")<0.9>;
export reqalts1d2 = <祈使前缀>?(<播放>|<换>)上?<另一首><其他><人写的><相似>?<与>(<其他槽位>:tag)<关于>的?<其他相似>?<诗>?(吗|没|么)?$ => request(intent="reqalts",author="DENY")<0.9>;
export reqalts1d3 = <祈使前缀>?(<播放>|<换>)上?<另一首><其他><人写的><相似>?<属于>(<题材>|<流派>|<朝代>|<经典>):tag的?<其他相似>?<诗>?(吗|没|么)?$ => request(intent="reqalts",author="DENY")<0.9>;

export reqalts1d4 = <其他><人写的>(<关于>|<描述>)?(<其他槽位>:tag)的?<其他相似>?<诗>?<祈使前缀>?(<播放>|<换>)上?<另一首>(吗|没|么)?$ => request(intent="reqalts",author="DENY")<0.9>;
export reqalts1d5 = <其他><人写的><与>(<其他槽位>:tag)<关于>的?<其他相似>?<诗>?<祈使前缀>?(<播放>|<换>)上?<另一首>(吗|没|么)?$ => request(intent="reqalts",author="DENY")<0.9>;
export reqalts1d6 = <其他><人写的><属于>(<题材>|<流派>|<朝代>|<经典>):tag的?<其他相似>?<诗>?<祈使前缀>?(<播放>|<换>)上?<另一首>(吗|没|么)?$ => request(intent="reqalts",author="DENY")<0.9>;


#1e.他有没有写过别的关于<其他槽位>的<诗>
export reqalts1e1 = <疑问前缀>?(<他她>:r_person)还?((有<没有>?)|(有<没有>?)<创作>|<创作>)<其他相似>(<关于>|<描述>)?(<其他槽位>:tag)的?(<诗>)?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts1e2 = <疑问前缀>?(<他她>:r_person)还?((有<没有>?)|(有<没有>?)<创作>|<创作>)<其他相似><与>(<其他槽位>:tag)<关于>的?(<诗>)?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts1e3 = <疑问前缀>?(<他她>:r_person)还?((有<没有>?)|(有<没有>?)<创作>|<创作>)<其他相似><属于>(<题材>|<流派>|<朝代>|<经典>):tag的?(<诗>)?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;

export reqalts1e4 = <疑问前缀>?还?有<没有>?(<他她>:r_person)(<创作>|的)<其他相似><相似>?(<关于>|<描述>)?(<其他槽位>:tag)的?(<诗>)?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts1e4 = <疑问前缀>?还?有<没有>?<其他相似>(<他她>:r_person)(<创作>|的)<相似>?(<关于>|<描述>)?(<其他槽位>:tag)的?(<诗>)?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts1e5 = <疑问前缀>?还?有<没有>?(<他她>:r_person)(<创作>|的)<其他相似><相似>?<与>(<其他槽位>:tag)<关于>的?(<诗>)?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts1e5 = <疑问前缀>?还?有<没有>?<其他相似>(<他她>:r_person)(<创作>|的)<相似>?<与>(<其他槽位>:tag)<关于>的?(<诗>)?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts1e6 = <疑问前缀>?还?有<没有>?(<他她>:r_person)(<创作>|的)<其他相似><相似>?<属于>(<题材>|<流派>|<朝代>|<经典>):tag的?(<诗>)?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts1e6 = <疑问前缀>?还?有<没有>?<其他相似>(<他她>:r_person)(<创作>|的)<相似>?<属于>(<题材>|<流派>|<朝代>|<经典>):tag的?(<诗>)?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;

export reqalts1e7 = <疑问前缀>?(<他她>:r_person)(<创作>|的)<其他><相似>?(<关于>|<描述>)?(<其他槽位>:tag)的?(<诗>)?还?有<没有>?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts1e7 = <疑问前缀>?<其他>(<他她>:r_person)(<创作>|的)<相似>?(<关于>|<描述>)?(<其他槽位>:tag)的?(<诗>)?还?有<没有>?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts1e8 = <疑问前缀>?(<他她>:r_person)(<创作>|的)<其他><相似>?<与>(<其他槽位>:tag)<关于>的?(<诗>)?还?有<没有>?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts1e8 = <疑问前缀>?<其他>(<他她>:r_person)(<创作>|的)<相似>?<与>(<其他槽位>:tag)<关于>的?(<诗>)?还?有<没有>?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts1e9 = <疑问前缀>?(<他她>:r_person)(<创作>|的)<其他><相似>?<属于>(<题材>|<流派>|<朝代>|<经典>):tag的?(<诗>)?还?有<没有>?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts1e9 = <疑问前缀>?<其他>(<他她>:r_person)(<创作>|的)<相似>?<属于>(<题材>|<流派>|<朝代>|<经典>):tag的?(<诗>)?还?有<没有>?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;


#1f.来首他写的其他的关于<其他槽位>的<诗>
export reqalts1f1 = <祈使前缀>?(<播放>|<换>)上?<另一首>(<他她>:r_person)(<创作>|的)<其他相似><相似>?(<关于>|<描述>)?(<其他槽位>:tag)的?(<诗>)?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts1f1 = <祈使前缀>?(<播放>|<换>)上?<另一首><其他相似>(<他她>:r_person)(<创作>|的)<相似>?(<关于>|<描述>)?(<其他槽位>:tag)的?(<诗>)?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts1f2 = <祈使前缀>?(<播放>|<换>)上?<另一首>(<他她>:r_person)(<创作>|的)<其他相似><相似>?<与>(<其他槽位>:tag)<关于>的?(<诗>)?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts1f2 = <祈使前缀>?(<播放>|<换>)上?<另一首><其他相似>(<他她>:r_person)(<创作>|的)<相似>?<与>(<其他槽位>:tag)<关于>的?(<诗>)?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts1f3 = <祈使前缀>?(<播放>|<换>)上?<另一首>(<他她>:r_person)(<创作>|的)<其他相似><相似>?<属于>(<题材>|<流派>|<朝代>|<经典>):tag的?(<诗>)?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts1f3 = <祈使前缀>?(<播放>|<换>)上?<另一首><其他相似>(<他她>:r_person)(<创作>|的)<相似>?<属于>(<题材>|<流派>|<朝代>|<经典>):tag的?(<诗>)?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;

export reqalts1f4 = (<他她>:r_person)(<创作>|的)<其他相似><相似>?(<关于>|<描述>)?(<其他槽位>:tag)的?(<诗>)?<祈使前缀>?(<播放>|<换>)上?<另一首>(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts1f4 = <其他相似>(<他她>:r_person)(<创作>|的)<相似>?(<关于>|<描述>)?(<其他槽位>:tag)的?(<诗>)?<祈使前缀>?(<播放>|<换>)上?<另一首>(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts1f5 = (<他她>:r_person)(<创作>|的)<其他相似><相似>?<与>(<其他槽位>:tag)<关于>的?(<诗>)?<祈使前缀>?(<播放>|<换>)上?<另一首>(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts1f5 = <其他相似>(<他她>:r_person)(<创作>|的)<相似>?<与>(<其他槽位>:tag)<关于>的?(<诗>)?<祈使前缀>?(<播放>|<换>)上?<另一首>(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts1f6 = (<他她>:r_person)(<创作>|的)<其他相似><相似>?<属于>(<题材>|<流派>|<朝代>|<经典>):tag的?(<诗>)?<祈使前缀>?(<播放>|<换>)上?<另一首>(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts1f6 = <其他相似>(<他她>:r_person)(<创作>|的)<相似>?<属于>(<题材>|<流派>|<朝代>|<经典>):tag的?(<诗>)?<祈使前缀>?(<播放>|<换>)上?<另一首>(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;




#2a.有没有适合<人群>的?
export reqalts2a1 = <疑问前缀>?<有其他相似>(<人群>:tag)<可以>?<听学>的?(吗|没|么)?$ => request(intent="reqalts")<0.9>;
export reqalts2a2 = <疑问前缀>?<有其他相似><适合>(<人群>:tag)<听学>?的?(吗|没|么)?$ => request(intent="reqalts")<0.9>;

export reqalts2a3 = <疑问前缀>?<其他相似>(<人群>:tag)<可以>?<听学>的?还?有(没有)?(吗|没|么)?$ => request(intent="reqalts")<0.9>;
export reqalts2a4 = <疑问前缀>?<其他相似><适合>(<人群>:tag)<听学>?的?还?有(没有)?(吗|没|么)?$ => request(intent="reqalts")<0.9>;

export reqalts2a5 = <疑问前缀>?<适合>(<人群>:tag)<听学>?的?<有其他相似>(吗|没|么)?$ => request(intent="reqalts")<0.9>;
export reqalts2a6 = <疑问前缀>?(<人群>:tag)<可以>?<听学>的?<有其他相似>(吗|没|么)?$ => request(intent="reqalts")<0.9>;


#2b.换首适合<人群>的
export reqalts2b1 = <祈使前缀>?(<播放>|<换>)上?<另一首><其他相似>?(<人群>:tag)<可以>?<听学>的?<其他相似>?(吗|没|么)?$ => request(intent="reqalts")<0.9>;
export reqalts2b2 = <祈使前缀>?(<播放>|<换>)上?<另一首><其他相似>?<适合>(<人群>:tag)<听学>?的?<其他相似>?(吗|没|么)?$ => request(intent="reqalts")<0.9>;

export reqalts2b3 = <其他相似>?(<人群>:tag)<可以>?<听学>的?<其他相似>?<祈使前缀>?(<播放>|<换>)上?<另一首>(吗|没|么)?$ => request(intent="reqalts")<0.9>;
export reqalts2b4 = <其他相似>?<适合>(<人群>:tag)<听学>?的?<其他相似>?<祈使前缀>?(<播放>|<换>)上?<另一首>(吗|没|么)?$ => request(intent="reqalts")<0.9>;


#2c.还有没有其他人写的适合<人群>的?
export reqalts2c1 = <疑问前缀>?<还有其他><人写的><相似>?(<人群>:tag)<可以>?<听学>的?<诗>?(吗|没|么)?$ => request(intent="reqalts", author="DENY")<0.9>;
export reqalts2c2 = <疑问前缀>?<还有其他><人写的><相似>?<适合>(<人群>:tag)<听学>?的?<诗>?(吗|没|么)?$ => request(intent="reqalts", author="DENY")<0.9>;

export reqalts2c3 = <疑问前缀>?<其他><人写的><相似>?(<人群>:tag)<可以>?<听学>的?<诗>?还?有(没有)?(吗|没|么)?$ => request(intent="reqalts", author="DENY")<0.9>;
export reqalts2c4 = <疑问前缀>?<其他><人写的><相似>?<适合>(<人群>:tag)<听学>?的?<诗>?还?有(没有)?(吗|没|么)?$ => request(intent="reqalts", author="DENY")<0.9>;

export reqalts2c5 = <疑问前缀>?<相似>?(<人群>:tag)<可以>?<听学>的?<还有其他><人写的><诗>?(吗|没|么)?$ => request(intent="reqalts", author="DENY")<0.9>;
export reqalts2c6 = <疑问前缀>?<相似>?<适合>(<人群>:tag)<听学>?的?<还有其他><人写的><诗>?(吗|没|么)?$ => request(intent="reqalts", author="DENY")<0.9>;

export reqalts2c7 = <疑问前缀>?<其他><诗人>还?有<没有>?<创作><相似>?(<人群>:tag)<可以>?<听学>的?<诗>?(吗|没|么)?$ => request(intent="reqalts", author="DENY")<0.9>;
export reqalts2c8 = <疑问前缀>?<其他><诗人>还?有<没有>?<创作><相似>?<适合>(<人群>:tag)<听学>?的?<诗>?(吗|没|么)?$ => request(intent="reqalts", author="DENY")<0.9>;


#2d.换首其他人写的适合<人群>的
export reqalts2d1 = <祈使前缀>?(<播放>|<换>)上?<另一首><其他><人写的><相似>?(<人群>:tag)<可以>?<听学>的?<相似>?<诗>?<其他相似>?(吗|没|么)?$ => request(intent="reqalts",author="DENY")<0.9>;
export reqalts2d2 = <祈使前缀>?(<播放>|<换>)上?<另一首><其他><人写的><相似>?<适合>(<人群>:tag)<听学>?的?<相似>?<诗>?(吗|没|么)?$ => request(intent="reqalts",author="DENY")<0.9>;

export reqalts2d4 = <其他><人写的><相似>?(<人群>:tag)<可以>?<听学>的?<相似>?<诗>?<祈使前缀>?(<播放>|<换>)上?<另一首>(吗|没|么)?$ => request(intent="reqalts",author="DENY")<0.9>;
export reqalts2d5 = <其他><人写的><相似>?<适合>(<人群>:tag)<听学>?的?<相似>?<诗>?<祈使前缀>?(<播放>|<换>)上?<另一首>(吗|没|么)?$ => request(intent="reqalts",author="DENY")<0.9>;


#2e.他还有没有写过其他的适合<人群>的？
export reqalts2e1 = <疑问前缀>?(<他她>:r_person)还?((有<没有>?)|(有<没有>?)<创作>|<创作>)<其他相似>(<人群>:tag)<可以>?<听学>的?(<诗>)?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts2e2 = <疑问前缀>?(<他她>:r_person)还?((有<没有>?)|(有<没有>?)<创作>|<创作>)<其他相似><适合>(<人群>:tag)<听学>?的?(<诗>)?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;

export reqalts2e3 = <疑问前缀>?还?有<没有>?(<他她>:r_person)(<创作>|的)<其他相似><相似>?(<人群>:tag)<可以>?<听学>的?(<诗>)?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts2e3 = <疑问前缀>?还?有<没有>?<其他相似>(<他她>:r_person)(<创作>|的)<相似>?(<人群>:tag)<可以>?<听学>的?(<诗>)?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts2e4 = <疑问前缀>?还?有<没有>?(<他她>:r_person)(<创作>|的)<其他相似><相似>?<适合>(<人群>:tag)<听学>?的?(<诗>)?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts2e4 = <疑问前缀>?还?有<没有>?<其他相似>(<他她>:r_person)(<创作>|的)<相似>?<适合>(<人群>:tag)<听学>?的?(<诗>)?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;

export reqalts2e5 = <疑问前缀>?(<他她>:r_person)(<创作>|的)<其他><相似>?(<人群>:tag)<可以>?<听学>的?(<诗>)?还?有<没有>?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts2e5 = <疑问前缀>?<其他>(<他她>:r_person)(<创作>|的)<相似>?(<人群>:tag)<可以>?<听学>的?(<诗>)?还?有<没有>?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts2e6 = <疑问前缀>?(<他她>:r_person)(<创作>|的)<其他><相似>?<适合>(<人群>:tag)<听学>?的?(<诗>)?还?有<没有>?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts2e6 = <疑问前缀>?<其他>(<他她>:r_person)(<创作>|的)<相似>?<适合>(<人群>:tag)<听学>?的?(<诗>)?还?有<没有>?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;


#2f.来首他写过的其他的适合<人群>听的
export reqalts2f1 = <祈使前缀>?(<播放>|<换>)上?<另一首>(<他她>:r_person)(<创作>|的)<其他相似><相似>?(<人群>:tag)<可以>?<听学>的?(<诗>)?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts2f1 = <祈使前缀>?(<播放>|<换>)上?<另一首><其他相似>(<他她>:r_person)(<创作>|的)<相似>?(<人群>:tag)<可以>?<听学>的?(<诗>)?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts2f2 = <祈使前缀>?(<播放>|<换>)上?<另一首>(<他她>:r_person)(<创作>|的)<其他相似><相似>?<适合>(<人群>:tag)<听学>?的?(<诗>)?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts2f2 = <祈使前缀>?(<播放>|<换>)上?<另一首><其他相似>(<他她>:r_person)(<创作>|的)<相似>?<适合>(<人群>:tag)<听学>?的?(<诗>)?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;

export reqalts2f3 = (<他她>:r_person)(<创作>|的)<其他相似><相似>?(<人群>:tag)<可以>?<听学>的?(<诗>)?<祈使前缀>?(<播放>|<换>)上?<另一首>(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts2f3 = <其他相似>(<他她>:r_person)(<创作>|的)<相似>?(<人群>:tag)<可以>?<听学>的?(<诗>)?<祈使前缀>?(<播放>|<换>)上?<另一首>(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts2f4 = (<他她>:r_person)(<创作>|的)<其他相似><相似>?<适合>(<人群>:tag)<听学>?的?(<诗>)?<祈使前缀>?(<播放>|<换>)上?<另一首>(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts2f4 = <其他相似>(<他她>:r_person)(<创作>|的)<相似>?<适合>(<人群>:tag)<听学>?的?(<诗>)?<祈使前缀>?(<播放>|<换>)上?<另一首>(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;




#3a.有没有其他<title>吗
export reqalts3a1 = <疑问前缀>?<还有其他><版本>?的?(&<poem_title>:title)(吗|没|么)?$ => request(intent="reqalts")<0.9>;
export reqalts3a2 = <疑问前缀>?<其他><版本>?的?(&<poem_title>:title)还?有(没有)?(吗|没|么)?$ => request(intent="reqalts")<0.9>;
export reqalts3a3 = <疑问前缀>?(&<poem_title>:title)<还有其他><版本>?的?(吗|没|么)?$ => request(intent="reqalts")<0.9>;


#3b.换首其他的<title>
export reqalts3b1 = <祈使前缀>?(<播放>|<换>)上?<另一首><其他><版本>?的?(&<poem_title>:title)(吗|没|么)?$ => request(intent="reqalts")<0.9>;
export reqalts3b2 = <其他><版本>?的?(&<poem_title>:title)<祈使前缀>?(<播放>|<换>)上?<另一首>(吗|没|么)?$ => request(intent="reqalts")<0.9>;


#3c.还有没有其他人写的<title>
export reqalts3c1 = <疑问前缀>?<还有其他><人写的>(&<poem_title>:title)(吗|没|么)?$ => request(intent="reqalts", author="DENY")<0.9>;
export reqalts3c2 = <疑问前缀>?<其他><人写的>(&<poem_title>:title)还?有(没有)?(吗|没|么)?$ => request(intent="reqalts", author="DENY")<0.9>;

#3d.换一首其他人的<title>
export reqalts3d1 = <祈使前缀>?(<播放>|<换>)上?<另一首><其他><人写的>(&<poem_title>:title)(吗|没|么)?$ => request(intent="reqalts", author="DENY")<0.9>;
export reqalts3d2 = <其他><人写的>(&<poem_title>:title)<祈使前缀>?(<播放>|<换>)上?<另一首>(吗|没|么)?$ => request(intent="reqalts", author="DENY")<0.9>;

#3e.还有没有他写的其他版本的<title>吗？
export reqalts3e1 = <疑问前缀>?(<他她>:r_person)(<创作>|的)(&<poem_title>:title)<还有其他><版本>?的?(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;
export reqalts3e2 = <疑问前缀>?(<他她>:r_person)还?(有<没有>?)?<创作>?<其他><版本>?的?(&<poem_title>:title)(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;

#3f.换一首他写的其他版本的<title>
export reqalts3f1 = <祈使前缀>?(<播放>|<换>)上?<另一首>(<他她>:r_person)(<创作>|的)<其他><版本>?的?(&<poem_title>:title)(吗|没|么)?$ => request(intent="reqalts", author="OLD_VAL")<0.9>;




#4a.有<author>写的吗
export reqalts4a1 = <疑问前缀>?还?有<没有>?(&<poem_author>:author)<创作>?的?(吗|没|么)?$ => request(intent="reqalts")<0.9>;
export reqalts4a2 = <疑问前缀>?(&<poem_author>:author)<创作>?的?还?有<没有>?(吗|没|么)?$ => request(intent="reqalts")<0.9>;

#4b.换首<author>的
export reqalts4b1 = <祈使前缀>?(<播放>|<换>)上?<另一首>(&<poem_author>:author)<创作>?的?(吗|没|么)?$ => request(intent="reqalts")<0.9>;
export reqalts4b2 = (&<poem_author>:author)<创作>?的?<祈使前缀>?(<播放>|<换>)上?<另一首>(吗|没|么)?$ => request(intent="reqalts")<0.9>;
