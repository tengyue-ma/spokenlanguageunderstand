###################################################################
# 版本迭代记录                                                    #
# zhikai.zhang 2018.08.30                                         # 
# leilei.zhou  2019.01.01 删掉type字段                            #
###################################################################

语气词 = 吧|啊|呀|呐|啦|呢;
export confirm00 = ^(还是)?(给|帮|替)?我?(想|要)?(确定|确认|确定确定|确认确认)(了|好了)?<语气词>?$ => request(intent="confirm") <5.61>;
export confirm01 = ^(好|好的|可以|没问题)了?<语气词>?$ => request(intent="confirm") <5.62>;
export confirm02 = ^(给|帮|替)?我?拨打<语气词>?$ => request(intent="confirm") <5.63>;
export confirm03 = ^(给|帮|替)我(打|拨)<语气词>?$ => request(intent="confirm") <5.64>;
export confirm04 = ^(打|拨)<语气词>$ => request(intent="confirm") <5.65>;
