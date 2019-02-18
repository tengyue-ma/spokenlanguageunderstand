################################
##                            ##
##      author:lengting       ##
##      time:2018-06-19       ##
##      modify:2018-11-20     ##
##                            ##
################################


第一人称	= 我|本小姐|本姑娘|朕;
播放	= 播|放|播放|唱|唱唱|唱一唱|听|听听|听一听|点播|欣赏|再?来|来听听|<第一人称>?想要?听听?;
一首	= 首|一首|几首|曲|一曲|几曲|段|一段|几段|个|一个|些|一些|下|一下|点|一点;
唱	= 唱|演唱|演奏|弹奏;
有	= 有|有没有|有木有|有么有|是否有;
这首	= (这|那)(首|支|个);
歌	= 歌|歌曲|曲子|音乐|乐|乐曲;
这首歌	= <这首><歌>;

其他	= (别|其他|其它|其余|另外|另一个)的?;
歌手	= 人|歌手|艺人|明星|歌星|艺术家|音乐家|大叔|欧巴|小哥哥|帅哥|帅哥哥|小鲜肉|美女|小姐姐|姑娘|萝莉;
个	= 个|名;
张	= 一?(张|个);

第三人称	= (他|她|它)们?;
性别	= 男|女;
性别修饰后缀	= 的|生|性;
性别搭配后缀	= 歌手|艺人|明星|音乐家;
性别合成词搭配	= 声|人;
称呼	= 大叔|欧巴|小哥哥|帅哥|帅哥哥|美女|小姐姐|姑娘;
组合	= 乐队|团|团体|组合;

句末语气词	= 吧|吗|嘛|么|咯|呢|不|啊|呀|吖;
可否		= 好吗|好不|好不好|行吗|行不|行不行|可以吗|可以么|可以嘛|可不可以|可以不|如何|怎么样;
结尾请求	= <可否>|<句末语气词>;

前缀程度修饰	= 稍微|比较|非常|特别|十分|很;
风格	= 古风|中国风|经典|怀旧|流行|热|火|热门|摇滚|说唱|民谣|古典|乡村|校园民谣|古典民谣|乡村民谣|小众|民族|小清新|清新|浪漫|性感|舞曲|嘻哈|民俗|英伦|金属|重金属|朋克|蓝调|雷鬼;
一点	= 一?(点|些);
版本	= 版|版本;

修饰	= (好听|悦耳)(的|得|地)?;
新	= 新|老;
都	= 都|全|全都;
一遍	= 一?(遍|次|下);
还	= 你?还;
种	= 个|种;



############# reqalts artist:
## 放别人的 | 放别人的这首歌[title unchanged] | 放别人的<title>[title assigned]
## 换个歌手 | 换个歌手唱的 | 换个歌手唱的这首歌[title unchanged] | 换个歌手唱的<title>[title assigned] | 这首歌换个歌手唱[title unchanged]
export reqalts00 = (<播放><一首>?|<还>?<有>)<其他><歌手><唱>?的<歌>?<结尾请求>?$ => request(intent="reqalts", reqalts_slot="artist") <0.996>;
export reqalts01 = (<播放><一首>?|<还>?<有>)<其他><歌手><唱>?过?的?(<这首歌>|版本)<结尾请求>?$ => request(intent="reqalts", reqalts_slot="artist", title="unchanged") <0.996>;
export reqalts02 = (<播放><一首>?|<还>?<有>)<其他><歌手><唱>?过?的?(&<music_title>:title)<结尾请求>?$ => request(intent="reqalts", reqalts_slot="artist") <0.996>;
export reqalts03 = <其他><歌手><唱>?的<歌>?(<播放><一首><结尾请求>?|<还>?<有><句末语气词>?)?$ => request(intent="reqalts", reqalts_slot="artist") <0.996>;
export reqalts04 = <其他><歌手><唱>?过?的(<这首歌>|版本)(<播放><一首><结尾请求>?|<还>?<有><句末语气词>?)?$ => request(intent="reqalts", reqalts_slot="artist", title="unchanged") <0.996>;
export reqalts05 = <其他><歌手><唱>?过?的(&<music_title>:title)(<播放><一首><结尾请求>?|<还>?<有><句末语气词>?)?$ => request(intent="reqalts", reqalts_slot="artist") <0.996>;
export reqalts06 = 换(一?个<其他>?|成?<其他>)(<歌手><唱>?|<版本>)的?<歌>?<结尾请求>?$ => request(intent="reqalts", reqalts_slot="artist") <0.96>;
export reqalts07 = 换(一?个<其他>?|成?<其他>)(<歌手><唱>?过?|<版本>)的?(<这首歌>|版本)<结尾请求>?$ => request(intent="reqalts", reqalts_slot="artist", title="unchanged") <0.996>;
export reqalts08 = 换(一?个<其他>?|成?<其他>)(<歌手><唱>?过?|<版本>)的?(&<music_title>:title)<结尾请求>?$ => request(intent="reqalts", reqalts_slot="artist") <0.996>;
export reqalts09 = ^(<播放><一首>?|<还>?<有>)?<这首歌>(<播放><一首>?|<有>)?<其他>(<歌手><唱>?过?|<版本>)的?<句末语气词>?$ => request(intent="reqalts", reqalts_slot="artist", title="unchanged") <0.998>;
export reqalts010 = <这首歌>换(一?个<其他>?|成?<其他>)(<歌手><唱>?过?|<版本>)的?<结尾请求>?$ => request(intent="reqalts", reqalts_slot="artist", title="unchanged") <0.998>;
export reqalts011 = (&<music_title>:title)(<播放><一首>?|<有>)<其他>(<歌手><唱>?过?|<版本>)的<句末语气词>?$ => request(intent="reqalts", reqalts_slot="artist") <0.996>;
export reqalts012 = (&<music_title>:title)换(一?个<其他>?|成?<其他>)(<歌手><唱>?过?|<版本>)的?<结尾请求>?$ => request(intent="reqalts", reqalts_slot="artist") <0.996>;

