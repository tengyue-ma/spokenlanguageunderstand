################################
##                            ##
##      author:lengting       ##
##      time:2018-06-19       ##
##      modify:2018-11-20     ##
##                            ##
################################


这首	= (这|那)一?(首|支|个)?;
歌	= 歌|歌曲|曲子|音乐;
名字	= 名|名字|名儿|歌名|歌名儿|歌曲名|歌曲名儿|标题;
是	= 是|是不是|难道是|难道不是|还是;
叫	= 叫|叫不叫|难道叫|难道不叫;
什么	= 什么|啥|啥么|啥子|啥子么;
句末语气词	= 吧|吗|么|嘛|呢|啊|呀|吖;

他	= 他|她; 
这名	= (这|那)(名|个)?;
歌手	= 歌手|人|艺人|歌星|明星|艺术家|音乐家|大叔|欧巴|小哥哥|帅哥|帅哥哥|小鲜肉|美女|小姐姐|姑娘|萝莉|乐队|组合|团体|男团|女团|男组合|女组合|男团体|女团体;
唱	= 唱|演唱|翻唱|演奏;
唱过	= <唱>过;
哪些	= <什么>|哪些;
其他	= (其他|其它|其余|别)的?;

现在	= 现在|当前|刚刚|刚才;
播放	= 播|放|播放|唱|听;

种	= 个|种|国;
修饰	= (好听|悦耳)(的|得)?;



########### 询问title类[一对一]: 这首歌叫什么名字 | 这首歌叫东风破吗
export request00 = <这首><歌>的?<名字>(叫|是)?<什么>?<句末语气词>?$ => request(intent="request", title="unchanged", request_slot="title") <0.93>;
export request01 = <这首><歌>(叫|是叫?)<什么><名字>? => request(intent="request", title="unchanged", request_slot="title") <0.93>;
#export request02 = <这首><歌>的?<名字>(<叫>|<是>)(.{1,10}|[A-Za-z0-9]{1,25})(<句末语气词>|不)$ => request(intent="request", title="unchanged", request_slot="title") <0.9>;
export request03 = (<这首><歌>|<现在><播放>的<歌>?)的?<名字>?(<叫>|<是>)(&<music_title>|什么|啥)(<句末语气词>|不|<歌>)$ => request(intent="request", title="unchanged", request_slot="title") <0.93>;
export request04 = (这|<这首><歌>?)是<什么><歌> => request(intent="request", title="unchanged", request_slot="title") <0.93>;
export request05 = <这首><歌>?<是>(&<music_artist>)<唱>?(过|的)?(&<music_title>)(<句末语气词>|不)$ => request(intent="request", title="unchanged", request_slot="title") <0.93>;
export request06 = <这首><歌>是不是(&<music_title>)<句末语气词>?$ => request(intent="request", title="unchanged", request_slot="title") <0.933>;
export request07 = <这首><歌>(什么|啥)<名字> => request(intent="request", title="unchanged", request_slot="title") <0.93>;

########### 询问title类(涉及artist)[一对多]: 他唱过哪些歌 | 他还有哪些歌  (备注:"artist唱过哪些歌" / "artist还有哪些歌" other)
########### 指代方式: 他 | 这个歌手
########### 询问方式: 唱过 | 有 | 还唱过 | 还有 
export request10 = (<他>|<这名><歌手>)(还|都)?(<唱过>|有)<其他>?(<哪些>|<什么>)<其他>?<修饰>?<歌><句末语气词>?$ => request(intent="request", artist="unchanged", request_slot="title", request_range="1") <0.93>;
export request10 = (<他>|<这名><歌手>)(还|都)?<唱过><其他>?(<哪些>|<什么>)$ => request(intent="request", artist="unchanged", request_slot="title", request_range="1") <0.93>;
########### 专辑中还有哪些歌
export request11 = 这(个|张)专辑(里面?|中)?(还|都)(有|收录)了?<其他>?(<哪些>|<什么>)<其他>?<歌><句末语气词>?$ => request(intent="request", album="unchanged", request_slot="title", request_range="1") <0.93>;

