##################################       
#create on 20180716 by zhikai.zhang
#20180927 - 限制数字位数
#20180928 - 支持阳光、闪电、放晴、短袖、凉鞋、拖鞋、清早、大清早、一大早，需要2增加可以话术，支持后面x天，支持我想知道，我说的是，我查询的是，我查的是
#20180929 - 部分两位数字改为一两位数字
#20180930 - 中午、月底
##################################

####################### 时间
###### 数字与单位
阿拉伯数字 = 0|1|2|3|4|5|6|7|8|9;
中文数字   = 零|一|二|三|四|五|六|七|八|九|十|百|千|万;

一位数字 = <阿拉伯数字>|<中文数字>;
一两位数字 = (<阿拉伯数字>|<中文数字>){1,2};
两位数字 = (<阿拉伯数字>|<中文数字>){2};
四位数字 = (<阿拉伯数字>|<中文数字>){4};
年数字 = 两位数字|四位数字;
单位 = 个月|周|星期|礼拜|天|个?小时|个?钟头|分钟;

###### 一些时间词后缀
那天 =  当天|那天|那一天|这天|这一天;
期间 =  期间|那几天|那段时间;
节日后缀 = ((假|假日|节假日)?)(<那天>|<期间>);

###### 时间段，某天中的一段时间
时间段 = 凌晨|早上|上午|下午|晚上|傍晚|夜间|午夜|白天|清早|大清早|一大早|中午;                         
时间段短语1 = (昨|今|明)(早|晚);

###### 星期几 
星期 = 星期|周|礼拜;
星期数字 = <一位数字>|日|天;                                            
星期几 = <星期><星期数字>;
相对星期几 = (上|这|下|上上|下下)一?个?<星期几>;

###### 上周、最近一周、最近几天、明后天等多天的说法

某周 = (上|这|下|上上|下下|本)一?个?(<星期>|周末);    # 上个星期 上一星期  上一个星期

多天短语1 = (最近|未来|接下来|之后)(<一两位数字>|两|几)<单位>;               # 最近两天，最近一周，最近几天
多天短语2 = 明后天|周六日|近期|这?个?周末;
多天 = <某周>|<多天短语1>|<多天短语2>;

###### 相对年月日，以及某天中某个时间的相对说法
相对年 = 前年|去年|今年|明年|来年;
相对月 = (((上|这|下)一?个?)|本)月;
相对日 = 前一天|第二天|前天|昨天|今天|明天|后天|大后天|<一两位数字>天后|后面?<一两位数字>天|<相对星期几>|月底;      
时间段短语2 = <相对日><时间段>;
时间段短语 = <时间段短语1>|<时间段短语2>;
数字相对 = (<一两位数字><单位>|(半|几)(个?小时|个?钟头|分钟))之?(前|后);

  
###### 绝对时间
绝对年 = <年数字>年;
绝对月 = <一两位数字>月份?;
绝对日 = (<一两位数字>(号|日))<星期几>?|<星期几>;
绝对时 = <一两位数字>(时|点|点钟);
绝对分 = <一两位数字>(分|刻);
绝对秒 = <一两位数字>秒;


###### 节气
二十四节气 = 立春|雨水|惊蛰|春分|清明|谷雨|立夏|小满|芒种|夏至|小暑|大暑|立秋|处暑|白露|秋分|寒露|霜降|立冬|小雪|大雪|冬至|小寒|大寒;
节气 = <二十四节气><那天>?;

###### 节日
### 后缀有没有‘节’都可以
节日1 = 元旦|清明|五一|国庆|圣诞|元宵|端午|七夕|中秋|重阳|腊八;
### 后缀应有‘节’
节日2 = 情人|妇女|三八|三八妇女|植树|愚人|五一劳动|劳动|国际劳动|中国青年|青年|五四青年|国际儿童|儿童|六一儿童|建党|七一建党|建军|八一建军|教师|万圣|复活|母亲|父亲|感恩|春;
### 后缀不应有‘节’
节日3 = 十一|除夕|除夕夜;
节日 = (<节日1>节?|<节日2>节|<节日3>)<节日后缀>?;

