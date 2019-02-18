####################################
#author:zhouleilei                 #
#date:2018.4.28                    #
#slot优先级:title->album->program  #
####################################

#include "music_public.lex"

确认	 = <你>?<难道>?<确不确认>(随便)?<给我>?(<唱>|<放>)?<一首>?;
结尾	 = <疑问助词>?<疑问助词>?<唱一首>?<疑问助词>?|(<好不好><疑问助词>?);
我想听	 = <我>?(想听|想学|想学唱)<一首>?;
给我	 = <给><我>;

确不确认 = (<会不会>|<有没有>|<能不能>|<可不可以>|<知不知道>)<什么>?<形容>?;
会不会	 = 会不会|是不是会|是不是不会|不会|会;
有没有	 = 有没有|有木有|有么有|是否有|这有|这里有|还有|有|没有|是不是有|是不是没有;
能不能	 = 能不能|能不能唱|是不是能唱|是不是不能唱|能唱|不能唱|能|能否;
可不可以 = 可不可以|可以;
知不知道 = 知不知道|知道;
疑问助词 = 呢|啊|吧|呀|啦|吗|嘛|么|了|的|咯;
好不好	 = 好不好|行不行|好不|行不|如何|好|行|怎么样;

你		 = 你|我;
难道	 = 难道|到底|应该;
一首	 = 一首|一下|首|一个|一点|一个|个;
唱一首	 = (<给我唱>|<唱给我听>|<其他结尾>);
给我唱	 = <给我>?<唱><一首>;
唱给我听 = <唱><一首>?<给我>?听?;
其他结尾 = 放来听听|来听几次|我们一起唱|怎么唱|听听|来听听|听;

放		 = 放|播放|听|来放|来|听过;

可以	 = <适合>|<喜欢>|可以;

###### 你会不会京剧呀
###### 倒装
###### 你会唱歌吗
###### 倒装
###### 你会不会歌曲串烧, ("串烧"比较特别,可以放在"歌曲"的后面)
###### 倒装
###### 能不能放首歌听
export confirm = ^<确认>(<歌曲类别>)<歌>?<结尾>?$ => request(tag="$1", intent="confirm") <1.0>;
export confirm = ^<我想听>?(<歌曲类别>)<歌>?<确认><结尾>?$ => request(tag="$1", intent="confirm") <1.0>;
export confirm = ^<确认>唱<歌><结尾>?$ => request(intent="confirm") <1.0>;
export confirm = ^<我想听>?唱<歌><确认><结尾>?$ => request(intent="confirm") <1.0>;
export confirm = ^<确认><歌>(串烧)<结尾>?$ => request(tag="$1", intent="confirm") <1.0>;
export confirm = ^<我想听>?(串烧)<歌><确认><结尾>?$ => request(tag="$1", intent="confirm") <1.0>;
export confirm = ^<确认><放><一首>?<歌><结尾>?$ => request(intent="confirm") <1.0>;

###### 新歌，例:有没有最新的歌
###### 倒装
export confirm = ^<确认>最?(新|近)的?(<歌曲形容标签>)?<一点>?的?(<歌曲类别>:tag|<歌>)的?<歌>?<结尾>?$ => request(rank="$1", tag="$2", intent="confirm") <1.0>;
export confirm = ^<我想听>?最?(新|近)的?(<歌曲形容标签>)?<一点>?的?(<歌曲类别>:tag|<歌>)的?<歌>?<确认><结尾>?$ => request(rank="$1", tag="$2", intent="confirm") <1.0>;

###### 会不会+歌手+歌，例:你会不会周杰伦的歌
###### 歌手+歌+会不会，例:周杰伦的歌你会不会
export confirm = ^<确认><性别搭配>?(&<music_artist>)<唱的>?最?(新|近)?的?(<歌曲形容标签>)?<一点>?的?(<歌曲类别>:tag|<歌>|<专辑>)的?<歌>?<结尾>?$ => request(artist="$1", rank="$2", tag="$3", intent="confirm") <0.8>;
export confirm = ^<我想听>?<性别搭配>?(&<music_artist>)<唱的>?最?(新|近)?的?(<歌曲形容标签>)?<一点>?的?(<歌曲类别>:tag|<歌>|<专辑>)的?<歌>?<确认><结尾>?$ => request(artist="$1", rank="$2", tag="$3", intent="confirm") <0.8>;