########### 询问artist类[一对一]: 这首歌是谁唱的[疑问代词] | 这首歌是哪个歌手唱的[疑问代词] | 这首歌是artist唱的吗[特定artist]
export request20 = (这|<这首><歌>)是由?(谁|哪一?个<歌手>|&<music_artist>)<唱>的<歌>?<句末语气词>?$ => request(intent="request", title="unchanged", request_slot="artist") <0.93>;
## 这是谁的歌 | 这是哪个歌手的歌 | 这是哪个人的歌 | 这首歌是谁的歌 | 这首歌是哪个歌手的歌 | 这首歌是哪个人的歌
export request21 = (这|<这首><歌>?)是(谁|哪一?个<歌手>)的<歌> => request(intent="request", title="unchanged", request_slot="artist") <0.93>;
export request22 = <这首><歌>是?(谁|哪一?个<歌手>?)<唱>?的 => request(intent="request", title="unchanged", request_slot="artist") <0.93>;
export request23 = <这首><歌>的?(演唱者|歌手)是?(谁|哪一?个<歌手>|哪一?位)?(<句末语气词>|不)?$ => request(intent="request", title="unchanged", request_slot="artist") <0.93>;
export request24 = <这首>是哪一?个(歌手|艺人|歌星|艺术家|音乐家)<唱>?的  => request(intent="request", title="unchanged", request_slot="artist") <0.93>;
export request22 = (这一?首|这个|歌)是?(谁|哪一?个<歌手>?)<唱>的 => request(intent="request", title="unchanged", request_slot="artist") <0.93>;
## 这是artist的歌吗 | 这首歌是artist的吗 | 这是不是artist的歌 | 这首歌是不是artist的
export request25 = 这<是>(&<music_artist>)唱?的<歌>(<句末语气词>|不)?$ => request(intent="request", title="unchanged", request_slot="artist") <0.96>;
export request26 = <这首><歌><是>(&<music_artist>)唱?的<歌>?(<句末语气词>|不)?$ => request(intent="request", title="unchanged", request_slot="artist") <0.96>;
export request26 = 这是不是(&<music_artist>)唱的<句末语气词>?$ => request(intent="request", title="unchanged", request_slot="artist") <0.96>;
#export request27 = 这<是>(.{1,10}|[A-Za-z0-9]{1,25})唱?的<歌>(<句末语气词>|不)?$ => request(intent="request", title="unchanged", request_slot="artist") <0.9>;
#export request28 = <这首><歌><是>(.{1,10}|[A-Za-z0-9]{1,25})唱的<歌>?(<句末语气词>|不)?$ => request(intent="request", title="unchanged", request_slot="artist") <0.9>;
## 唱这首歌的歌手是谁
export request29 = <唱><这首><歌>的<歌手>是(谁|哪一?个<歌手>)?<句末语气词>?$ => request(intent="request", title="unchanged", request_slot="artist") <0.96>;
## 是谁唱的这首歌
export request210 = ^是?(谁|哪一?个<歌手>)<唱>的<这首><歌><句末语气词>?$ => request(intent="request", title="unchanged", request_slot="artist") <0.96>;
export request211 = (&<music_artist>)<唱过><这首><歌>(吗|么|嘛)$ => request(intent="request", title="unchanged", request_slot="artist") <0.93>;

########### 询问artist类[一对多]: 还有谁唱过这首歌 | 这首歌都有谁翻唱过  (备注:此处不区分原唱翻唱概念)
export request30 = (还|都)有?(谁|<其他>?哪些<歌手>|哪个<歌手>)<唱过><这首><歌><句末语气词>?$ => request(intent="request", title="unchanged", request_slot="artist", request_range="1") <0.9>;
export request31 = <这首><歌>(还|都)?有?(谁|<其他>?哪些<歌手>)<唱过><句末语气词>?$ => request(intent="request", title="unchanged", request_slot="artist", request_range="1") <0.9>;
export request32 = 哪些<歌手><唱过><这首><歌><句末语气词>?$ => request(intent="request", title="unchanged", request_slot="artist", request_range="1") <0.9>;
export request33 = (&<music_artist>)(是不是|有没有|有木有|有么有)<唱过><这首><歌><句末语气词>?$ => request(intent="request", title="unchanged", request_slot="artist", request_range="1") <0.9>;
export request34 = (还|都)有?(谁|<其他>?哪些<歌手>|哪个<歌手>)<唱过>(&<music_title>:title)<句末语气词>?$ => request(intent="request", request_slot="artist", request_range="1") <0.9>;