###### 时间1：分秒
时间1 = <绝对分><绝对秒>?|<数字相对>;  # 20分10秒     分秒
###### 时分秒
时分秒1 = <绝对时>(<时间1>|半|一刻)?;
时分秒2 = <中文数字>{1,2}点<中文数字>{1,2}分?(<中文数字>{1,2}秒)?;
时分秒 = <时分秒1>|<时分秒2>;
###### 时间2：时间段+时分秒
时间2 = <时间段>?<时分秒>;   # 5点半 / 上午8点半 / 上午5时20分10秒  /  上午5：20
###### 时间3：某天+时间段+时分秒
时间3 = (<绝对日>|<相对日>)(的?<时间2>)?;  # 10号5点半 / 10号上午5时20分10秒 / 明天10点 / 星期五10点 /周五上午10点 / 周六夜间
#时间3 = <绝对日>|<相对日>;  # 10号5点半 / 10号上午5时20分10秒 / 明天10点 / 星期五10点 /周五上午10点 / 周六夜间
###### 时间4：某月的某天+时间段+时分秒
时间4 = (<绝对月>|<相对月>)(的?<时间3>)?;  # 2月20号 / 下个月10号 / 2月20号星期五10点 / 下个月的10号
###### 时间5：年月日+时间段+时分秒
时间5 = (<绝对年>|<相对年>)(的?<时间4>)?;  # 2018年5月 / 明年5月 /明年的5月
###### 时间6: 时间段短语+时分秒

时间6 = (<时间段短语>|<相对日>)(的?<时分秒>)?;
时间7 = <相对日>的?这个(点|时间);
###### 时间8: 所有时间表述
时间8 = <时间1>|<时间2>|<时间3>|<时间4>|<时间5>|<时间6>|<时间7>|<时分秒>|<时间段>|<多天>|<节日>|<节气>;
###### 支持两个时间之间的表述

时间 = 从?<时间8>((到|至)<时间8>(之间|期间|这段时间|那段时间|这期间)?)?;
#时间 = (从?<时间8>(到|至)<时间8>((之间|期间|这段时间|那段时间|这期间)?))|<时间8>;


############# 天气focus
怎样 = 如何|怎样|怎么样|咋样|咋么样|肿么样|肿样|什么样;
好 = 好|还行|还可以|不错|舒服|舒适|适宜;
外面 = (外面|这儿|这里|这)的?;
那里 = (那儿|那里|那边|那|那那)的?;
很 = 很|比较|特别|有点|有点儿|相当;
情况 = 情况|状况|级别|等级;
疑问助词 = 了?(吗|么|呢|啊|呀);

暂不支持focus1 = 冰雹|露|霜|沙尘暴|雷暴|台风|龙卷风|飓风|结冰|雷|雷电|沙尘;
天气focus1 = 晴|雨|雪|雨夹雪|阴|多云|风|雾|雾霾|霾|阳光|闪电|温|<暂不支持focus1>;
天气前缀1 = 小|大|暴|大暴|雷阵|雷|暴风|阵|浓|放|降|打|起|雨|回|高;
天气后缀1 = (天|朗)?|(天气)?(袭击|来袭|来临|降临|登陆|出现)?;
天气询问句式1 = 大不大|严不严重|厉不厉害|影响大|影响大不大|浓不浓;
舒服 = (令|让)?人?(感到|觉得)?<很>?<好>?;
温度focus = 温度|气温|最高温|最低温;
暂不支持focus2 = 气压|能见度|日照|PM二点五|PM十|PM25|PM10|日出|日落|日出日落|可见度;
天气focus2 = 风向|风力|风速|风|<温度focus>|湿度|空气|<暂不支持focus2>;
天气前缀2 = 最高|最低;
天气后缀2 = (污染|质量|质量指数|指数|时间)?<情况>?;
天气询问句式2 = ((是否)?<很>?(大|<舒服>|严重)?)|(是?<怎样>的?)|((有|是|达到|达到了)?(多少|多大|几|多高)?级?)|(好|好不好|高不高|大不大)?;
天气focus3 = 冷|热|凉;
天气后缀3 = 快;
# 天气询问句式3 = 不冷|不热|不凉快;
天气focus4 = 晴|雪|阴|雨;
暂不支持focus5 = 紫外线|洗车|感冒|运动|晨练;
天气focus5 = 穿衣|出行|<暂不支持focus5>;
天气询问句式5 = (是?<怎样>的?<疑问助词>?)|(是?多少);

句式前缀 = 那|那么|我想知道|我说的是|我查询的是|我查的是|需要注意;
时间地点前缀 = <句式前缀>|换个?;    

############# time:
## 明天呢 / 明天的呢
export inform = ^<时间地点前缀>?(<时间>)的?呢?$=> request(time="$1") <1.2>;
## 包含指代的特殊情况
export inform = ^<句式前缀>?(<时间>:time)?<那里>(<时间>:time)?的?(天气)?(是?)<怎样>?的?<疑问助词>?$ => request(loc="unchanged") <1.2>;