###### 会不会+歌曲名，例:你会不会周杰伦的告白气球这首歌, 梁静茹的宁夏很好听你能播放一下吗
###### 歌曲名+会不会，例:周杰伦的告白气球这首歌你会不会
###### 歌名+会不会这首歌，例：周杰伦的告白气球你会不会这首歌
export confirm = ^<确认>(&<music_artist>)?<唱的>?(<歌曲形容标签>)?<一点>?的?(&<music_title>)<歌曲描述>?<这首>?(<歌曲类别>:tag|<歌>)?<结尾>?$  => request(artist="$1", tag="$2", title="$3", intent="confirm") <0.8>;
export confirm = ^<我想听>?(&<music_artist>)?<唱的>?(<歌曲形容标签>)?<一点>?的?(&<music_title>)<歌曲描述>?<这首>?(<歌曲类别>:tag|<歌>)?<确认><结尾>?$  => request(artist="$1", tag="$2", title="$3", intent="confirm") <0.8>;
export confirm = ^(&<music_artist>)?<唱的>?(&<music_title>)<确认><这首>?(<歌曲类别>:tag|<歌>)?<结尾>?$  => request(artist="$1", title="$2", intent="confirm") <0.8>;

###### 会不会+歌手名+专辑名:周杰伦的叶惠美(权重低于“歌手名+歌曲名”)
###### 倒装
###### 会不会+专辑名+[这张]+专辑+[里面的]+[歌], 你会不会周杰伦的叶惠美这张专辑（专辑的权重要小）
###### 倒装
###### 专辑名+[这张]+[专辑]+[里面]+的+歌，例：你会不会范特西里面的歌
###### 倒装
export confirm = ^<确认><性别搭配>?(&<music_artist>)<唱的>?(&<music_album>)<这张>?<专辑>?<里面>?的?(<歌曲类别>:tag|<歌>)?<结尾>?$  => request(artist="$1", album="$2", intent="confirm") <0.65>;
export confirm = ^<我想听>?<性别搭配>?(&<music_artist>)<唱的>?(&<music_album>)<这张>?<专辑>?<里面>?的?(<歌曲类别>:tag|<歌>)?<确认><结尾>?$  => request(artist="$1", album="$2", intent="confirm") <0.65>;
export confirm = ^<确认>(&<music_artist>)?<唱的>?(&<music_album>)<这张>?<专辑><里面>?的?(<歌曲类别>:tag|<歌>)?<结尾>?$  => request(artist="$1", album="$2", intent="confirm") <0.7>;
export confirm = ^<我想听>?(&<music_artist>)?<唱的>?(&<music_album>)<这张>?<专辑><里面>?的?(<歌曲类别>:tag|<歌>)?<确认><结尾>?$  => request(artist="$1", album="$2", intent="confirm") <0.7>;
export confirm = ^<确认>(&<music_artist>)?<唱的>?(&<music_album>)<这张>?<专辑>?<里面>?的(<歌曲类别>:tag|<歌>)<结尾>?$  => request(artist="$1", album="$2", intent="confirm") <0.7>;
export confirm = ^<我想听>?(&<music_artist>)?<唱的>?(&<music_album>)<这张>?<专辑>?<里面>?的(<歌曲类别>:tag|<歌>)<确认><结尾>?$  => request(artist="$1", album="$2", intent="confirm") <0.7>;

###### 你连远走高飞都不会唱
export confirm = ^<你>?<难道>?连?(&<music_title>)都(不会|不能|不可以)<唱>?<结尾>?$  => request(title="$1", intent="confirm") <0.8>;

