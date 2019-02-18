#include "../comm/digit.lex"

#### --------------------通用变量---------------------- ####

我 =  我|俺|大爷|爷|咱|咱们|老娘|本小姐|本大小姐|我们|本人|给朕;

一首 = (一|几)?(首|个|段|下|些);
诗人 = 诗人|作者|人;

作品 = 作品|诗|词;
标题 = 标题|名字|题目;

#### --------------------动词变量---------------------- ####

想 = 想|要|想要;
属于 = 属于|是|算|算是;
可以 = 能|可以;
能不能 = 能不能|可不可以|可以不可以|能不能够|能否|可否;
创作 = (写|创作)过?的?;

#### --------------------其他槽位---------------------- ####

朝代 = (夏|商|周|西周|东周|秦|西楚|东晋|西晋|南|北|南北|汉|东汉|西汉|东汉末年|三国|魏|蜀|吴|越|隋|唐|宋|北宋|南宋|西夏|元|明|清|近|近现|现)(代|朝|国)?(时期)?;

#### -------------------辅助变量---------------------- ####

疑问前缀 = <我><想>知道|你知道|(<可以>|<能不能>)?(告诉<我>|给我说(一下|下));

#### -------------------正则规则---------------------- ####

#1a. 问这首诗的作者
export request1a1 = ^<疑问前缀>?(这?<一首>?<作品>?|它)是?(谁|(哪个|什么)<诗人>)(<创作>|的)<作品>?(吗|么|不)?$ => request(intent="request", author="ASK")<0.9>;
export request1a2 = ^<疑问前缀>?是?(谁|(哪个|什么)<诗人>)(<创作>|的)(这<一首>?<作品>?|它)(吗|么|不)?$ => request(intent="request", author="ASK")<0.9>;
export request1a3 = ^<疑问前缀>?(这?<一首>?<作品>?|它)的?(作者)是?(谁|(哪个|什么)<诗人>)?(吗|么|不)?$ => request(intent="request", author="ASK")<0.9>;
export request1a4 = ^<疑问前缀>?(谁|(哪个|什么)<诗人>)是(这?<一首>?<作品>?|它)的?(作者)(吗|么|不)?$ => request(intent="request", author="ASK")<0.9>;

#2a. 问这首诗的朝代(不支持对tag的request)
#export request2a1 = ^<疑问前缀>?(这?<一首>?<作品>?|它)<属于>?(什么|哪|哪个|哪一个)(朝|代|朝代|时期|时候)(<创作>|的)?<作品>?(吗|么|不)?$ => request(intent="request", tag="ASK_CHAODAI")<0.9>;
#export request2a2 = ^<疑问前缀>?(这?<一首>?<作品>?|它)<创作>于(什么|哪|哪个|哪一个)(朝|代|朝代|时期|时候)(吗|么|不)?$ => request(intent="request", tag="ASK_CHAODAI")<0.9>;
#export request2a3 = ^<疑问前缀>?(这?<一首>?<作品>?|它)的(朝|代|朝代|时期|时候)是(什么|哪|哪个|哪一个)(吗|么|不)?$ => request(intent="request", tag="ASK_CHAODAI")<0.9>;

#3a. 问这首诗的名字
export request2a1 = ^<疑问前缀>?(这?<一首>?<作品>?|它)的?<标题>(是|叫)?(什么|啥)?(吗|么|不)?$ => request(intent="request", title="ASK")<0.9>;
export request2a2 = ^<疑问前缀>?(这?<一首>?<作品>?|它)叫(什么|啥)?(吗|么|不)?$ => request(intent="request", title="ASK")<0.9>;