############# loc/loc+time/time+loc:
## X
export inform = ^<时间地点前缀>?(<时间>:time)?(&<loc_city>:loc_city|&<loc_area>:loc_area|&<loc_poi>:loc_poi|&<loc_country>:loc_country|&<loc_province>:loc_province)<那里>?(<时间>:time)?的?<疑问助词>?$ => request() <1.2>;
## 国家+X
export inform = ^<时间地点前缀>?(<时间>:time)?(&<loc_country>:loc_country)的?(&<loc_city>:loc_city|&<loc_area>:loc_area|&<loc_poi>:loc_poi|&<loc_province>:loc_province)<那里>?(<时间>:time)?的?<疑问助词>?$ => request() <1.2>;
## 省+X | 国家+省+X
export inform = ^<时间地点前缀>?(<时间>:time)?((&<loc_country>:loc_country)的?)?(&<loc_province>:loc_province)的?(&<loc_city>:loc_city|&<loc_area>:loc_area|&<loc_poi>:loc_poi)<那里>?(<时间>:time)?的?<疑问助词>?$ => request() <1.2>;
## 市+X | 国家+市+X
export inform = ^<时间地点前缀>?(<时间>:time)?((&<loc_country>:loc_country)的?)?(&<loc_city>:loc_city)的?(&<loc_area>:loc_area|&<loc_poi>:loc_poi)<那里>?(<时间>:time)?的?<疑问助词>?$ => request() <1.2>;
## 区+景点 | X+区+景点
export inform = ^<时间地点前缀>?(<时间>:time)?((&<loc_country>:loc_country|&<loc_province>:loc_province|&<loc_city>:loc_city)的?)?(&<loc_area>:loc_area)的?(&<loc_poi>:loc_poi)<那里>?(<时间>:time)?的?<疑问助词>?$ => request() <1.2>;
## 省+市+X | 国家+省+市+X
## 
export inform = ^<时间地点前缀>?(<时间>:time)?((&<loc_country>:loc_country)的?)?(&<loc_province>:loc_province)的?(&<loc_city>:loc_city)的?(&<loc_area>:loc_area|&<loc_poi>:loc_poi)<那里>?(<时间>:time)?的?<疑问助词>?$ => request() <1.2>;
## 市+区+景点 | X+市+区+景点
## 
export inform = ^<时间地点前缀>?(<时间>:time)?((&<loc_province>:loc_province|&<loc_country>:loc_country)的?)?(&<loc_city>:loc_city)的?(&<loc_area>:loc_area)的?(&<loc_poi>:loc_poi)<那里>?(<时间>:time)?的?<疑问助词>?$ => request() <1.2>;
## 国家+省+市+区+景点 | 国家+省+区+景点
## 
export inform = ^<时间地点前缀>?(<时间>:time)?(&<loc_country>:loc_country)的?(&<loc_province>:loc_province)(的?(&<loc_city>:loc_city))?的?(&<loc_area>:loc_area)的?(&<loc_poi>:loc_poi)<那里>?(<时间>:time)?的?<疑问助词>?$ => request() <1.2>;



############# focus:
下 = 下|有|是|刮|下没下|下不下|刮不刮|刮没刮|是不是;
可能会 = (是否)?(可能|可不可能|可能不可能|有可能|有没有可能)?(会|要|有没有|有木有|会不会|要不要)?;
可能会下 = <可能会>?<下>?;

######## 晴、雨、雪、雨夹雪、阴、多云、风
######## 以及 冰雹、露、霜、沙尘暴、雷暴、台风、龙卷风、飓风、结冰(暂不支持)
export inform = ^<句式前缀>?<外面>?<可能会下>?<天气前缀1>?(<天气focus1>)<天气后缀1>?<天气询问句式1>?<疑问助词>?$=> request(focus="$1") <1.2>;
export inform = ^<句式前缀>?(<时间>:time)?<那里>(<时间>:time)?<可能会下>?<天气前缀1>?(<天气focus1>)<天气后缀1>?<天气询问句式1>?<疑问助词>?$=> request(focus="$3", loc="unchanged") <1.2>;

######## 风向、风力、温度、气温、湿度、空气
######## 以及 气压、能见度、日照、PM二点五、PM十、日出、日落、日出日落(暂不支持)
export inform = ^<句式前缀>?<外面>?<天气前缀2>?(<天气focus2>)<天气后缀2>?<天气询问句式2>?<疑问助词>?$=> request(focus="$1") <1.2>;
export inform = ^<句式前缀>?(<时间>:time)?<那里>(<时间>:time)?<天气前缀2>?(<天气focus2>)<天气后缀2>?<天气询问句式2>?<疑问助词>?$=> request(focus="$3", loc="unchanged") <1.2>;

