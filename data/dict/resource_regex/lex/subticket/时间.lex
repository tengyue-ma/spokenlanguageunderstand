#include "../comm/time.lex"

搜索=有没有|(要|找|搜|查|订|找找|查查|搜搜|查一查|找一找)(一|两|三|四|五|六|七|八|九|十|几)?(下|张)?;
有 = 有|还有;

出发 = 出发|发车|起飞|走;
去   = 去|到|飞往?|直飞|直达;
时间 = <时刻>(之?前|之?后|左右|开始)?;
吧   = 啊|呀|吧;

订票种类=(火车|飞机|汽车)票?;
车次类型=动车|高铁|特快|普快|绿皮|直达;
航空公司=(春秋|厦门|东方|南方)航空|东航|南航|厦航;

动车席别=(特|一|二)等座|商务座;
绿皮席别=硬座|硬卧|软卧|卧铺|站票;
机票席别=头等舱|商务舱|经济舱;
席别=<动车席别>|<绿皮席别>|<机票席别>;

火车车次=(K|T|Z|D|G|快|普快|特|特快|直|直达|动|动车|高|高铁)?(<数字>+);
航班号=(CA|CZ|MU|HU|ZH|FM|MF|SC|EU|BK|8C|9C|3U)<数字>+;
车次=<火车车次>|<航班号>;

#================================================
###### 以时间开头 ######
	### 时间+出发地?+目的地?
export exp11 = ^<有>? (<时间>) 从? (&<country>:dep_country|&<province>:dep_province|&<city>:dep_city|&<area>:dep_area|&<site>:dep_site|&<station>:dep_station)? <出发>? <去>? (&<country>:des_country|&<province>:des_province|&<city>:des_city|&<area>:des_area|&<site>:des_site|&<station>:des_station)? 的? (<车次类型>:train_type|<航空公司>:airline)? 的? (<席别>:level)?的?(呢|啊|呀|吗|还?有吗|还?有没有)?$ => request(time="$1") <0.9>;

export exp12 = <搜索> (<时间>) 从? (&<country>:dep_country|&<province>:dep_province|&<city>:dep_city|&<area>:dep_area|&<site>:dep_site|&<station>:dep_station)? <出发>? <去>? (&<country>:des_country|&<province>:des_province|&<city>:des_city|&<area>:des_area|&<site>:des_site|&<station>:des_station)? 的? (<车次类型>:train_type|<航空公司>:airline)? 的? (<席别>:level)?的?(呢|啊|呀|吗|还?有吗|还?有没有)?$ => request(time="$1") <0.9>;

	### 时间+目的地?+出发地?
export exp21 = ^<有>? (<时间>) <去>? (&<country>:des_country|&<province>:des_province|&<city>:des_city|&<area>:des_area|&<site>:des_site|&<station>:des_station) 从? (&<country>:dep_country|&<province>:dep_province|&<city>:dep_city|&<area>:dep_area|&<site>:dep_site|&<station>:dep_station) <出发> 的? (<车次类型>:train_type|<航空公司>:airline)? 的? (<席别>:level)?的?(呢|啊|呀|吗|还?有吗|还?有没有)?$ => request(time="$1") <0.9>;

export exp22 = <搜索> (<时间>) <去>? (&<country>:des_country|&<province>:des_province|&<city>:des_city|&<area>:des_area|&<site>:des_site|&<station>:des_station) 从? (&<country>:dep_country|&<province>:dep_province|&<city>:dep_city|&<area>:dep_area|&<site>:dep_site|&<station>:dep_station) <出发> 的? (<车次类型>:train_type|<航空公司>:airline)? 的? (<席别>:level)?的?(呢|啊|呀|吗|还?有吗|还?有没有)?$ => request(time="$1") <0.9>;

#================================================
###### 以出发地开头
	### 出发地+时间?+目的地?
