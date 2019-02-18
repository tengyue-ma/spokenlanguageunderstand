#########################
#author:zhouleilei      #
#date:2018.4.28         #
#########################

#include "../music/music_public.lex"

播放否定 = <请求礼貌>?(<我不想>|<不要给我>)<播放>?;

我不想	 = <我>?<现在>?<程度>?(<不喜欢>|<不想听>)<你>?;
不喜欢	 = (不要|不想要|不喜欢|拒绝|讨厌|不爱)<再>?<播放>?;
不想听	 = (不想|停止|别|不|不爱|不是很想|不是要)<再>?<播放>;

不要给我 = <你>?<现在>?<能不能>?<不要><再>?<给我>?;
不要	 = 不要|别;
给我	 = <给><我>?;
你		 = 你|朋友|机器人;
播放	 = 播|播放|放|总放|唱|听;
再		 = 再|继续|一直|老;
现在	 = 现在|此时;

请求礼貌 = <你>?<能不能>?也?请?(快|马上)?;
结尾礼貌 = (<疑问助词>?<疑问助词>)|(<好不好><疑问助词>?);
疑问助词 = 吗|呢|啊|咯|呀|吧|么|了|的|啦|吧;
能不能	 = 能不能|能|可以|可不可以|拜托;
好不好	 = 好不好|行不行|好不|行不|如何|可以|好|行|怎么样|可不可以;

这首	 = 这首|那首|这个|那个;
这张	 = 这张|那张|这个|那个;
这么	 = 这么|那么|<程度>|这种|这样;

他		 = 他|她|这个歌手|这个人|这人|这种人;
风格	 = 风格|类型|年代;
这种	 = 这种|这|这个;
难听	 = 难听|差|不好听|不喜欢;
很难听	 = (真|很|超|太)?<难听>?<难听>的?;
嗓音	 = 嗓音|声音|歌声;
干什么	 = 干嘛|干什么;
版本	 = 版本|版;

###### 不要京剧|儿歌|摇滚|民谣
###### 倒装
export deny = ^<播放否定>(<歌曲类别>)<结尾礼貌>?$ => request(tag="$1", intent="deny") <1.0>;
export deny = ^(<歌曲类别>)<播放否定><结尾礼貌>?$ => request(tag="$1", intent="deny") <1.0>;

###### 不要周杰伦的|不要周杰伦的歌|不要周杰伦的摇滚|不要周杰伦的摇滚歌曲
###### 倒装
###### 不要总放周杰伦的这首歌
###### 倒装
export deny = ^<播放否定>(&<music_artist>)<唱的>?(<歌曲形容标签>)?(<歌曲类别>:tag|<歌>)?<结尾礼貌>?$ => request(artist="$1", tag="$2", intent="deny") <1.0>;
export deny = ^(&<music_artist>)<唱的>?(<歌曲形容标签>)?(<歌曲类别>:tag|<歌>)?<播放否定><结尾礼貌>?$ => request(artist="$1", tag="$2", intent="deny") <1.0>;
export deny = ^<播放否定>(&<music_artist>)<唱的>?<这首>(<歌曲类别>:tag|<歌>)?<结尾礼貌>?$ => request(artist="$1", title="unchanged", intent="deny") <1.0>;
export deny = ^(&<music_artist>)<唱的>?<这首>(<歌曲类别>:tag|<歌>)?<播放否定><结尾礼貌>?$ => request(artist="$1", title="unchanged", intent="deny") <1.0>;

###### 不要青花瓷这首歌
###### 倒装
###### 不要叶惠美这个专辑(权重小)
###### 倒装
export deny = ^<播放否定>(&<music_title>)<这首>?(<歌曲类别>:tag|<歌>)?<结尾礼貌>?$ => request(title="$1", intent="deny") <0.9>;
export deny = ^(&<music_title>)<这首>?(<歌曲类别>:tag|<歌>)?<播放否定><结尾礼貌>?$ => request(title="$1", intent="deny") <0.9>;
export deny = ^<播放否定>(&<music_album>)<这张>?<专辑>?<结尾礼貌>?$ => request(album="$1", intent="deny") <0.5>;
export deny = ^(&<music_album>)<这张>?<专辑>?<播放否定><结尾礼貌>?$ => request(album="$1", intent="deny") <0.5>;