########### 询问album类[一对一]: 这首歌是哪张专辑里的 | 这首歌是album里面的吗[特定album]
########### 这首歌是哪张专辑里面的 | 这首歌是魔杰座专辑里面的吗 | 这首歌是周杰伦哪张专辑里面的 | 这首歌是周杰伦唱的魔杰座专辑里面的吗
########### 询问方式: 是...吗 | 是不是 | 是...不 | ...是不(后置的暂未处理)
# export request40 = <这首><歌><是>(.{1,10}|[A-Za-z0-9]{1,25})?<唱>?过?的?(哪一?张|哪一?个|.{1,10}|[A-Za-z0-9]{1,25})专辑里面?的<歌>?(<句末语气词>|不)?$ => request(intent="request", title="unchanged", request_slot="album") <0.9>;
# export request41 = <这首><歌><是>(.{1,10}|[A-Za-z0-9]{1,25})?<唱>?过?的?(&<music_album>)(专辑)?里面?的<歌>?(<句末语气词>|不)?$ => request(intent="request", title="unchanged", request_slot="album") <0.9>;
# export request40 = (<这首><歌>|<现在><播放>的<歌>?)<是>(&<music_artist>)?<唱>?过?的?(哪一?张|哪一?个|<什么>|&<music_album>)(专辑)?里?面?中?的<歌>?(<句末语气词>|不)?$ => request(intent="request", title="unchanged", request_slot="album") <0.93>;
# export request41 = <这首><是>(&<music_artist>)?<唱>?过?的?(哪一?张|哪一?个|<什么>|&<music_album>)专辑里?面?中?的<歌>?(<句末语气词>|不)?$ => request(intent="request", title="unchanged", request_slot="album") <0.93>;
# export request42 = (这|<这首><歌>)<是>(&<music_artist>)?<唱>?过?的?专辑(&<music_album>)里?面?中?的<歌>?(<句末语气词>|不)?$ => request(intent="request", title="unchanged", request_slot="album") <0.93>;
export request40 = (<这首>|<现在><播放>的)<歌>?(<是>|在)?(&<music_artist>:artist)?<唱>?过?的?(哪一?张|哪一?个|<什么>)专辑里?面?中?的?<歌>?<句末语气词>?$ => request(intent="request", title="unchanged", request_slot="album") <0.93>;
export request41 = (<这首>|<现在><播放>的)<歌>?的?(专辑)<是>(&<music_artist>:artist)?<唱>?过?的?(专辑)?(&<music_album>)(专辑)?里?面?中?的?<歌>?(<句末语气词>|不)?$ => request(intent="request", title="unchanged", request_slot="album") <0.93>;
export request41 = (<这首>|<现在><播放>的)<歌>?<是>(&<music_artist>:artist)?<唱>?过?的?(专辑)(&<music_album>)里?面?中?的?<歌>?(<句末语气词>|不)?$ => request(intent="request", title="unchanged", request_slot="album") <0.93>;
export request41 = (<这首>|<现在><播放>的)<歌>?<是>(&<music_artist>:artist)?<唱>?过?的?(&<music_album>)(专辑)里?面?中?的?<歌>?(<句末语气词>|不)?$ => request(intent="request", title="unchanged", request_slot="album") <0.93>;
export request42 = (<这首>|<现在><播放>的)<歌>?(<是>|在)(<他>|<这名><歌手>)<唱>?过?的?(哪一?张|哪一?个|<什么>)专辑里?面?中?的?<歌>?<句末语气词>?$ => request(intent="request", title="unchanged", artist="unchanged", request_slot="album") <0.93>;
export request43 = 这(张|个)专辑(叫|是)<什么>(名|名字|名儿)?<句末语气词>?$ => request(intent="request", album="unchanged", request_slot="album") <0.93>;

########### 询问album类(涉及artist)[一对多]: 他还有哪些专辑
export request50 = (<他>|<这名><歌手>)还?(<唱过>|有)<其他>?<哪些>专辑<句末语气词>?$ => request(intent="request", artist="unchanged", request_slot="album", request_range="1") <0.93>;
#export request51 = (&<music_artist>:artist)还?(<唱过>|有)<其他>?<哪些>专辑<句末语气词>?$ => request(intent="request", request_slot="album", request_range="1") <0.93>;