###### 会不会+歌手名+歌手名+歌名，例:会不会周杰伦和蔡依林合唱的告白气球|会不会周杰伦和蔡依林合唱的歌
###### 会不会+歌手名+歌手名+的歌，例:会不会周杰伦和蔡依林合唱的歌
export confirm = ^<确认>(&<music_artist>)<和>?(&<music_artist>)<合唱>?(&<music_title>:title|<歌曲类别>:tag|<歌>)<结尾>?$  => request(artist="$1", artist="$2", intent="confirm") <1.0>;
export confirm = ^<我想听>?(&<music_artist>)<和>?(&<music_artist>)<合唱>?(&<music_title>:title|<歌曲类别>:tag|<歌>)<确认><结尾>?$  => request(artist="$1", artist="$2", intent="confirm") <1.0>;

###### 会不会+语言+歌曲名+[儿歌|摇篮曲]
###### 倒装
###### 会不会+语言+[儿歌|摇篮曲]+歌曲名
###### 倒装
###### 会不会+语言+儿歌|摇篮曲
###### 倒装
###### 会不会+歌曲名+[儿歌|摇篮曲]+语言
###### 倒装
###### 会不会+[标签]+语言+儿歌|摇篮曲
###### 倒装
export confirm = ^<确认>(&<music_artist>)?<唱的>?(&<music_language>:language|&<music_region>:region)<版本>?的?<形容>?(&<music_title>)(<歌曲类别>:tag|<歌>)?<结尾>?$ => request(artist="$1", title="$3", intent="confirm") <1.0>;
export confirm = ^<我想听>?(&<music_artist>)?<唱的>?(&<music_language>:language|&<music_region>:region)<版本>?的?<形容>?(&<music_title>)(<歌曲类别>:tag|<歌>)?<确认><结尾>?$ => request(artist="$1", title="$3", intent="confirm") <1.0>;
export confirm = ^<确认>(&<music_artist>)?<唱的>?(&<music_language>:language|&<music_region>:region)<版本>?的?<形容>?(<歌曲类别>:tag|<歌>)<结尾>?$ => request(artist="$1", intent="confirm") <1.0>;
export confirm = ^<我想听>?(&<music_artist>)?<唱的>?(&<music_language>:language|&<music_region>:region)<版本>?的?<形容>?(<歌曲类别>:tag|<歌>)<确认><结尾>?$ => request(artist="$1", intent="confirm") <1.0>;
export confirm = ^<确认>(&<music_artist>)?<唱的>?(&<music_language>:language|&<music_region>:region)<版本>?的?<形容>?(<歌曲类别>:tag|<歌>)?(&<music_title>)<结尾>?$ => request(artist="$1", title="$4", intent="confirm") <1.0>;
export confirm = ^<我想听>?(&<music_artist>)?<唱的>?(&<music_language>:language|&<music_region>:region)<版本>?的?<形容>?(<歌曲类别>:tag|<歌>)?(&<music_title>)<确认><结尾>?$ => request(artist="$1", title="$4", intent="confirm") <1.0>;
export confirm = ^<确认>(&<music_title>)的?(&<music_language>:language|&<music_region>:region)<版本>?的?(<歌曲类别>:tag|<歌>)?<结尾>?$ => request(title="$1", intent="confirm") <0.8>;
export confirm = ^<我想听>?(&<music_title>)的?(&<music_language>:language|&<music_region>:region)<版本>?的?(<歌曲类别>:tag|<歌>)?<确认><结尾>?$ => request(title="$1", intent="confirm") <0.8>;
export confirm = ^<确认>(<歌曲形容标签>)?<一点>?的?(&<music_language>:language|&<music_region>:region)<版本>?的?<形容>?(<歌曲类别>:tag|<歌>)<结尾>?$ => request(tag="$1", intent="confirm") <1.0>;
export confirm = ^<我想听>?(<歌曲形容标签>)?<一点>??的?(&<music_language>:language|&<music_region>:region)<版本>?的?<形容>?(<歌曲类别>:tag|<歌>)<确认><结尾>?$ => request(tag="$1", intent="confirm") <1.0>;

