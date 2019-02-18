####################################
#author:zhouleilei                 #
#date:2018.4.28                    #
#slot优先级:title->tag->program	   #
#			->album  			   #
####################################

#include "music_public.lex"

播放操作 = (你好)?(<我想听>|<给我播放>|<你听过>)<一首>?(那个|这个)?;

我想听	 = <我>?(<想>让?你?<播放>|<程度>?喜欢);
给我播放 = <请求礼貌>?<给我>?<播放>;
给我	 = <给><我>;
你听过	 = 你听过;

想		 = <程度>?(想|要|想要|喜欢);
播放	 = 播|播放|放|唱|唱唱|听|听听|听听看|点播|欣赏|学|换|换成|唱|来听|来听听|找找|找|搜|搜搜|搜索|搜寻|查找|查查|查一查|查|查询|列出|搜一搜|找一找|推荐|(再?来<一首>);
一首	 = 一首|首|个|段|一段|一个|几首|几段|一些|一下|下|一曲|点;

请求礼貌 = 那?请?你?(随便)?(快|马上)?;
结尾礼貌 = <疑问助词>|(<好不好><疑问助词>?);
疑问助词 = 吗|呢|啊|咯|呀|吧|么|了|的|啦;
好不好	 = 好不好|行不行|好不|行不|如何|好|行|怎么样;

一点	 = 一点|一些|些|点;
可以	 = <适合>|<喜欢>|可以;

###### 京剧|儿歌|摇滚
###### 歌曲串烧|串烧歌曲
###### 你给我唱歌
export inform = ^<播放操作>?<形容>?(<歌曲类别>:tag|<歌>)<歌>?<播放操作>?<结尾礼貌>?$ => request() <0.9>;
export inform = ^<播放操作>?<歌>(串烧)<播放操作>?<结尾礼貌>?$ => request(tag="$1") <1.0>;
export inform = ^<播放操作>?(串烧)<歌><播放操作>?<结尾礼貌>?$ => request(tag="$1") <1.0>;
export inform = ^<播放操作><歌>(给我)?<播放操作>?<结尾礼貌>?$ => request() <0.9>;

###### 新歌，例:有没有最新的歌
###### 有没有artist新专辑里面的歌
###### 倒装
export inform = ^<播放操作>(&<music_artist>)?<唱的>?最?(新)的?(<歌曲类别>:tag|<歌>)的?<歌>?<播放操作>?<结尾礼貌>?$ => request(artist="$1", rank="$2") <1.0>;
export inform = ^<播放操作>(&<music_artist>)?<唱的>?最?(新)专辑<里面>?的?(<歌曲类别>:tag|<歌>)<播放操作>?<结尾礼貌>?$ => request(artist="$1", rank="$2") <1.0>;

###### 歌手+歌，例:周杰伦的歌(权重较高)
###### 播放+歌手，例:我想听肖邦(权重低)，不属于music，不符合语法逻辑
export inform = ^<播放操作>?<性别搭配>?(&<music_artist>)<唱的>?(<歌曲类别>:tag|<歌>)<歌>?<播放操作>?<结尾礼貌>?$ => request(artist="$1") <1.0>;
#export inform = ^<播放操作>(&<music_artist>)<唱的>?(<歌曲类别>:tag|<歌>)?<播放操作>?<结尾礼貌>?$ => request(artist="$1") <0.4>;

###### 单独点播歌曲名
export inform = ^(&<music_title_single>)$ 	=> request(title="$1") <1.0>;

###### 播放+歌曲名，例:播放告白气球
###### 倒装
###### 歌曲名+播放，例:青花瓷播放
export inform = ^<播放操作><歌>?(&<music_artist>)?<唱的>?(&<music_title>)<歌曲描述>?<这首>?(<歌曲类别>:tag|<歌>)?<播放操作>?<结尾礼貌>?$  => request(artist="$1", title="$2") <0.9>;
export inform = ^<播放操作>?(&<music_artist>)?<唱的>?(&<music_title>)<歌曲描述>?<这首>?(<歌曲类别>:tag|<歌>)?<播放操作><结尾礼貌>?$  => request(artist="$1", title="$2") <0.9>;

###### 歌曲名+这首歌，例:告白气球这首歌|唱笑话这首歌(权重低避免匹配到“上学”歌)
###### 歌手名+歌曲名，例:周杰伦的告白气球
export inform = ^<播放操作>?(&<music_artist>)?<唱的>?(&<music_title>|笑话)的?<歌曲描述>?<这首>(<歌曲类别>:tag|<歌>)<播放操作>?<结尾礼貌>?$  => request(artist="$1", title="$2") <0.5>;
export inform = ^<播放操作>?<性别搭配>?(&<music_artist>)<唱的>?(&<music_title>)的?<这首>?(<歌曲类别>:tag|<歌>)?<播放操作>?<结尾礼貌>?$  => request(artist="$1", title="$2") <0.7>;