export exp31 = ^<有>? (&<country>:dep_country|&<province>:dep_province|&<city>:dep_city|&<area>:dep_area|&<site>:dep_site|&<station>:dep_station) <出发>? (<时间>)? <出发>? <去>? (&<country>:des_country|&<province>:des_province|&<city>:des_city|&<area>:des_area|&<site>:des_site|&<station>:des_station)? 的? (<车次类型>:train_type|<航空公司>:airline)? 的? (<席别>:level)?的?(呢|啊|呀|吗|还?有吗|还?有没有)?$ => request(time="$2") <0.9>;

export exp32 = <搜索> (&<country>:dep_country|&<province>:dep_province|&<city>:dep_city|&<area>:dep_area|&<site>:dep_site|&<station>:dep_station) <出发>? (<时间>)? <出发>? <去>? (&<country>:des_country|&<province>:des_province|&<city>:des_city|&<area>:des_area|&<site>:des_site|&<station>:des_station)? 的? (<车次类型>:train_type|<航空公司>:airline)? 的? (<席别>:level)?的?(呢|啊|呀|吗|还?有吗|还?有没有)?$ => request(time="$2") <0.9>;

	### 出发地+目的地?+时间?
export exp41 = ^<有>? (&<country>:dep_country|&<province>:dep_province|&<city>:dep_city|&<area>:dep_area|&<site>:dep_site|&<station>:dep_station) <出发>? <去>? (&<country>:des_country|&<province>:des_province|&<city>:des_city|&<area>:des_area|&<site>:des_site|&<station>:des_station)? (<时间>)? <出发>? 的? (<车次类型>:train_type|<航空公司>:airline)? 的? (<席别>:level)?的?(呢|啊|呀|吗|还?有吗|还?有没有)?$ => request(time="$3") <0.9>;

export exp42 = <搜索> (&<country>:dep_country|&<province>:dep_province|&<city>:dep_city|&<area>:dep_area|&<site>:dep_site|&<station>:dep_station) <出发>? <去>? (&<country>:des_country|&<province>:des_province|&<city>:des_city|&<area>:des_area|&<site>:des_site|&<station>:des_station)? (<时间>)? <出发>? 的? (<车次类型>:train_type|<航空公司>:airline)? 的? (<席别>:level)?的?(呢|啊|呀|吗|还?有吗|还?有没有)?$ => request(time="$3") <0.9>;

#================================================
###### 以目的地开头
	### 目的地+时间?+出发地?
export exp50 = ^<有>? (&<country>:des_country|&<province>:des_province|&<city>:des_city|&<area>:des_area|&<site>:des_site|&<station>:des_station) 的?(呢|啊|呀|吗|还?有吗|还?有没有)?$ => request() <0.9>;

export exp51 = ^<有>? <去> (&<country>:des_country|&<province>:des_province|&<city>:des_city|&<area>:des_area|&<site>:des_site|&<station>:des_station) (<时间>)? <出发>? 从?(&<country>:dep_country|&<province>:dep_province|&<city>:dep_city|&<area>:dep_area|&<site>:dep_site|&<station>:dep_station)? <出发>?  的? (<车次类型>:train_type|<航空公司>:airline)? 的? (<席别>:level)?的?(呢|啊|呀|吗|还?有吗|还?有没有)?$ => request(time="$2") <0.9>;

export exp52 = <搜索> <去> (&<country>:des_country|&<province>:des_province|&<city>:des_city|&<area>:des_area|&<site>:des_site|&<station>:des_station) (<时间>)? <出发>? 从?(&<country>:dep_country|&<province>:dep_province|&<city>:dep_city|&<area>:dep_area|&<site>:dep_site|&<station>:dep_station)? <出发>?  的? (<车次类型>:train_type|<航空公司>:airline)? 的? (<席别>:level)?的?(呢|啊|呀|吗|还?有吗|还?有没有)?$ => request(time="$2") <0.9>;

	### 目的地+出发地?+时间?
export exp61 = ^<有>? <去> (&<country>:des_country|&<province>:des_province|&<city>:des_city|&<area>:des_area|&<site>:des_site|&<station>:des_station) 从? (&<country>:dep_country|&<province>:dep_province|&<city>:dep_city|&<area>:dep_area|&<site>:dep_site|&<station>:dep_station)? <出发>? (<时间>)? <出发>? 的? (<车次类型>:train_type|<航空公司>:airline)? 的? (<席别>:level)?的?(呢|啊|呀|吗|还?有吗|还?有没有)?$ => request(time="$3") <0.9>;