###### 不要周杰伦的青花瓷
###### 倒装
export deny = ^<播放否定>(&<music_artist>)<唱的>?(&<music_title>)<结尾礼貌>?$ => request(artist="$1", title="$2", intent="deny") <1.0>;
export deny = ^(&<music_artist>)<唱的>?(&<music_title>)<播放否定><结尾礼貌>?$ => request(artist="$1", title="$2", intent="deny") <1.0>;

###### 不要忧伤的
###### 倒装
###### 不要忧伤的英文|英国，不要英文流行歌
###### 倒装
###### 不想听大陆女的，大陆男歌手唱的
###### 倒装
###### 不想听男乐队唱的
###### 倒装
export deny = ^<播放否定><这么>?(<歌曲形容标签>)的?(新)?(<歌曲类别>:tag|<歌>)?<结尾礼貌>?$ => request(tag="$1", rank="$2", intent="deny") <1.0>;
export deny = ^<这么>?(<歌曲形容标签>)的?(新)?(<歌曲类别>:tag|<歌>)?<播放否定><结尾礼貌>?$ => request(tag="$1", rank="$2", intent="deny") <1.0>;
export deny = ^<播放否定><这么>?(<歌曲形容标签>|<年代>)?的?(&<music_language>:language|&<music_region>:region)<版本>?(<歌曲形容标签>)?的?(<歌曲类别>:tag|<歌>)?<结尾礼貌>?$ => request(tag="$1", tag="$3", intent="deny") <1.0>;
export deny = ^<这么>?(<歌曲形容标签>|<年代>)?的?(&<music_language>:language|&<music_region>:region)<版本>?(<歌曲形容标签>)?的?(<歌曲类别>:tag|<歌>)?<播放否定><结尾礼貌>?$ => request(tag="$1", tag="$3", intent="deny") <1.0>;
export deny = ^<播放否定><这么>?(&<music_region>:region)?的?(<性别>:gender)<性别修饰>?<性别搭配>?的?(<唱>|<版本>)?的?(&<music_language>:language|&<music_region>:region)?<版本>?的?(<歌曲类别>:tag|<歌>)?<结尾礼貌>?$ => request(intent="deny") <1.0>;
export deny = ^<这么>?(&<music_region>:region)?的?(<性别>:gender)<性别修饰>?<性别搭配>?的?(<唱>|<版本>)?的?(&<music_language>:language|&<music_region>:region)?<版本>?的?(<歌曲类别>:tag|<歌>)?<播放否定><结尾礼貌>?$ => request(intent="deny") <1.0>;
export deny = ^<播放否定><这么>?(&<music_region>:region)的?(<性别>:gender)?<性别修饰>?(<组合>:gender)的?(<唱>|<版本>)?的(<歌曲类别>:tag|<歌>)?<结尾礼貌>?$ => request(intent="deny") <1.0>;
export deny = ^<这么>?(&<music_region>:region)的?(<性别>:gender)?<性别修饰>?(<组合>:gender)的?(<唱>|<版本>)?的(<歌曲类别>:tag|<歌>)?<播放否定><结尾礼貌>?$ => request(intent="deny") <1.0>;

###### 不要听西游记的歌|不要西游记的主题曲
export deny = ^<播放否定>(&<music_artist>)?<唱的>?(&<music_program>:program|<动画片>:tag)的?(<主题曲>:tag|<歌>)<结尾礼貌>?$ => request(artist="$1", intent="deny") <1.0>;
#export deny = ^<播放否定>(&<music_artist>)?<唱的>?(<动画片>)的?(<主题曲>:tag|<歌>)<结尾礼貌>?$ => request(artist="$1", tag="$2", intent="deny") <1.0>;