###### 歌手名+专辑名:周杰伦的叶惠美(权重低于“歌手名+歌曲名”)
###### 专辑名+[这张]+专辑+[里面的]+[歌]，例：我想听范特西这张专辑
###### 专辑名+[这张]+[专辑]+[里面]+的+歌，例：我想听范特西里面的歌
###### 播放+专辑名，例：我想听叶惠美(权重最低)
export inform = ^<播放操作>?<性别搭配>?(&<music_artist>)<唱的>?(&<music_album>)<这张>?<专辑>?<里面>?的?(<歌曲类别>:tag|<歌>)?<播放操作>?<结尾礼貌>?$  => request(artist="$1", album="$2") <0.65>;
export inform = ^<播放操作>?(&<music_artist>)?<唱的>?(&<music_album>)<这张>?<专辑><里面>?的?(<歌曲类别>:tag|<歌>)?<播放操作>?<结尾礼貌>?$  => request(artist="$1", album="$2") <0.7>;
export inform = ^<播放操作>?(&<music_artist>)?<唱的>?(&<music_album>)<这张>?<专辑>?<里面>?的(<歌曲类别>:tag|<歌>)<播放操作>?<结尾礼貌>?$  => request(artist="$1", album="$2") <0.7>;
export inform = ^<播放操作>(&<music_artist>)?<唱的>?(&<music_album>)<这张>?<专辑>?<里面>?的?(<歌曲类别>:tag|<歌>)?<结尾礼貌>?$  => request(artist="$1", album="$2") <0.5>;

###### 歌手名+歌手名+歌名，例:周杰伦和蔡依林合唱的告白气球这首歌
###### 歌手名+歌手名+的歌，例:周杰伦和蔡依林合唱的歌
export inform = ^<播放操作>?(&<music_artist>)<和>?(&<music_artist>)<合唱>?(&<music_title>)<歌曲描述>?<这首>?(<歌曲类别>:tag|<歌>)?<播放操作>?<结尾礼貌>?$  => request(artist="$1", artist="$2", title="$3") <1.0>;
export inform = ^<播放操作>?(&<music_artist>)<和>?(&<music_artist>)<合唱>?(<歌曲类别>:tag|<歌>)<播放操作>?<结尾礼貌>?$  => request(artist="$1", artist="$2") <1.0>;

###### 语言+儿歌|摇篮曲|歌名，英文歌曲|英文版的摇篮曲|英文版的青花瓷
###### 歌名+用+语言，唱个圣诞歌吧用英文唱(权重低于“播放+title”，避免匹配到"我爱你中国")
###### 歌名+语言+版，
###### 播放用英文唱的歌
export inform = ^<播放操作>?(&<music_artist>)?<唱的>?用?(&<music_language>:language|&<music_region>:region)<版本>?的?(&<music_title>:title|<歌曲类别>:tag|<歌>)<播放操作>?<结尾礼貌>?$ => request(artist="$1") <1.0>;
export inform = ^<播放操作>?(&<music_title>)<疑问助词>?用(&<music_language>:language|&<music_region>:region)的?<播放操作>?<结尾礼貌>?$ => request(title="$1") <0.8>;
export inform = ^<播放操作>?(&<music_title>)<疑问助词>?(&<music_language>:language|&<music_region>:region)<版本>的?<播放操作>?<结尾礼貌>?$ => request(title="$1") <0.8>;
export inform = ^<播放操作>?用?(&<music_language>:language)<唱的>?(&<music_title>:title|<歌曲类别>:tag|<歌>)<播放操作>?<结尾礼貌>?$ => request() <1.0>;

###### 区域+儿歌|摇篮曲
###### 区域+歌曲名
###### [区域]+乐器+歌曲名
export inform = ^<播放操作>?(&<music_artist>)?<唱的>?(&<music_region>)(<歌曲类别>:tag|<歌>)<播放操作>?<结尾礼貌>?$ => request(artist="$1", region="$2") <1.0>;
export inform = ^<播放操作>?(&<music_artist>)?<唱的>?(&<music_region>)<版本>?的?(<歌曲类别>:tag|<歌>)?(&<music_title>)<播放操作>?<结尾礼貌>?$ => request(artist="$1", region="$2", title="$4") <1.0>;
export inform = ^<播放操作>?(&<music_artist>)?<唱的>?(&<music_region>)?<版本>?的?(<乐器>)<版本>?的?(&<music_title>)<播放操作>?<结尾礼貌>?$ => request(artist="$1", region="$2", tag="$3", title="$4") <10000>;