###### 会不会欢快的歌|有没有快节奏的流行歌曲啊
###### 倒装
###### 会不会钢琴演奏的歌曲
###### 倒装
###### 葫芦丝版的葫芦娃
###### 倒装
export confirm = ^<确认>(&<music_artist>)?<唱的>?<最近>?<程度>?<歌>?(<歌曲形容标签>)<一点>?的?(<歌曲形容标签>)?(<歌曲类别>:tag|<歌>)<歌>?<结尾>?$ => request(artist="$1", tag="$2", tag="$3", intent="confirm") <1.0>;
export confirm = ^<我想听>?(&<music_artist>)?<唱的>?<最近>?<程度>?<歌>?(<歌曲形容标签>)<一点>?的?(<歌曲形容标签>)?(<歌曲类别>:tag|<歌>)<确认><歌>?<结尾>?$ => request(artist="$1", tag="$2", tag="$3", intent="confirm") <1.0>;
export confirm = ^<确认>(&<music_artist>)?<唱的>?<用>?(<乐器>)(<演奏>|<版本>)?的?(<歌曲类别>:tag|<歌>)<歌>?<结尾>?$ => request(artist="$1", tag="$2", intent="confirm") <1.0>;
export confirm = ^<我想听>?(&<music_artist>)?<唱的>?<用>?(<乐器>)(<演奏>|<版本>)?的?(<歌曲类别>:tag|<歌>)<确认><歌>?<结尾>?$ => request(artist="$1", tag="$2", intent="confirm") <1.0>;
export confirm = ^<确认>(&<music_artist>)?<唱的>?<用>?(<乐器>)(<演奏>|<版本>)?的?(&<music_title>)的?<这首>?(<歌曲类别>:tag|<歌>)?<结尾>?$ => request(artist="$1", tag="$2", title="$4", intent="confirm") <1.0>;
export confirm = ^<我想听>?(&<music_artist>)?<唱的>?<用>?(<乐器>)(<演奏>|<版本>)?的?(&<music_title>)的?<这首>?(<歌曲类别>:tag|<歌>)?<确认><结尾>?$ => request(artist="$1", tag="$2", title="$4", intent="confirm") <1.0>;

###### 会不会+program|动画片+主题曲
###### 倒装
###### 会不会+program|动画片+[title]
###### 倒装
export confirm = ^<确认>(&<music_artist>)?<唱的>?<形容>?(<动画片>)?(&<music_program>:program|<动画片>:tag)<里面>?的?(<主题曲>:tag|<歌>)(&<music_title>)?<结尾>?$ => request(artist="$1", tag="$2", title="$5", intent="confirm") <0.75>;
export confirm = ^<我想听>?(&<music_artist>)?<唱的>?<形容>?(<动画片>)?(&<music_program>:program|<动画片>:tag)<里面>?的?(<主题曲>:tag|<歌>)(&<music_title>)?<确认><结尾>?$ => request(artist="$1", tag="$2", title="$5", intent="confirm") <0.75>;
export confirm = ^<确认>(&<music_artist>)?<唱的>?<形容>?(<动画片>)?(&<music_program>:program|<动画片>:tag)<里面>?的?(&<music_title>)?<结尾>?$ => request(artist="$1", tag="$2", title="$4", intent="confirm") <0.4>;
export confirm = ^<我想听>?(&<music_artist>)?<唱的>?<形容>?(<动画片>)?(&<music_program>:program|<动画片>:tag)<里面>?的?(&<music_title>)?<确认><结尾>?$ => request(artist="$1", tag="$2", title="$4", intent="confirm") <0.4>;

