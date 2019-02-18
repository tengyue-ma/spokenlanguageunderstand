#####################################################################
# 版本迭代记录                                                      #
# zhikai.zhang 2018.08.31                                           # 
# zhikai.zhang 2018.11.13 支持后缀                                  #
# leilei.zhou  2019.01.01 支持X-OS音乐电话相关app / type->opr       #
#####################################################################

前缀 = <我想>|<给我>;
我想 = 我?想?要?;
给我 = 请?(给|帮|替)?我?;
后缀 = 吧|啊|呀|好吗|可以吗;
appname = <地图>|<音乐>|<通讯录>|<电话>;
地图 = 导航|地图|高德|高德地图|百度地图;
音乐 = 音乐|网易云音乐|qq音乐|QQ音乐;
通讯录 = 通讯录|联系人;
电话 = 电话;

app前缀 = 当前;  
app后缀 = 功能|应用|模式|系统;
打开 = (打开|进入|启动|开启|前往)(一下|下)?;
查看 = 查看;
关闭 = (关闭|退出|关掉)(一下|下)?;
停止 = 结束|停止|终止|取消;

export app = ^<前缀>?<打开>(<appname>)<app后缀>?<后缀>?$ => request(intent="app", opr="open", name="$1") <5.6>;
export app = ^<前缀>?把?(<appname>)<app后缀>?<打开><后缀>?$ => request(intent="app", opr="open", name="$1") <5.6>;
export app = ^(导航)功能$ => request(intent="app", opr="open", name="$1") <5.6>;
export app = ^<前缀>?<关闭><app前缀>?(<appname>)<app后缀>?<后缀>?$ => request(intent="app", opr="close", name="$1") <5.6>;
export app = ^<前缀>?把?<app前缀>?(<appname>)<app后缀>?<关闭><后缀>?$ => request(intent="app", opr="close", name="$1") <5.6>;

# 停止/结束导航 ("停止音乐"属于playstate)
export app = ^<前缀>?<停止><app前缀>?(<地图>)<app后缀>?<后缀>?$ => request(intent="app", opr="close", name="$1") <5.6>;

# 查看通讯录
export app = ^<前缀>?<查看>(<通讯录>)<app后缀>?<后缀>?$ => request(intent="app", opr="open", name="$1") <5.6>;
