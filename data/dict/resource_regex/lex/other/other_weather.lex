####################################
#         author:zhangzhikai       #
#         date:2018.7.24           #
####################################


代词 = 你|我|它|他|她|我们|你们|他们|她们;
喜欢 = 喜欢|讨厌|厌恶|憎恨|恨|爱|喜爱;
非常 = 很|有点|非常|超级|特别|极其|超;
语气助词 = 了?(哦|呢|呦|啊|呀|吧|罢|呗|啵|啦|嘞|哩|咧|咯|啰|喽|嘛|哪|呐|哈|噻|了);

export inform = ^<代词>?<非常>?不?<喜欢>(&<loc_city>:loc_city|&<loc_area>:loc_area|&<loc_poi>:loc_poi|&<loc_country>:loc_country|&<loc_province>:loc_province)的?天气<语气助词>?$ => request(intent="other") <1.5>;
export inform = ^<代词>?<非常>?不?<喜欢>(((&<loc_province>:loc_province)的?(&<loc_city>:loc_city))|((&<loc_city>:loc_city)的?(&<loc_area>:loc_area)))的?天气<语气助词>?$ => request(intent="other") <1.5>;

