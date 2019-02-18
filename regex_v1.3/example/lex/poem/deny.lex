不要  = (不要|不想|不想要|不喜欢|拒绝|别|不听|不) (再|继续)?;
给    = 给|帮|为|替;
我    = 我|俺|大爷|爷|咱|咱们|老娘|本小姐|本大小姐|我们|本人|朕;
播放  = 播|播放|放|唱|听;
诗    = 古诗|诗|词|诗词;
诗词类别 = 唐诗|宋词|元曲|七言律诗|七言绝句|五言律诗|五言绝句;
写的  = (写|创作)?的?;
关于  = 关于|有关;
主题 = 月亮|思乡|思念家乡;



###### 对应原来的poem_deny0: <主观否定> <独立的诗歌类别> [<结尾礼貌>]
export deny00 = <不要>(<给><我>)?<播放>?(<诗词类别>) => request(tag="$2", intent="deny") <0.7>;

###### 对应原来的poem_deny1: <主观否定> <诗人> <的诗> [<结尾礼貌>]
export deny01 = <不要>(<给><我>)?<播放>?(&<poem_author>)<写的>(<诗词类别>) 	 => request(author="$2", tag="$3", intent="deny") <0.8>;
export deny01 = <不要>(<给><我>)?<播放>?(&<poem_author>)<写的>?(<诗>) => request(author="$2", intent="deny") <0.8>;

###### 对应原来的poem_deny2: <主观否定> <诗名> [<这首诗>] [<结尾礼貌>]
export deny02 = <不要>(<给><我>)?<播放>?(&<poem_title>)(这|那)(首|个|种)(<诗词类别>)? => request(title="$2", tag="$5", intent="deny") <0.9>;
export deny02 = <不要>(<给><我>)?<播放>?(&<poem_title>) 							 	 => request(title="$2", intent="deny") <0.9>;

###### 对应原来的poem_deny3: <主观否定> <诗人> [<写的>] <诗名> [<结尾礼貌>]
export deny03 = <不要>(<给><我>)?<播放>?(&<poem_author>)<写的>(&<poem_title>)	 	 => request(author="$2", title="$3", intent="deny") <0.9>;

###### 对应原来的poem_deny4: <主观否定> <其他槽位> <的诗> [<结尾礼貌>]
export deny04 = <不要>(<给><我>)<播放>(<主题>)<写的>(<诗>) => request(author="$2", title="$3", intent="deny") <0.9>;
