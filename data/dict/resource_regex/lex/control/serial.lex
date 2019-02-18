###################################################################
# 版本迭代记录                                                    #
# zhikai.zhang 2018.08.30                                         # 
# zhikai.zhang 2018.10.15 支持第一个路线/第一条路线/第一个联系人  #
# leilei.zhou  2019.01.01 type -> count                           #
###################################################################

unit = 个|条;
first = 第1<unit>|第一<unit>|1|一|第1|第一;
second = 第2<unit>|第二<unit>|2|二|第2|第二;
third = 第3<unit>|第三<unit>|3|三|第3|第三;
前缀 = ((帮我|替我|给我|我)?(选|选择))|就|去|打;
后缀 = 吧|好了|号码|联系人|路线;
export serial = ^<前缀>?<first><后缀>?$ => request(intent="serial", count="first") <5.6>;
export serial = ^<前缀>?<second><后缀>?$ => request(intent="serial", count="second") <5.6>;
export serial = ^<前缀>?<third><后缀>?$ => request(intent="serial", count="third") <5.6>;
