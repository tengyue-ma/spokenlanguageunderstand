#include "../comm/digit.lex"

daytime = (这|下|上)?(星期|周)(一|二|三|四|五|六|七|天|日)|(今|明|后|大后)天;
sectime = 早上|清晨|早晨|上午|中午|下午|晚上|傍晚|白天|夜间|夜里|半夜|凌晨;
两位数字 = <全角两位数字>|<汉语两位数字>|<大写两位数字>;
十二进制    = (<汉语一位数字>|拾|拾壹|拾贰|<大写一位数字>|十|十一|十二|<全角一位数字>|10|11|12);
几几年     = <两位数字>?<两位数字>年;
几月      = <十二进制>月;
几日      = <一百以内数字>(日|号);
小时      = (大概|大约)?<十二进制>(点|点钟|时|点半|点整)(多|多一点|左右)?;
分钟      = (大概|大约)?<一百以内数字>(分|分钟|刻|刻钟)(多|多一点|左右)?;
时分      = (<小时><分钟>?)|<分钟>;
绝对时分前后    = <时分>(后|之后|以后|前|之前|以前|前后)?;
时间单位    = <一百以内数字>(小时|个小时|个半小时|分|分钟|刻钟)|半小时|半刻钟;
相对时分前后    = (<时间单位>){1,2}(后|之后|以后|前|之前|以前)?;
时分前后    = <绝对时分前后>|<相对时分前后>;
绝对日期    = (<几几年>?<几月>)?<几日>;

时间日期 = (<绝对日期>+|(<daytime>)+)(<时间段>*);
时间段 = (<sectime>(<时分前后>)?)|<时分前后>;
时间  = <时间日期>|<时间段>;

那 = 那|那么;
再 = 再|另外;
查 = 查一下|查一查|看一下|搜一下|知道|了解|了解一下;
可否 = 能不能|可以|可以不可以|可否;
告诉 = <那>?你?<可否>?(帮忙|帮我)?(再)?(<查>|告诉我);
想知道 = <那>?(我)?想(再)?<查>;

的呢 = 的|的呢|呢|(的(吗|嘛|么));
和 = 和|以及|到|的;

###### 1, 天气领域内，时间相关
## 1-1, 单个时间slot，<time>呢
## 1-2, 多个时间slot的组合，<time>的<time>呢
## e.g, 下午和晚上呢 | 17号和18号 | 明天和大后天呢 | 周三和明天呢 | 昨天和周三呢
## e.g, 18号上午呢 |  明天早上的呢 | 下周一上午呢 | 周一的早上呢 | 周五周六呢
## export inform00 = ^(<告诉>|<想知道>)(<时间>:time)<的呢>$ 	=> request() <0.4>();
export inform00 = ^(<时间>:time)<和>?(<时间>:time1)?<的呢>$ 	=> request() <0.5>;

###### 2, 天气领域内，地点相关
# export inform10=(&<city>:loc_city)(明天|后天)的?呢?$ => request(time="$2") <0.99>;
## 2-1, 单个地点slot，<loc.city>|<loc.area>呢
## e.g, 那苏州呢 | 玄武区的
## 2-2, 不同类型地点slot的组合，<loc.city><loc.area>呢，<loc.country>?<loc.province><loc.city>呢
## e.g, 北京海淀区的 | 中国南京的 | 江苏南京呢 | 中国江苏南京市的 | 中国北京海淀区的
export inform10 = ^(&<country>:loc_country)的?(&<province>:loc_province)?的?(&<city>:loc_city)?的?(&<area>:loc_area)?(<时间>:time1)?和?(<时间>:time2)?<的呢>$ => request() <0.5>;
export inform10 = ^(&<province>:loc_province)的?(&<city>:loc_city)?的?(&<area>:loc_area)?(<时间>:time)?(<时间>:time1)?和?(<时间>:time2)?<的呢>$ => request() <0.4>;
export inform10 = ^(&<city>:loc_city)的?(&<area>:loc_area)?(<时间>:time)?(<时间>:time1)?和?(<时间>:time2)?<的呢>$ => request() <0.3>;
export inform10 = ^(&<area>:loc_area)(<时间>:time)?(<时间>:time1)?和?(<时间>:time2)?<的呢>$ => request() <0.2>;
export inform10 = ^(<时间>:time1)和?(<时间>:time2)?的?(&<country>:loc_country)的?(&<province>:loc_province)?的?(&<city>:loc_city)?的?(&<area>:loc_area)?<的呢>$ => request() <0.5>;
export inform10 = ^(<时间>:time1)和?(<时间>:time2)?的?(&<province>:loc_province)的?(&<city>:loc_city)?的?(&<area>:loc_area)?(<时间>:time)?<的呢>$ => request() <0.4>;
export inform10 = ^(<时间>:time1)和?(<时间>:time2)?的?(&<city>:loc_city)的?(&<area>:loc_area)?(<时间>:time)?<的呢>$ => request() <0.3>;
export inform10 = ^(<时间>:time1)和?(<时间>:time2)?的?(&<area>:loc_area)(<时间>:time)?<的呢>$ => request() <0.2>;

## 2-3, 同一类型地点slot的组合，<loc.city>和<loc.city>呢，<loc.area><loc.area>呢
## e.g, 北京的和天津的呢 | 玄武区的和栖霞区呢
export inform11 = ^(&<city>:loc_city1|&<area>:loc_area1)的?<和>?(&<city>:loc_city2|&<area>:loc_area2)<的呢>$ 	=> request() <0.4>;

## 3-2, 同一类型地点slot的组合 + 单个time
## e.g, 明天南京和北京呢 | 中国的北京明天的呢
## 暂时忽略loc_area的组合，基本不会用到
export inform22 = ^(&<city>)的?<和>?(&<city>)(<时间>)<的呢>$ 	=> request(loc_city1="$1", loc_city2="$2", time="$3") <0.5>;
export inform22 = ^(<时间>)的?(&<city>)的?<和>?(&<city>)<的呢>$ 	=> request(time="$1", loc_city1="$2", loc_city="$3") <0.5>;

