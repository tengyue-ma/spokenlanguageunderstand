不要  = (不要|不想|不想要|不喜欢|拒绝|别) (再|继续)?;
给    = 给|帮|为|替;
我    = 我|俺|大爷|爷|咱|咱们|老娘|本小姐|本大小姐|我们|本人|朕;
播放  = 播|播放|放|听|讲;
独立故事类别   = 神话|童话|寓言|传说;
非独立故事类别 = 成语;

###### story_deny0: <主观否定> <故事类别> [<结尾礼貌>]
###### 例如: 我不想听神话故事
export deny00 = <不要>(<给><我>)?<播放>?(<独立故事类别>)(故事)?  => request(tag="$2", intent="deny") <0.9>;
export deny01 = <不要>(<给><我>)?<播放>?(<非独立故事类别>)故事 => request(tag="$2", intent="deny") <0.9>;

###### story_deny1: <主观否定> <故事作者> <写的故事> [<结尾礼貌>]
###### 例如: 别将安徒生的故事了
export deny10 = <不要>(<给><我>)?<播放>?(&<story_author>)(写|创作)?的(<独立故事类别>)故事?   => request(author="$2", tag="$4", intent="deny") <0.8>;
export deny11 = <不要>(<给><我>)?<播放>?(&<story_author>)(写|创作)?的(<非独立故事类别>)?故事 => request(author="$2", tag="$4", intent="deny") <0.8>;

###### story_deny2: <主观否定> <故事名> [<这个故事>] [<结尾礼貌>]
###### 例如: 不要灰姑娘
export deny20 = <不要>(<给><我>)?<播放>?(&<story_title>)(这|那)(个|种)(<独立故事类别>)故事?  => request(title="$2", tag="$5", intent="deny") <0.8>;
export deny21 = <不要>(<给><我>)?<播放>?(&<story_title>)(这|那)(个|种)(<非独立故事类别>)?故事  => request(title="$2", tag="$5", intent="deny") <0.8>;