###### 会不会+年代+[language|region]+歌
###### 倒装
###### 会不会+language|region+歌
###### 倒装
###### 会不会+用language+唱的歌
###### 倒装
export confirm = ^<确认>(<年代>)的?(&<music_language>:language|&<music_region>:region)?的?(<歌曲类别>:tag|<歌>)<歌>?<结尾>?$ => request(tag="$1", intent="confirm") <1.0>;
export confirm = ^<我想听>?(<年代>)的?(&<music_language>:language|&<music_region>:region)?的?(<歌曲类别>:tag|<歌>)<歌>?<确认><结尾>?$ => request(tag="$1", intent="confirm") <1.0>;
export confirm = ^<确认>(&<music_language>:language|&<music_region>:region)的?<形容>?(<歌曲形容标签>)?<一点>?的?(<歌曲类别>:tag|<歌>)<歌>?<结尾>?$ => request(tag="$2", intent="confirm") <1.0>;
export confirm = ^<我想听>?(&<music_language>:language|&<music_region>:region)的?<形容>?(<歌曲形容标签>)?<一点>?的?(<歌曲类别>:tag|<歌>)<歌>?<确认><结尾>?$ => request(tag="$2", intent="confirm") <1.0>;
export confirm = ^<确认><用>?(&<music_language>:language)<唱的>?(&<music_title>:title|<歌曲类别>:tag|<歌>)<结尾>?$ => request(intent="confirm") <1.0>;
export confirm = ^<我想听>?<用>?(&<music_language>:language)<唱的>?(&<music_title>:title|<歌曲类别>:tag|<歌>)<确认><结尾>?$ => request(intent="confirm") <1.0>;

###### 会不会+[歌手]+适合给+人群+的+[歌曲形容]+[language|region]+歌, 有黄家驹唱的适合儿童听的欢快一点的粤语歌曲吗|你可以唱适合学龄前孩子的悲伤的儿歌吗|你能不能放一首老年人喜欢的革命歌曲
###### 倒装
###### 会不会+[适合给]+人群+[可以]+听的+歌, 女生听的歌
###### 倒装
###### 知不知道+人群+[适合]+听+什么歌, 女生适合听什么歌
###### 倒装
export confirm = ^<确认>(&<music_artist>)?<唱的>?(<可以>|给|<可以>给)(<人群>)<可以>?<听>?的(<歌曲形容标签>)?<一点>?的?(&<music_language>:language|&<music_region>:region)?的?(<歌曲类别>:tag|<歌>)<结尾>?$ => request(artist="$1", tag="$3", tag="$4", intent="confirm") <1.0>;
export confirm = ^<我想听>?(&<music_artist>)?<唱的>?(<可以>|给|<可以>给)(<人群>)<可以>?<听>?的(<歌曲形容标签>)?<一点>?的?(&<music_language>:language|&<music_region>:region)?的?(<歌曲类别>:tag|<歌>)<确认><结尾>?$ => request(artist="$1", tag="$3", tag="$4", intent="confirm") <1.0>;
export confirm = ^<确认><可以>?给?(<人群>)<可以>?<听>的(<歌曲形容标签>)?<一点>?的?(&<music_language>:language|&<music_region>:region)?的?(<歌曲类别>:tag|<歌>)<结尾>?$ => request(tag="$1", tag="$2", intent="confirm") <1.0>;
export confirm = ^<我想听>?<可以>?给?(<人群>)<可以>?<听>的(<歌曲形容标签>)?<一点>?的?(&<music_language>:language|&<music_region>:region)?的?(<歌曲类别>:tag|<歌>)<确认><结尾>?$ => request(tag="$1", tag="$2", intent="confirm") <1.0>;
export confirm = ^<确认>给?(<人群>)<可以>?<听>的?<什么>的?(<歌曲类别>:tag|<歌>)<结尾>?$ => request(tag="$1", intent="confirm") <1.0>;
export confirm = ^<我想听>?给?(<人群>)<可以>?<听>的?<什么>的?(<歌曲类别>:tag|<歌>)<确认><结尾>?$ => request(tag="$1", intent="confirm") <1.0>;