###### 不要他[唱]的 | 不要他的歌
###### 倒装
###### 不要他唱歌
###### 倒装
###### 不要他的红豆这首歌
###### 倒装
export deny = ^<播放否定><他><唱>?的(<歌曲类别>:tag|<歌>|<嗓音>)?<结尾礼貌>?$ => request(artist="unchanged", intent="deny") <1.0>;
export deny = ^<他><唱>?的(<歌曲类别>:tag|<歌>|<嗓音>)?<播放否定><结尾礼貌>?$ => request(artist="unchanged", intent="deny") <1.0>;
export deny = ^<播放否定><他><唱>的?(<歌曲类别>:tag|<歌>|<嗓音>)?<结尾礼貌>?$ => request(artist="unchanged", intent="deny") <1.0>;
export deny = ^<他><唱>的?(<歌曲类别>:tag|<歌>|<嗓音>)?<播放否定><结尾礼貌>?$ => request(artist="unchanged", intent="deny") <1.0>;
export deny = ^<播放否定><他><唱的>?(&<music_title>)<这首>?(<歌曲类别>:tag|<歌>|<嗓音>)?<结尾礼貌>?$ => request(artist="unchanged", title="$1", intent="deny") <0.9>;
export deny = ^<他><唱的>?(&<music_title>)<这首>?(<歌曲类别>:tag|<歌>|<嗓音>)?<播放否定><结尾礼貌>?$ => request(artist="unchanged", title="$1", intent="deny") <0.9>;

###### 不要这首歌太难听了
###### 倒装
###### 这首很难听
export deny = ^<播放否定><这首>(<歌曲类别>:tag|<歌>)?<结尾礼貌>?<很难听>?<结尾礼貌>?$ => request(title="unchanged", intent="deny") <1.0>;
export deny = ^<这首>(<歌曲类别>:tag|<歌>)?<播放否定><结尾礼貌>?$ => request(title="unchanged", intent="deny") <1.0>;
export deny = ^<这首>(<歌曲类别>:tag|<歌>)?<很难听><结尾礼貌>?$ => request(title="unchanged", intent="deny") <1.0>;

###### 不要这首歌
export deny = ^<播放否定>(&<music_language>:language|&<music_region>:region|<歌曲形容标签>:tag)?<版本>?的?<这首>(<歌曲类别>:tag|<歌>)?<结尾礼貌>?$ => request(title="unchanged", intent="deny") <0.9>;
export deny = ^(&<music_language>:language|&<music_region>:region|<歌曲形容标签>:tag)?<版本>?的?<这首>(<歌曲类别>:tag|<歌>)?<播放否定><结尾礼貌>?$ => request(title="unchanged", intent="deny") <0.9>;

###### 不要这张专辑里面的歌
###### 倒装
export deny = ^<播放否定>(这张|这个)专辑(里面|里)?的(<歌曲类别>:tag|<歌>)?<结尾礼貌>?$ => request(album="unchanged", intent="deny") <1.0>;
export deny = ^(这张|这个)专辑(里面|里)?的(<歌曲类别>:tag|<歌>)?<播放否定><结尾礼貌>?$ => request(album="unchanged", intent="deny") <1.0>;

###### 不要这种歌|不要这种类型的歌
###### 不要这种风格的|不想听这种风格的美国歌
export deny = ^<播放否定><这种><风格>?的?<歌><结尾礼貌>?$ => request(tag="unchanged", intent="deny") <0.7>;
export deny = ^<这种><风格>?的?<歌><播放否定><结尾礼貌>?$ => request(tag="unchanged", intent="deny") <0.7>;
export deny = ^<播放否定><这种><风格>的(&<music_language>:language|&<music_region>:region)?<歌>?<结尾礼貌>?$ => request(tag="unchanged", intent="deny") <0.7>;
export deny = ^<这种><风格>的(&<music_language>:language|&<music_region>:region)?<歌>?<播放否定><结尾礼貌>?$ => request(tag="unchanged", intent="deny") <0.7>;

###### 你放xxx干嘛呀
export deny = ^<你>?<播放>(<歌曲类别>)<干什么><结尾礼貌>?$ => request(tag="$1", intent="deny") <0.7>;

###### 不要这种新歌|老歌
###### 倒装
export deny = ^<播放否定><这种>?(新)<歌><结尾礼貌>?$ => request(rank="$1", intent="deny") <1.0>;
export deny = ^<这种>?(新)<歌><播放否定><结尾礼貌>?$ => request(rank="$1", intent="deny") <1.0>;



