######## 温度 
export inform = ^<句式前缀>?<外面>?(温度|气温)多少(度|摄氏度)<疑问助词>?$=> request(focus="$1", focus="$2") <1.2>;
export inform = ^<句式前缀>?(<时间>:time)?<那里>(<时间>:time)?(温度|气温)多少(度|摄氏度)<疑问助词>?$=> request(focus="$3", focus="$4", loc="unchanged") <1.2>;



######## [通用说法]穿衣指数、出行指数（指数不能缺省）
######## 以及 紫外线指数、洗车指数、感冒指数、运动指数、晨练指数(暂不支持)

export inform = ^<句式前缀>?<外面>?(<天气focus5>)指数<天气询问句式5>?<疑问助词>?$=> request(focus="$1") <1.2>;
export inform = ^<句式前缀>?(<时间>:time)?<那里>(<时间>:time)?(<天气focus5>)指数<天气询问句式5>?<疑问助词>?$=> request(focus="$3", loc="unchanged") <1.2>;


######## [定制说法]穿衣指数 
衣服类型 = T恤|衬衫|毛衣|针织衫|小背心|吊带|蕾丝衫|雪纺衫|外套|风衣|大衣|棉衣|羽绒服|皮衣|马夹|卫衣|西装|连衣裙|短裙|长裙|超短裙|秋裤|牛仔裤|短裤|长裤|短袖|凉鞋|拖鞋;
需要1 = 适合|适宜|需要|要|应该|可以|能;
添加衣服 = 加衣保暖|添加衣服|添加衣物|加衣服;
出门 = (出门|外出|出行)(前|时|的时候)?;
需要2 = 有必要|需要|要|应该|要不要|有没有必要|需不需要|可以;
export inform = ^<句式前缀>?<需要1>?穿什么衣服<疑问助词>?$=> request(focus="穿衣") <1.2>;
export inform = ^<句式前缀>?<需要1>?穿<衣服类型><疑问助词>?$=> request(focus="穿衣") <1.2>;
export inform = ^<句式前缀>?穿<衣服类型>(会冷|会热|会不会冷|会不会热|冷不冷|热不热)<疑问助词>?$=> request(focus="穿衣") <1.2>;
export inform = ^<句式前缀>?(需要|要)<添加衣服><疑问助词>?$=> request(focus="穿衣") <1.2>;

######## [定制说法]出行指数
export inform = ^(是否)?(适宜|适合)(出行|外出)<疑问助词>?$=> request(focus="出行") <1.2>;


######## 太阳、防晒
export inform = ^<句式前缀>?<外面>?(是否)?<需要2>防晒<疑问助词>?$=> request(focus="防晒") <1.2>;
export inform = ^<句式前缀>?<外面>?的?太阳(是否)?大<疑问助词>?$=> request(focus="太阳") <1.2>;
export inform = ^<句式前缀>?(<时间>:time)?<那里>(<时间>:time)?(是否)?<需要2>防晒<疑问助词>?$=> request(focus="防晒", loc="unchanged") <1.2>;
export inform = ^<句式前缀>?(<时间>:time)?<那里>(<时间>:time)?的?太阳(是否)?大<疑问助词>?$=> request(focus="太阳", loc="unchanged") <1.2>;

感觉 = 感觉|感到|觉得;

######## 冷、热、凉
export inform = ^<句式前缀>?<外面>?(冷|热|凉|晴|阴)快?不(冷|热|凉|晴|阴)快?<疑问助词>?$=> request(focus="$1", focus="$2") <1.2>;
export inform = ^<句式前缀>?<外面>?<可能会>?<感觉>?<很>?的?(<天气focus3>)<天气后缀3>?<疑问助词>?$=> request(focus="$1") <1.2>;
export inform = ^<句式前缀>?(<时间>:time)?<那里>(<时间>:time)?<可能会>?<很>?的?(<天气focus3>)<天气后缀3>?<疑问助词>?$=> request(focus="$3", loc="unchanged") <1.2>;

######## 伞
export inform = ^<句式前缀>?(我|我们)?<出门>?<需要2>?(带|打|撑)一?(个|把)?(太阳|遮阳|雨)?伞<疑问助词>?$=> request(focus="伞") <1.2>;

######## 包含两个focus，只解析为前面的那个
export inform = ^<句式前缀>?<外面>?<可能会>?是?(<天气focus4>)天<疑问助词>?还<可能会>?是(<天气focus4>)天<疑问助词>?$=> request(focus="$1") <1.2>;
export inform = ^<句式前缀>?(<时间>:time)?<那里>(<时间>:time)?<可能会>?是?(<天气focus4>)天<疑问助词>?还<可能会>?是(<天气focus4>)天<疑问助词>?$=> request(focus="$3", loc="unchanged") <1.2>;