############# reqalts title:
## 换一首歌 | 还有别的吗 | 还有别的歌吗 | 有没有其他的歌
## 他的别的歌 | 播放一首他的别的歌 | 他的别的歌来一首
## 这个歌手的其他歌 | 这个艺人唱的别的歌来一首
## <artist>的别的歌 | 播放一首<artist>的别的歌 | <artist>的别的歌来一首
export reqalts10 = (<播放><一首>?|<还>?<有>|换<一首>?)<其他>(<歌>|儿歌:tag)?<播放>?<一首>?<结尾请求>?$ => request(intent="reqalts", reqalts_slot="title") <0.96>;
export reqalts10 = <其他>(<歌>|儿歌:tag)(<播放><一首>?|<有>)<结尾请求>?$ => request(intent="reqalts", reqalts_slot="title") <0.96>;
export reqalts11 = 换(<一首>?<其他>?|成<其他>)(<修饰>|<新>:rank)?的?<歌><结尾请求>?$ => request(intent="reqalts", reqalts_slot="title") <0.996>;
export reqalts12 = (<第三人称>|这<个><歌手>)(<唱>?过?的?|还?<有>?)<其他>(<修饰>|<新>:rank)?<歌> => request(intent="reqalts", reqalts_slot="title", artist="unchanged") <0.996>;
export reqalts13 = (换|再<播放>)<一首>(<第三人称>|这<个><歌手>)<唱>?过?的<歌><结尾请求>?$ => request(intent="reqalts", reqalts_slot="title", artist="unchanged") <0.996>;
export reqalts14 = (<第三人称>|这<个><歌手>)<唱>?过?的<歌>(换|再<播放>)<一首><结尾请求>?$ => request(intent="reqalts", reqalts_slot="title", artist="unchanged") <0.996>;
export reqalts15 = (&<music_artist>:artist)<唱>?过?的?<其他><歌> => request(intent="reqalts", reqalts_slot="title") <0.996>;
#export reqalts16 = 换<一首>(&<music_artist>:artist)<唱>?的<歌><结尾请求>?$ => request(intent="reqalts", reqalts_slot="title") <0.96>;
export reqalts17 = (&<music_artist>:artist)<唱>?过?的<歌>?换<一首><结尾请求>?$ => request(intent="reqalts", reqalts_slot="title") <0.996>;

