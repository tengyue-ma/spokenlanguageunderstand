#include "../comm/digit.lex"

我 =  我|俺|大爷|爷|咱|咱们|老娘|本小姐|本大小姐|我们|本人|给朕;
一个 = (一|几|两|一两)?(篇|首|段|个|则|下|段);
给 = 给|帮|为|替;
想 = 想|要|想要;
播放 = 背|播|放|播放|来|循环播放|朗诵|说;
听一听 = 听(一?听)?;
创作 = (写|创作)过?的?;
语气词 = 吗|嘛|呢|啊|么|哎|唉;
疑问前缀 = <我><想>知道|你知道|告诉<我>;

###### 作者信息
###### 安徒生的
#1a. <story_author> 独词句
export inform0a1 = ^(&<story_author>:author)(<创作>|的)<语气词>?$ => request(intent="inform")<0.9>;

###### 我想听安徒生的
#1b. <story_title> 祈使句(人)
export inform0b1 = ^(<我>?<想><听一听>?<一个>?|<听一听><一个>?)(&<story_author>:author)(<创作>|的)<语气词>?$ => request(intent="inform")<0.9>;

###### 给我一首安徒生的
#1c. <story_author> 祈使句(机器)
export inform0c1 = ^(<给><我>)?<播放><一个>?(&<story_author>:author)(<创作>|的)<语气词>?$ => request(intent="inform")<0.9>;

###### 还有没有安徒生的
###### 安徒生创作的还有么
#1d. <story_autohr> 疑问句
export inform0d1 = ^<疑问前缀>?还?(是否|有没|有么)?有(&<story_author>:author)(<创作>|的)(吗|么|没|不)?$ => request(intent="inform")<0.9>;
export inform0d2 = ^<疑问前缀>?(&<story_author>:author)(<创作>|的)还?(是否|有没|有么)?有(吗|么|没|不)?$ => request(intent="inform")<0.9>;