###### 欢快的歌|快节奏的流行歌曲
###### 钢琴演奏的曲子
###### 钢琴曲
#export inform = ^<播放操作>?<形容>?(<歌曲形容标签>|<乐器>)(<演奏>)?<一点>?的?(<歌曲类别>:tag|<歌>)<歌>?<结尾礼貌>?$ => request(tag="$1") <1.0>;
export inform = ^<播放操作>?(&<music_artist>)?<唱的>?<最近>?<程度>?<歌>?(<歌曲形容标签>)<一点>?的?(<歌曲形容标签>)?(<歌曲类别>:tag|<歌>)<歌>?<播放操作>?<结尾礼貌>?$ => request(artist="$1", tag="$2", tag="$3") <1.0>;
export inform = ^<播放操作>?(&<music_artist>)?<唱的>?<用>?(<乐器>)(<演奏>|<版本>)?的?(<歌曲类别>:tag|<歌>)<歌>?<播放操作>?<结尾礼貌>?$ => request(artist="$1", tag="$2") <1.0>;
export inform = ^<播放操作>?(&<music_artist>)?<唱的>?<用>?(<乐器>)(<演奏>|<版本>)?的?(&<music_title>)的?<这首>?(<歌曲类别>:tag|<歌>)?<播放操作>?<结尾礼貌>?$ => request(artist="$1", tag="$2", title="$4") <1.0>;

###### [播放]+program|动画片+主题曲
export inform = ^<播放操作>?(&<music_artist>)?<唱的>?<形容>?(<动画片>)?(&<music_program>:program|<动画片>:tag)<里面>?的?(<主题曲>:tag|<歌>)(&<music_title>)?<播放操作>?<结尾礼貌>?$ => request(artist="$1", tag="$2", title="$5") <0.75>;

###### 年代+[language|region]+歌
export inform = ^<播放操作>?(<年代>)的?(&<music_language>:language|&<music_region>:region)的?(<歌曲类别>)<歌>?<播放操作>?<结尾礼貌>?$ => request(tag="$1", tag="$3") <1.0>;
export inform = ^<播放操作>?(<年代>)的?(&<music_language>:language|&<music_region>:region)的?<歌><播放操作>?<结尾礼貌>?$ => request(tag="$1") <1.0>;

###### [适合]+[歌手]+人群+[歌曲形容]+[language|region]+歌, 黄家驹唱的适合儿童听的欢快一点的粤语歌曲
###### 人群+适合+听什么歌
export inform = ^<播放操作>?(&<music_artist>)?<唱的>?<适合>?给?(<人群>)<可以>?<听>?的?(<歌曲形容标签>)?<一点>?的?(&<music_language>:language|&<music_region>:region)?的?(<歌曲类别>:tag|<歌>)<播放操作>?<结尾礼貌>?$ => request(artist="$1", tag="$2", tag="$3") <1.0>;
export inform = ^<播放操作>?给?(<人群>)<可以>?<听>?的?<什么>的?(<歌曲类别>:tag|<歌>)<播放操作>?<结尾礼貌>?$ => request(tag="$1") <1.0>;


###### [适合]+场景+[时候]+歌
###### 场景+适合+听什么歌
export inform = ^<播放操作>?<适合>?<在>?(<场景>)<时候>?<听>?的?(<歌曲类别>:tag|<歌>)<播放操作>?<结尾礼貌>?$ => request(tag="$1") <1.0>;
export inform = ^<播放操作>?<在>?(<场景>)<时候>?<适合>?<听>?的?<什么>的?(<歌曲类别>:tag|<歌>)<播放操作>?<结尾礼貌>?$ => request(tag="$1") <1.0>;

###### 性别+歌名, 男歌手唱的青花瓷
###### [语言|地区]+性别+[语言|地区]+的歌, 男歌手唱的英文歌
###### 性别+歌名, 男团体唱的青花瓷
###### 性别+[语言|地区]+的歌, 男生乐队唱的英文歌
export inform = ^<播放操作>?(<性别>:gender)<性别修饰>?<性别搭配>?的?(<唱>|<版本>)?的?(&<music_title>)的?<这首>?(<歌曲类别>:tag|<歌>)?<播放操作>?<结尾礼貌>?$ => request(title="$3") <0.9>;
export inform = ^<播放操作>?(&<music_language>:language|&<music_region>:region)?(<性别>:gender)<性别修饰>?<性别搭配>?的?(<唱>|<版本>)?的?(&<music_language>:language|&<music_region>:region)?的?(<歌曲类别>:tag|<歌>)<播放操作>?<结尾礼貌>?$ => request() <1.0>;
export inform = ^<播放操作>?(<性别>:gender)?<性别修饰>?(<组合>:gender)的?(<唱>|<版本>)?的(&<music_title>)的?<这首>?(<歌曲类别>:tag|<歌>)?<播放操作>?<结尾礼貌>?$ => request(title="$3") <1.0>;
export inform = ^<播放操作>?(<性别>:gender)?<性别修饰>?(<组合>:gender)的?(<唱>|<版本>)?的(&<music_language>:language|&<music_region>:region)?的?(<歌曲类别>:tag|<歌>)<播放操作>?<结尾礼貌>?$ => request() <1.0>;

###### [和]+xxx+[有关的]+歌
export inform = ^<播放操作>?<和>?(<某事物>)<有关>?的?(<歌曲类别>:tag|<歌>)<结尾礼貌>?$ => request(tag="$1") <1.0>;