###### 会不会+[适合]+场景+[时候]+歌
###### 倒装
###### 知不知道+场景+适合+听什么歌
###### 倒装
export confirm = ^<确认><适合>?<在>?(<场景>)<时候>?<听>?的?(<歌曲类别>:tag|<歌>)<结尾>?$ => request(tag="$1", intent="confirm") <1.0>;
export confirm = ^<我想听>?<适合>?<在>?(<场景>)<时候>?<听>?的?(<歌曲类别>:tag|<歌>)<确认><结尾>?$ => request(tag="$1", intent="confirm") <1.0>;
export confirm = ^<确认><在>?(<场景>)<时候>?<适合>?<听>?的?<什么>?的?(<歌曲类别>:tag|<歌>)<结尾>?$ => request(tag="$1", intent="confirm") <1.0>;
export confirm = ^<我想听>?<在>?(<场景>)<时候>?<适合>?<听>?的?<什么>?的?(<歌曲类别>:tag|<歌>)<确认><结尾>?$ => request(tag="$1", intent="confirm") <1.0>;

###### 会不会+xxx+[有关的]+歌
###### 倒装
export confirm = ^<确认><和>?<关于>?(<某事物>)<有关>?的?(&<music_language>:language|&<music_region>:region)?的?(<歌曲类别>:tag|<歌>)<结尾>?$ => request(tag="$1", intent="confirm") <1.0>;
export confirm = ^<我想听>?<和>?<关于>?(<某事物>)<有关>?的?(&<music_language>:language|&<music_region>:region)?的?(<歌曲类别>:tag|<歌>)<确认><结尾>?$ => request(tag="$1", intent="confirm") <1.0>;

###### 会不会+性别+歌名, 会不会男歌手的歌
###### 倒装
###### 会不会+性别+[语言|地区]+的歌, 会不会女歌唱家的英文歌
###### 倒装
###### 会不会+性别+团体+歌名, 会不会乐队的歌
###### 倒装
###### 会不会+性别+团体+[语言|地区]+的歌, 会不会女团的英文歌
###### 倒装
export confirm = ^<确认>(<性别>:gender)<性别修饰>?<性别搭配>?的?(<唱>|<版本>)?的(&<music_title>)的?<这首>?(<歌曲类别>:tag|<歌>)?<结尾>?$ => request(title="$3", intent="confirm") <1.0>;
export confirm = ^<我想听>?(<性别>:gender)<性别修饰>?<性别搭配>?的?(<唱>|<版本>)?的(&<music_title>)的?<这首>?(<歌曲类别>:tag|<歌>)?<确认><结尾>?$ => request(title="$3", intent="confirm") <1.0>;
export confirm = ^<确认>(<性别>:gender)<性别修饰>?<性别搭配>?的?(<唱>|<版本>)?的(&<music_language>:language|&<music_region>:region)?的?(<歌曲类别>:tag|<歌>)<结尾>?$ => request(intent="confirm") <1.0>;
export confirm = ^<我想听>?(<性别>:gender)<性别修饰>?<性别搭配>?的?(<唱>|<版本>)?的(&<music_language>:language|&<music_region>:region)?的?(<歌曲类别>:tag|<歌>)<确认><结尾>?$ => request(intent="confirm") <1.0>;
export confirm = ^<确认>(<性别>:gender)?<性别修饰>?(<组合>:gender)的?(<唱>|<版本>)?的(&<music_title>)的?<这首>?(<歌曲类别>:tag|<歌>)?<结尾>?$ => request(title="$3", intent="confirm") <1.0>;
export confirm = ^<我想听>?(<性别>:gender)?<性别修饰>?(<组合>:gender)的?(<唱>|<版本>)?的(&<music_title>)的?<这首>?(<歌曲类别>:tag|<歌>)?<确认><结尾>?$ => request(title="$3", intent="confirm") <1.0>;
export confirm = ^<确认>(<性别>:gender)?<性别修饰>?(<组合>:gender)的?(<唱>|<版本>)?的(&<music_language>:language|&<music_region>:region)?的?(<歌曲类别>:tag|<歌>)<结尾>?$ => request(intent="confirm") <1.0>;
export confirm = ^<我想听>?(<性别>:gender)?<性别修饰>?(<组合>:gender)的?(<唱>|<版本>)?的(&<music_language>:language|&<music_region>:region)?的?(<歌曲类别>:tag|<歌>)<确认><结尾>?$ => request(intent="confirm") <1.0>;









