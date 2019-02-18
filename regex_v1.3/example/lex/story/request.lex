独立故事类别   = 神话|童话|寓言|传说;
非独立故事类别 = 成语;

###### [<我想知道>] <故事名> <询问故事作者名> [<结尾礼貌>]
###### 告诉我美人鱼诗谁写的啊
export request00 = (&<story_title>)(是|由)?(谁|哪个|哪位)(人|作者|作家)?所?(写|创作) => request(title="$1", intent="request") <0.8>;
export request01 = (&<story_title>)的?(作者|作家)是(谁|哪个|哪位) => request(title="$1", intent="request") <0.8>;


###### [<我想知道>] <这个故事> <询问故事作者名> [<结尾礼貌>]
###### 你知道这个故事是谁写的吗
export request10 = (这|那)(个|则)?(&<独立故事类别>)故事?(是|由)?(谁|哪个|哪位)(人|作者|作家)?所?(写|创作) => request(tag="$3", intent="request") <0.6>;
export request11 = (这|那)(个|则)?(&<非独立故事类别>)?故事(是|由)?(谁|哪个|哪位)(人|作者|作家)?所?(写|创作) => request(tag="$3", intent="request") <0.6>;
export request12 = (这|那)(个|则)?(&<独立故事类别>)故事?的?(作者|作家)是(谁|哪个|哪位) => request(tag="$3", intent="request") <0.6>;
export request13 = (这|那)(个|则)?(&<非独立故事类别>)?故事的?(作者|作家)是(谁|哪个|哪位) => request(tag="$3", intent="request") <0.6>;


###### [<我想知道>] <这个故事> <询问故事名> [<结尾礼貌>]
###### 这个故事叫什么
export request20 = (这|那)(个|则)?(&<独立故事类别>)故事?的?(名|名字|故事名)?(是|叫)(啥|什么) => request(tag="$3", intent="request") <0.6>;
export request21 = (这|那)(个|则)?(&<非独立故事类别>)故事的?(名|名字|故事名)?(是|叫)(啥|什么) => request(tag="$3", intent="request") <0.6>;
export request22 = (这|那)(个|则)?(&<独立故事类别>)故事?(是|叫)(啥|什么)(名|名字|故事名)? => request(tag="$3", intent="request") <0.6>;
export request23 = (这|那)(个|则)?(&<非独立故事类别>)故事(是|叫)(啥|什么)(名|名字|故事名)? => request(tag="$3", intent="request") <0.6>;
