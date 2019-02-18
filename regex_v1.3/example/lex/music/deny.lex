不要  = (不要|不想|不想要|不喜欢|拒绝|别.*(播放|放|唱)) (再|继续)?;
给    = 给|帮|为|替;
我    = 我|俺|大爷|爷|咱|咱们|老娘|本小姐|本大小姐|我们|本人|朕;
播放  = 播|播放|放|唱|听;
歌曲类别 = 京剧|评剧|豫剧|越剧|黄梅戏|轻音乐|儿歌;
歌    = 歌|歌曲|专辑|曲|音乐;
唱    = 唱|演唱|演奏|弹奏;
心情  = 怀旧|清新|浪漫|性感|伤感|治愈|放松|悲伤|温暖|孤独|感动|兴奋|快乐|安静|思念|欢快|爱情|忧伤;
类别  = 流行|摇滚|民谣|电子|说唱|爵士|乡村|古典|民族|英伦|金属|朋克|蓝调|原声|雷鬼|拉丁|另类|独立|嘻哈|古风;
乐器  = 钢琴|小提琴|吉他|长笛|萨克斯|单簧管|口琴|二胡|琵琶|唢呐|古筝;
标签  = <心情>|<类别>|<乐器>;
歌手性别 = (男|女)(性|生|歌手|歌星|艺人|音乐家|艺术家);

这那  = 这|那;
首    = 首|个|种;

###### 对应原来的music_deny0: <主观否定> <独立的歌曲类别> [<结尾礼貌>]
export deny00 = <不要>(<给><我>)?<播放>?(<歌曲类别>) => request(tag="$2", intent="deny") <0.7>;

###### 对应原来的music_deny1: <主观否定> <歌手名> <的歌> [<结尾礼貌>]
###### 的歌=(<唱的> [<歌曲>]) | ([的] <歌曲>) = [唱|演唱]的 (歌|歌曲|专辑|曲|音乐|<歌曲类别>)? | [的] (歌|歌曲|专辑|曲|音乐|<歌曲类别>);
export deny01 = <不要>(<给><我>)?<播放>?(&<music_artist>)<唱>的?(<歌曲类别>)? 	 => request(artist="$2", tag="$3", intent="deny") <0.8>;
export deny01 = <不要>(<给><我>)?<播放>?(&<music_artist>)的?(<歌曲类别>) 			 => request(artist="$2", tag="$3", intent="deny") <0.8>;
export deny01 = <不要>(<给><我>)?<播放>?(&<music_artist>)的?<歌> => request(artist="$2", intent="deny") <0.8>;

###### 对应原来的music_deny2: <主观否定> <歌曲名> [<这首歌>] [<结尾礼貌>]
###### 这首歌 = 这首|那首|这个|那个 <歌曲> = (这首|那首|这个|那个) (歌|歌曲|专辑|曲|音乐|<独立的歌曲类别>)
export deny02 = <不要>(<给><我>)?<播放>?(&<music_title>)<这那><首>(<歌曲类别>)? => request(title="$2", tag="$3", intent="deny") <0.9>;
export deny02 = <不要>(<给><我>)?<播放>?(&<music_title>) 							 	 => request(title="$2", intent="deny") <0.9>;

###### 对应原来的music_deny3: <主观否定> <歌手名> [<唱的>] <歌曲名> [<结尾礼貌>]
export deny03 = <不要>(<给><我>)?<播放>?(&<music_artist>)<唱>?的?(&<music_title>)	 	 => request(artist="$2", title="$3", intent="deny") <0.9>;

###### 对应原来的music_deny4: <主观否定> <其他槽位> <的歌> [<结尾礼貌>]
###### 其他槽位= <歌曲标签>|<歌曲地区>|<歌曲语言>|<歌手性别> = <标签>|&<music_region>|&<music_language>|(男|女)(性|生|歌手|艺人)
export deny04 = <不要>(<给><我>)?<播放>?(<标签>)<唱>的?(<歌曲类别>)?		 => request(tag="$2", tag1="$3", intent="deny") <0.8>;
export deny04 = <不要>(<给><我>)?<播放>?(<标签>)的?(<歌曲类别>)		 		 => request(tag="$2", tag1="$3", intent="deny") <0.8>;
export deny04 = <不要>(<给><我>)?<播放>?(<标签>)的?<歌> 	 => request(tag="$2", intent="deny") <0.8>;

export deny04 = <不要>(<给><我>)?<播放>?(&<music_region>)<唱>的?(<歌曲类别>)?		 => request(region="$2", tag="$3", intent="deny") <0.8>;
export deny04 = <不要>(<给><我>)?<播放>?(&<music_region>)的?(<歌曲类别>)			 => request(region="$2", tag="$3", intent="deny") <0.8>;
export deny04 = <不要>(<给><我>)?<播放>?(&<music_region>)的?<歌> => request(tag="$2", intent="deny") <0.8>;

export deny04 = <不要>(<给><我>)?<播放>?(&<music_language>)<唱>的?(<歌曲类别>)?	 => request(language="$2", tag="$3", intent="deny") <0.8>;
export deny04 = <不要>(<给><我>)?<播放>?(&<music_language>)的?(<歌曲类别>)		 => request(language="$2", tag="$3", intent="deny") <0.8>;
export deny04 = <不要>(<给><我>)?<播放>?(&<music_language>)的?<歌>=> request(language="$2", intent="deny") <0.8>;

export deny04 = <不要>(<给><我>)?<播放>?(<歌手性别>)<唱>的?(<歌曲类别>)?	 => request(gender="$2", tag="$3", intent="deny") <0.8>;
export deny04 = <不要>(<给><我>)?<播放>?(<歌手性别>)的?(<歌曲类别>)		 	 => request(gender="$2", tag="$3", intent="deny") <0.8>;
export deny04 = <不要>(<给><我>)?<播放>?(<歌手性别>)的?<歌>=> request(gender="$2", intent="deny") <0.8>;
