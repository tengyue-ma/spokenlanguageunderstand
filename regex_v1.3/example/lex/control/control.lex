#include "../comm/digit.lex"

音量大  = 高|大|响|加;
音量小  = 低|小|少|轻;
加      = 加|提|增|调节?|开|放;
减      = 减|降|调节?|开|放;
更      = 更|再;
停      = 暂停|停止|停止播放|暂停播放|停|结束|结束播放;
等      = 稍等|等等|等一下|等一等|等下;
关      = 关一下|关闭|退出|关了|关起来|关掉;
播放的  = 播?放?的?;
前      = (前|上|(上面|前面|刚才|刚刚)<播放的>那?);
一点    = 一?(点|点儿|些|下);
音量    = 音量|声音|声|音[^质乐];
祈使    = 请|请你|麻烦|麻烦你|可以|可不可以|能|能不能|能否|是否可以|帮我;
帮我    = 给我|为我|帮我;
想      = 想|想再|想要|要|好想|希望;
诗      = 诗|词|诗词|诗歌|诗句|名诗|诗朗诵|古诗;
歌      = 歌|歌曲|专辑|曲|音乐|乐;
个      = 首|个|曲|集|段|则;
故事    = 神话|童话|寓言|传说|成语故事|故事;
应用    = 微信;


#调音
export control00 = <加>?<音量大><一点>?<音量> => request(object="volumn", operation="adjust", val_rel="up") <0.3>;
export control00 = <减>?<音量小><一点>?<音量> => request(object="volumn", operation="adjust", val_rel="down") <0.3>;
export control01 = <音量大>声<一点> => request(object="volumn", operation="adjust", val_abs="up") <0.3>;
export control01 = <音量小>声<一点> => request(object="volumn", operation="adjust", val_abs="down") <0.3>;
export control02 = <音量>(<祈使>|还是|稍微)?<帮我>?<更>?<加>(<音量大>|加) => request(object="volumn", operation="adjust", val_rel="up") <0.3>;
export control02 = <音量>(<祈使>|还是|稍微)?<帮我>?<更>?<减>(<音量小>) => request(object="volumn", operation="adjust", val_rel="down") <0.3>;
export control03 = <音量>(<祈使>|还是|稍微)?<帮我>?<更>?(<音量大>)<一点> => request(object="volumn", operation="adjust", val_rel="up") <0.3>;
export control03 = <音量>(<祈使>|还是|稍微)?<帮我>?<更>?(<音量小>)<一点> => request(object="volumn", operation="adjust", val_rel="down") <0.3>;

export control04 = 最<音量大><音量> => request(object="volumn", operation="adjust", val_abs="max") <0.3>;
export control04 = 最<音量小><音量> => request(object="volumn", operation="adjust", val_abs="min") <0.3>;
export control05 = <音量>(调|开)?到?最<音量大> => request(object="volumn", operation="adjust", val_abs="max") <0.3>;
export control05 = <音量>(调|开)?到?最<音量小> => request(object="volumn", operation="adjust", val_abs="min") <0.3>;

export control06 = <音量>(太|有<一点>)<音量大> => request(object="volumn", operation="adjust", val_rel="down") <0.3>;
export control06 = <音量>(太|有<一点>)<音量小> => request(object="volumn", operation="adjust", val_rel="up") <0.3>;
export control07 = <音量><音量大>了 => request(object="volumn", operation="adjust", val_rel="down") <0.3>;
export control07 = <音量><音量小>了 => request(object="volumn", operation="adjust", val_rel="up") <0.3>;
export control07 = <音量>不够<音量大> => request(object="volumn", operation="adjust", val_rel="up") <0.3>;
export control07 = <音量>不够<音量小> => request(object="volumn", operation="adjust", val_rel="down") <0.3>;

export control08 = 听不(到|见) => request(object="volumn", operation="adjust", val_rel="up") <0.3>;
export control08 = (太吵|吵死)(了|啦)|别(发出|太|放太)<音量大><音量> => request(object="volumn", operation="adjust", val_rel="down") <0.3>;

export control08 = <音量>(<加>|<减>)到(<一百以内数字>) => request(object="volumn", operation="adjust", val_abs="$2") <0.3>;

#控音
export control10 = (<祈使><帮我>?|先|调到|调成)(静音|安静) => request(object="volumn", operation="mute") <0.3>;
export control10 = 静音|安静|闭嘴|别说了 => request(object="volumn", operation="mute") <0.2>;
export control10 = <音量><关> => request(object="volumn", operation="mute") <0.3>;
export control10 = <关>.{0,5}<音量> => request(object="volumn", operation="mute") <0.3>;
export control10 = (不要|别)(开|放|播放|出|发出|有)<音量> => request(object="volumn", operation="mute") <0.3>;
export control11 = ^<祈使>?<帮我>?(取消|关闭|结束)静音 => request(object="volumn", operation="unmute") <0.4>;
export control11 = 打开<音量> => request(object="volumn", operation="unmute") <0.3>;
export control11 = <音量>打开 => request(object="volumn", operation="unmute") <0.3>;
export control12 = (恢复|回|回复)到?(刚才|原先|原来)的?<音量> => request(object="volumn", operation="adjust") <0.3>;
export control12 = (恢复|回复)<音量> => request(object="volumn", operation="adjust") <0.3>;