############# reqalts album:
## 换张专辑 | 其他专辑 | 有没有别的专辑
## 他的别的专辑里的歌
## 这个歌手的其他专辑
## <artist>的别的专辑
export reqalts20 = ^(<播放><一首>?|<还>?<有>)?<其他>专辑里?面?中?的?<歌>?(<播放><一首>?|<还>?<有>)?<结尾请求>?$ => request(intent="reqalts", reqalts_slot="album") <0.996>;
export reqalts21 = 换(<张>|<一首>)?<其他>?专辑里?面?中?的?<歌>?<播放>?<结尾请求>?$ => request(intent="reqalts", reqalts_slot="album") <0.996>;
export reqalts22 = ^专辑换<张><结尾请求>?$ => request(intent="reqalts", reqalts_slot="album") <0.996>;
export reqalts23 = ^(<播放><一首>?|<还>?<有>)?(<第三人称>|这<个><歌手>)<唱>?的?<其他>专辑里?面?中?的?<歌>?(<播放><一首>?|<还>?<有>)?<结尾请求>?$ => request(intent="reqalts", reqalts_slot="album", artist="unchanged") <0.996>;
export reqalts24 = 换(<张>|<一首>)?(<第三人称>|这<个><歌手>)<唱>?的?<其他>?专辑里?面?中?的?<歌>?<结尾请求>?$ => request(intent="reqalts", reqalts_slot="album", artist="unchanged") <0.996>;
export reqalts25 = (<第三人称>|这<个><歌手>)<唱>?的?<其他>?专辑换<张><结尾请求>?$ => request(intent="reqalts", reqalts_slot="album", artist="unchanged") <0.996>;
export reqalts26 = (&<music_artist>:artist)<唱>?的?<其他>专辑里?面?中?的?<歌>?(<播放><一首>?|<还>?<有>)?<结尾请求>?$ => request(intent="reqalts", reqalts_slot="album") <0.996>;
export reqalts27 = 换<张>?(&<music_artist>:artist)<唱>?的?<其他>专辑里?面?中?的?<歌>?<结尾请求>?$ => request(intent="reqalts", reqalts_slot="album") <0.996>;
export reqalts28 = (&<music_artist>:artist)<唱>?的?<其他>?专辑换<张><结尾请求>?$ => request(intent="reqalts", reqalts_slot="album") <0.996>;
## album不变, reqalts title
export reqalts29 = (这|那|同一|本)<张>?专辑里?面?中?的?还?<有>?<其他><歌>(<播放><一首>?|<还>?<有>|<都><播放><一遍>)?<结尾请求>?$ => request(intent="reqalts", album="unchanged", reqalts_slot="title") <0.996>; 
## 其他专辑的这首歌
export reqalts210 = ^(<播放><一首>?|<还>?<有>)?<其他>专辑里?面?中?的?<这首歌>(<播放><一首>?|<还>?<有>)?<结尾请求>?$ => request(intent="reqalts", reqalts_slot="album", title="unchanged") <0.996>;

############# reqalts language:
## 换个语言的歌
export reqalts30 = 换(一?<种>?<其他>?|成<其他>)(语言|语种)<唱>?的?<歌>? => request(intent="reqalts", reqalts_slot="language") <0.96>;
export reqalts31 = <其他>(语言|语种)<唱>?的?<歌>?(<播放><一首><结尾请求>?|<还>?<有>?<句末语气词>?)?$ => request(intent="reqalts", reqalts_slot="language") <0.96>;
## 换首其他<language>的歌 (language指定, 换的其实是title)
export reqalts32 = (<播放><一首>?|<有>|换<一首>?|换成?)<其他>(&<music_language>:language)的?<歌><结尾请求>?$ => request(intent="reqalts", reqalts_slot="title") <0.996>;

############# reqalts gender:
## 换个其他女歌手的 | 其他女歌手的歌来一首
export reqalts40 = 换(一?<个>?<其他>?|成?<其他>)(&<music_region>:region)?(<性别>:gender)(<性别修饰后缀>?<性别搭配后缀>|<性别合成词搭配>)<唱>?的<歌>?<结尾请求>?$ => request(intent="reqalts", reqalts_slot="artist") <0.96>;
export reqalts41 = <其他>(&<music_region>:region)?(<性别>:gender)(<性别修饰后缀>?<性别搭配后缀>|<性别合成词搭配>)<唱>?的<歌>?(<播放><一首><结尾请求>?|<还>?<有><句末语气词>?)?$ => request(intent="reqalts", reqalts_slot="artist") <0.96>;
export reqalts42 = 换(一?<个>?<其他>?|成?<其他>)(&<music_region>:region)?(<称呼>:gender)<唱>的<歌>?<结尾请求>?$ => request(intent="reqalts", reqalts_slot="artist") <0.96>;
export reqalts43 = <其他>(&<music_region>:region)?(<称呼>:gender)<唱>的<歌>?(<播放><一首><结尾请求>?|<还>?<有><句末语气词>?)?$ => request(intent="reqalts", reqalts_slot="artist") <0.96>;
export reqalts44 = 换(一?<个>?<其他>?|成?<其他>)(&<music_region>:region)?(<性别>:gender)<性别修饰后缀>?(团|团体|组合)<唱>?的<歌>?<结尾请求>?$ => request(intent="reqalts", reqalts_slot="artist", gender="$4") <0.966>;
export reqalts45 = <其他>(&<music_region>:region)?(<性别>:gender)<性别修饰后缀>?(团|团体|组合)<唱>?的<歌>?(<播放><一首><结尾请求>?|<还>?<有><句末语气词>?)?$ => request(intent="reqalts", reqalts_slot="artist", gender="$3") <0.966>;
export reqalts46 = 换(一?<个>?<其他>?|成?<其他>)(&<music_region>:region)?(乐队|团体|组合)<唱>?的<歌>?<结尾请求>?$ => request(intent="reqalts", reqalts_slot="artist", gender="$3") <0.96>;
export reqalts47 = <其他>(&<music_region>:region)?(乐队|团体|组合)<唱>?的<歌>?(<播放><一首><结尾请求>?|<还>?<有><句末语气词>?)?$ => request(intent="reqalts", reqalts_slot="artist", gender="$2") <0.96>;