########### 询问language类: 这首歌是用哪个语言唱的 | 这首歌是粤语歌吗
## 这是哪个语言的歌
## 这是用哪个语言唱的
## 这首歌是哪个语言的歌
## 这首歌是用哪个语言唱的
## 这首歌是粤语歌吗
## 这是用xx语言唱的吗
## 这首歌是用xx语言唱的吗
export request60 = (这|<这首><歌>)是?用?(<什么>|哪一?<种>|哪一?个?国家?)的?语言唱?的<歌>? => request(intent="request", title="unchanged", request_slot="language") <0.97>;
export request61 = (这|<这首><歌>?)<是>?用?(&<music_language>)(语言)?(<唱>的|<歌>|的)还?是?用?(&<music_language>)?<唱>?的?<句末语气词>?$ => request(intent="request", title="unchanged", request_slot="language") <0.96>;

########### 询问region类: 这是哪里的歌 | 这首歌是西班牙的歌吗
## 这是哪里的歌
## 这首歌是哪里的
## 这是西班牙的歌吗
## 这是不是西班牙的歌
## 这首歌是西班牙的歌吗
## 这首歌是不是西班牙的歌
export request70 = 这是(哪里|哪一?个地方|哪一?个国家|哪一?个地区)的<歌>(<句末语气词>|不)?$ => request(intent="request", title="unchanged", request_slot="region") <0.95>;
export request71 = <这首><歌>是?(来自)?(哪里|哪一?个地方|哪一?个国家|哪一?个地区)的?<歌>?(<句末语气词>|不)?$ => request(intent="request", title="unchanged", request_slot="region") <0.95>;
export request72 = 这<是>(&<music_region>)的?<歌>(<句末语气词>|不)?$  => request(intent="request", title="unchanged", request_slot="region") <0.95>;
export request73 = (<这首><歌>|<现在><播放>的<歌>?)<是>(&<music_region>)<歌手>?<唱>?的?<歌>?(<句末语气词>|不)?$  => request(intent="request", title="unchanged", request_slot="region") <0.95>;

########### 询问gender类: 这首歌是男的还是女的唱的 
export request80 = (这|<这首><歌>)是?(男|女)(生|声|性|的)?<歌手>?唱的<歌>?(<句末语气词>|不)$ => request(intent="request", title="unchanged", request_slot="gender") <0.98>;
export request81 = (这|<这首><歌>)是不是(男|女)(生|声|性|的)?<歌手>?唱的<歌>?(呢|啊|呀|吖)?$ => request(intent="request", title="unchanged", request_slot="gender") <0.98>;
export request82 = (这|<这首><歌>)是?(男|女)(生|声|性|的)?<歌手>?(唱的)?还是(男|女)(生|声|性|的)?<歌手>?唱的<歌>?(呢|啊|呀|吖)?$ => request(intent="request", title="unchanged", request_slot="gender") <0.98>;
#export request82 = (这|<这首><歌>)是男的?<歌手>?(唱的)?还是女的?<歌手>?唱的(呢|啊|呀|吖)?$ => request(intent="request", title="unchanged", request_slot="gender") <0.98>;
#export request82 = (这|<这首><歌>)是女的?<歌手>?(唱的)?还是男的?<歌手>?唱的(呢|啊|呀|吖)?$ => request(intent="request", title="unchanged", request_slot="gender") <0.98>;
export request83 = <唱><歌>的是(男|女)(生|声|性|的)<歌手>?还是(男|女)(生|声|性|的)<歌手>?(呢|啊|呀|吖)?$ => request(intent="request", title="unchanged", request_slot="gender") <0.98>;

########## 询问program类: 这首歌是哪部电影的主题曲
#export request90 = (这|<这首><歌>)(<是>|出自|来自)(哪一?个|哪一?部|<什么>|&<music_program>)的?(电影|电视剧|综艺|动漫|动画片)里?面?中?的?(主题曲|片头曲|片尾曲|插曲|<歌>)?<句末语气词>?$ => request(intent="request", title="unchanged", request_slot="program") <0.93>;
#export request91 = (&<music_program>)(电影|电视剧|综艺|动漫|动画片)?里?面?中?的?(主题曲|片头曲|片尾曲|插曲|<歌>)<是><这首><歌>?(<句末语气词>|不)?$ => request(intent="request", title="unchanged", request_slot="program") <0.93>;
#export request92 = <这首><歌>?(<是>|出自|来自)(&<music_program>)的?(电影|电视剧|综艺|动漫|动画片)?里?面?中?的?(主题曲|片头曲|片尾曲|插曲|<歌>)<句末语气词>?$ => request(intent="request", title="unchanged", request_slot="program") <0.93>;