#开关
export control20 = 下次再玩 => request(object="robot", operation="shutdown") <0.2>;
export control20 = ^(请|立刻|马上|现在|直接)?(关闭|关机|关掉)吧?$ => request(object="robot", operation="shutdown") <0.45>;
export control21 = ^(开机重启|重新启动|重启) => request(object="robot", operation="restart") <0.3>;
export control22 = ^(睡眠) => request(object="robot", operation="sleep") <0.3>;


#循环
export control30 = (一直播?放|重复播?放|循环播?放|重复|重播|单曲循环|循环|再听|再放|再来)(这首歌|这个歌曲)?(<一百以内数字>)?(次|遍)? => request(object="history", operation="play", mode="single_cycle", rep_val="$3") <0.4>;
export control31 = 一直播?放|重复播?放|循环播?放|重复|重播|单曲循环|循环 => request(object="history", operation="play", mode="single_cycle") <0.2>;
export control31 = (顺序|列表|依次)(循环|播放|听) => request(object="history", operation="play", mode="list_cycle") <0.3>;
export control32 = 随机播放 => request(object="history", operation="play", mode="random") <0.3>;

#操作|暂停
export control40 = (打开|开启)(播放|音乐) => request(object="history", operation="play") <0.3>;
export control40 = (<关>)(播放|音乐|歌曲?|了|一下) => request(object="history", operation="pause") <0.3>;
export control40 = (播放|音乐|歌曲?|<祈使>|先)<关> => request(object="history", operation="pause") <0.3>;
export control40 = ^(<祈使>|先|现在)?(<停>|<等>) => request(object="history", operation="pause") <0.3>;
export control40 = ^(<祈使>|先|现在)?(别|不要|取消)<停> => request(object="history", operation="play") <0.4>;
export control40 = ((<停>|休息)(下|下来|一下)?|<等>)(<祈使>?(么|吗|嘛|吧))?$ => request(object="history", operation="pause") <0.3>;
export control40 = (继续|接着|重新|再次|再|恢复|开始)(播|放|播放|听) => request(object="history", operation="play") <0.35>;
export control40 = (继续|接着|重新|再次|恢复) => request(object="history", operation="play") <0.2>;
export control40 = ^(播放|可以播放(吗|嘛|么))$ => request(object="history", operation="play") <0.3>;
export control41 = 不<想>?(听|继续听?)(歌|了|这)? => request(object="history", operation="pause") <0.3>;
export control41 = (别再?|不要)((放|播放|播|唱)(啦|了|这首|这个)|说话|讲话|吵吵) => request(object="history", operation="pause") <0.3>;

#收藏
export control42 = 收藏(这首|一下) => request(object="history", operation="collect") <0.3>;
export control42 = 收藏<前>一?<个> => request(object="history", operation="collect", val_rel="-1") <0.4>;
export control42 = 加入收藏 => request(object="history", operation="collect") <0.2>;
export control43 = 把这首.{0,2}(加|放)(入|到|进|入到)收藏夹 => request(object="history", operation="collect") <0.3>;


#切换
export control53 = <前>一?<个> => request(object="history", operation="play", val_rel="-1") <0.3>;
export control54 = ^<祈使>?<帮我>?(切|切歌|换|切换|换歌)(至|到)?<前>(<一百以内数字>)<个> => request(object="history", operation="play", sign="-1", val_rel="$3") <0.4>;
export control55 = ^<祈使>?<帮我>?(切|切歌|换|切换|换歌|快进)((至|到)(后|下))?(<一百以内数字>) => request(object="history", operation="play", val_rel="$3") <0.4>;
export control55 = ^<祈使>?<帮我>?(切歌|换(首|另|其他|别|歌)|切换|切到|跳过) => request(object="history", operation="play", val_rel="1") <0.3>;
export control55 = 换|下一?<个> => request(object="history", operation="play", val_rel="1") <0.3>;
export control55 = ^<祈使>?<帮我>?(切|换)$ => request(object="history", operation="play", val_rel="1") <0.3>;
export control56 = <想>听(一首)?(别的|其他|其它) => request(object="history", operation="play", val_rel="1") <0.3>;

#应用
export control60 = 打开(<应用>) => request(object="application", operation="open", name="$1") <0.3>;
export control61 = <关>(<应用>) => request(object="application", operation="close", name="$1") <0.3>;
export control62 = <关>(这个(应用|软件|APP|app|App)) => request(object="application", operation="close", name="this") <0.3>;

###问题
#安静点：是静音还是小声