############# reqalts tag:
## 换个风格的 | 换个其他风格的
export reqalts50 = 换(一?<种>?<其他>?|成<其他>)(风格|类型|曲风|曲调|曲式|旋律)的?<歌>? => request(intent="reqalts", reqalts_slot="tag") <0.96>;
export reqalts51 = <其他>(风格|类型|曲风|曲调|曲式|旋律)的<歌>?(<播放><一首><结尾请求>?|<还>?<有>?<句末语气词>?)?$ => request(intent="reqalts", reqalts_slot="tag") <0.96>;
export reqalts52 = (<第三人称>|这<个><歌手>)<唱>?的?<其他>(风格|类型|曲风|曲调|曲式|旋律)的<歌>?(<播放><一首><结尾请求>?|<还>?<有><句末语气词>?)?$ => request(intent="reqalts", artist="unchanged", reqalts_slot="tag") <0.98>;
## tag指定, 换的其实是title
export reqalts53 = (<播放><一首>?|<有>|换<一首>?|换成?)<其他><前缀程度修饰>?(<风格>:tag)(风|风格|<一点>)?的?<歌>?<结尾请求>?$ => request(intent="reqalts", reqalts_slot="title") <0.96>;
export reqalts54 = (<第三人称>|这<个><歌手>)<唱>?的?<其他><前缀程度修饰>?(<风格>:tag)(风|风格|<一点>)?的?<歌> => request(intent="reqalts", artist="unchanged", reqalts_slot="title") <0.96>;
export reqalts55 = ^(<播放><一首>?|<有>|换<一首>?|换成?)?(与|和)<这首歌>(风格|曲风|类型)(相似|相同|相仿|类似|差不多)的?<其他>?的?<歌>?(<播放><一首>?|<还>?<有>)?<结尾请求>?$ => request(intent="reqalts", tag="unchanged", reqalts_slot="title") <0.96>;
export reqalts56 = ^(<播放><一首>?|<有>|换<一首>?|换成?)?<其他>?(类似|相似|相仿|差不多)的?<其他>?的?<歌>(<播放><一首>?|<还>?<有>)?<结尾请求>?$ => request(intent="reqalts", tag="unchanged", reqalts_slot="title") <0.96>;

############# reqalts region:
export reqalts60 = <其他>(国家|地区)的<歌>?(<播放><一首>?|<还>?<有>)?<结尾请求>?$ => request(intent="reqalts", reqalts_slot="region") <0.96>;
export reqalts61 = (这|那)个(国家|地区)的?<其他><歌>(<播放><一首>?|<还>?<有>)?<结尾请求>?$ => request(intent="reqalts", region="unchanged", reqalts_slot="title") <0.96>;
export reqalts62 = (&<music_region>:region)的?<其他><歌>(<播放><一首>?|<还>?<有>)?<结尾请求>?$ => request(intent="reqalts", reqalts_slot="title") <0.96>;

############# reqalts program:
## 换<program>里面的其他歌
## 换个电影主题曲
export reqalts70 = (<播放><一首>?|<有>|换<一首>?|换成?)<其他>(电影|电视剧|综艺|动漫|动画片)的?(主题曲|片头曲|片尾曲|插曲) => request(intent="reqalts", reqalts_slot="program") <0.96>;