export exp62 = <搜索> <去> (&<country>:des_country|&<province>:des_province|&<city>:des_city|&<area>:des_area|&<site>:des_site|&<station>:des_station) 从? (&<country>:dep_country|&<province>:dep_province|&<city>:dep_city|&<area>:dep_area|&<site>:dep_site|&<station>:dep_station)? <出发>? (<时间>)? <出发>? 的? (<车次类型>:train_type|<航空公司>:airline)? 的? (<席别>:level)?的?(呢|啊|呀|吗|还?有吗|还?有没有)?$ => request(time="$3") <0.9>;

#================================================

###### 以订票种类开头
export exp71 = ^<有>? (<订票种类>:type) (<车次类型>:train_type|<航空公司>:airline)? 的? (<席别>:level)? 的? (啊|呀|吗|有吗|还有吗|还有没有)?$ => request() <0.85>;
export exp72 = <搜索> (<订票种类>:type) (<车次类型>:train_type|<航空公司>:airline)? 的? (<席别>:level)? 的? (啊|呀|吗|有吗|还有吗|还有没有)?$ => request() <0.85>;

###### 以车次类型开头
export exp81 = ^<有>? (<车次类型>:train_type|<航空公司>:airline) 的? (<席别>:level)? 的? (<订票种类>:type) 的?(啊|呀|吗|有吗|还有吗|还有没有)?$ => request() <0.85>;
export exp82 = <搜索> (<车次类型>:train_type|<航空公司>:airline) 的? (<席别>:level)? 的? (<订票种类>:type) 的?(啊|呀|吗|有吗|还有吗|还有没有)?$ => request() <0.85>;

###### 以席别开头
export exp91 = ^<有>? (<席别>:level) 的? (啊|呀|吗|有吗|还有吗|还有没有)?$ => request() <0.85>;
export exp92 = <搜索> (<席别>:level) 的? (啊|呀|吗|有吗|还有吗|还有没有)?$ => request() <0.85>;

###### 车次/航班号开头
export exp101 = ^<有>? (<火车车次>:number) 的? (这|那)?趟?的?(啊|呀|吗|有吗|还有吗|还有没有)?$ => request() <0.85>;
export exp102 = <搜索> (<火车车次>:number) 的? (这|那)?趟?的?(啊|呀|吗|有吗|还有吗|还有没有)?$ => request() <0.85>;

###### 车票价格开头
export exp111 = ^<有>? (价格)? (低于|少于|不高于|不大于) (<数字>+) (块钱?|元|RMB|人民币)? 的?(啊|呀|吗|有吗|还有吗|还有没有)?$ => request(priceMax="$2") <0.85>;
export exp112 = <搜索>? (价格)? (低于|少于|不高于|不大于) (<数字>+) (块钱?|元|RMB|人民币)? 的?(啊|呀|吗|有吗|还有吗|还有没有)?$ => request(priceMax="$2") <0.85>;

export exp121 = ^<有>? (价格)? (高于|多于|不低于|不小于) (<数字>+) (块钱?|元|RMB|人民币)? 的?(啊|呀|吗|有吗|还有吗|还有没有)?$ => request(priceMin="$2") <0.85>;
export exp122 = <搜索> (价格)? (高于|多于|不低于|不小于) (<数字>+) (块钱?|元|RMB|人民币)? 的?(啊|呀|吗|有吗|还有吗|还有没有)?$ => request(priceMin="$2") <0.85>;

###### 车次开头
export exp131 = ^<有>? (<车次>:number) 的?(啊|呀|吗|有吗|还有吗|还有没有)?$ => request() <0.85>;
export exp132 = <搜索> (<车次>:number) 的?(啊|呀|吗|有吗|还有吗|还有没有)?$ => request() <0.85>;
