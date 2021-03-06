##################################       
#create on 20180720 by zhikai.zhang
#20180927 - 限制数字位数
#20180928 - 支持阳光、闪电、放晴、短袖、凉鞋、拖鞋、清早、大清早、一大早，需要2增加可以话术,支持后面x天，支持我想知道、我说的是、我查询的是、我查的是
#20180929 - 部分两位数字改为一两位数字
#20180930 - 中午、月底
##################################

############# 时间
阿拉伯数字 = 0|1|2|3|4|5|6|7|8|9;
中文数字   = 零|一|二|三|四|五|六|七|八|九|十|百|千|万;
一位数字 = <阿拉伯数字>|<中文数字>;
一两位数字 = (<阿拉伯数字>|<中文数字>){1,2};
两位数字 = (<阿拉伯数字>|<中文数字>){2};
四位数字 = (<阿拉伯数字>|<中文数字>){4};
年数字 = 两位数字|四位数字;
常用单位 = 个月|周|星期|礼拜|天;
时间段 = 凌晨|早上|上午|下午|晚上|傍晚|夜间|午夜|白天|清早|大清早|一大早|中午;                          
时间段短语 = (昨|今|明)(早|晚);
星期 = 星期|周|礼拜;
星期数字 = <一位数字>|日|天;                                            
星期几 = ((上|这|下|上上|下下)一?个?)?<星期><星期数字>;
某周 = (上|这|下|上上|下下|本)一?个?<星期>;    
多天短语 = ((最近|未来|接下来|之后)(<一两位数字>|两|几)<常用单位>)|(明后天|周六日|周末);
相对日 = 前天|昨天|今天|明天|后天|大后天|<一两位数字>天后|后面?<一两位数字>天|月底;      
节日后缀 = ((假|假日|节假日)?)(那天|那一天|期间|那几天|那段时间);
常见节日 = ((元旦|清明|五一|国庆|圣诞|元宵|端午|七夕|中秋|重阳|腊八)节?|(情人|劳动|儿童|教师|母亲|父亲|春)节|(十一|除夕|除夕夜))<节日后缀>?;

时间 = <时间段>|<时间段短语>|<星期几>|<某周>|<多天短语>|<相对日>|<常见节日>;


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
下 = 下|有|是|刮|下没下|下不下|刮不刮|刮没刮|是不是;
可能会 = (是否)?(可能|可不可能|可能不可能|有可能|有没有可能)?(会|要|有没有|有木有|会不会|要不要)?;
可能会下 = <可能会>?<下>?;

## 
export inform = ^<句式前缀>?(<时间>:time)的?天气(是?)<怎样>的?<疑问助词>?$ => request() <0.9>;

######## 晴、雨、雪、雨夹雪、阴、多云、风
######## 以及 冰雹、露、霜、沙尘暴、雷暴、台风、龙卷风、飓风、结冰(暂不支持)
export inform = ^<句式前缀>?(<时间>:time)?<外面>?<可能会下>?<天气前缀1>?(<天气focus1>)<天气后缀1>?<天气询问句式1>?<疑问助词>?$=> request(focus="$2") <0.9>;

######## 风向、风力、温度、气温、湿度、空气
######## 以及 气压、能见度、日照、PM二点五、PM十、日出、日落、日出日落(暂不支持)
export inform = ^<句式前缀>?(<时间>:time)?<外面>?<天气前缀2>?(<天气focus2>)<天气后缀2>?<天气询问句式2>?<疑问助词>?$=> request(focus="$2") <0.9>;

######## 温度
export inform = ^<句式前缀>?(<时间>:time)?<外面>?(温度|气温)多少(度|摄氏度)<疑问助词>?$=> request(focus="$2", focus="$3") <0.9>;


######## [通用说法]穿衣指数、出行指数（指数不能缺省）
######## 以及 紫外线指数、洗车指数、感冒指数、运动指数、晨练指数(暂不支持)
export inform = ^<句式前缀>?(<时间>:time)?<外面>?(<天气focus5>)指数<天气询问句式5>?<疑问助词>?$=> request(focus="$2") <0.9>;

######## [定制说法]穿衣指数 
衣服类型 = T恤|衬衫|毛衣|针织衫|小背心|吊带|蕾丝衫|雪纺衫|外套|风衣|大衣|棉衣|羽绒服|皮衣|马夹|卫衣|西装|连衣裙|短裙|长裙|超短裙|秋裤|牛仔裤|短裤|长裤|短袖|凉鞋|拖鞋;
需要1 = 适合|适宜|需要|要|应该|可以|能;
添加衣服 = 加衣保暖|添加衣服|添加衣物|加衣服;
export inform = ^<句式前缀>?(<时间>:time)?<需要1>?穿什么衣服<疑问助词>?$=> request(focus="穿衣") <0.9>;
export inform = ^<句式前缀>?(<时间>:time)?<需要1>?穿<衣服类型><疑问助词>?$=> request(focus="穿衣") <0.9>;
export inform = ^<句式前缀>?(<时间>:time)?穿<衣服类型>(会冷|会热|会不会冷|会不会热|冷不冷|热不热)<疑问助词>?$=> request(focus="穿衣") <0.9>;
export inform = ^<句式前缀>?(<时间>:time)?(需要|要)<添加衣服><疑问助词>?$=> request(focus="穿衣") <0.9>;

######## [定制说法]出行指数
export inform = ^(<时间>:time)?(是否)?(适宜|适合)(出行|外出)<疑问助词>?$=> request(focus="出行") <0.9>;

######## 太阳、防晒
export inform = ^<句式前缀>?(<时间>:time)?<外面>?(是否)?<需要2>防晒<疑问助词>?$=> request(focus="防晒") <0.9>;
export inform = ^<句式前缀>?(<时间>:time)?<外面>?的?太阳(是否)?大<疑问助词>?$=> request(focus="太阳") <0.9>;


感觉 = 感觉|感到|觉得;

######## 冷、热、凉、晴、阴
export inform = ^<句式前缀>?(<时间>:time)?外面>?(冷|热|凉|晴|阴)快?不(冷|热|凉|晴|阴)快?<疑问助词>?$=> request(focus="$2", focus="$3") <0.9>;
export inform = ^<句式前缀>?(<时间>:time)?<外面>?<可能会>?<感觉>?<很>?的?(<天气focus3>)<天气后缀3>?<疑问助词>?$=> request(focus="$2") <0.9>;

######## 伞
出门 = (出门|外出|出行)(前|时|的时候)?;
需要2 = 有必要|需要|要|应该|要不要|有没有必要|需不需要|可以;
export inform = ^<句式前缀>?(<时间>:time)?(我|我们)?<出门>?<需要2>?(带|打|撑)一?(个|把)?(太阳|遮阳|雨)?伞<疑问助词>?$=> request(focus="伞") <0.9>;

######## 包含两个focus，只解析为前面的那个
export inform = ^<句式前缀>?(<时间>:time)?<外面>?<可能会>?是?(<天气focus4>)天<疑问助词>?还<可能会>?是(<天气focus4>)天<疑问助词>?$=> request(focus="$2") <0.9>;


######## 我想听一下天气预报
export inform = ^<句式前缀>?(我|我们)?想?要?听一?下?(<时间>:time)?的?天气预报<疑问助词>?$=> request() <0.9>;



