谁		= 谁|(哪个|哪位)(人|歌手|艺人|歌星|艺术家|音乐家)?;
唱	 	= 唱|演唱|演奏;
歌曲类别= 京剧|评剧|豫剧|越剧|黄梅戏|儿歌;
歌名	= 名|名字|名儿|歌名|歌曲名|歌名儿|歌曲名儿;
什么	= 啥|什么|啥子;

###### 对应原来的music_request0: [<我想知道>] <歌曲名> <询问歌手名> [<结尾礼貌>];
###### 告诉我告白气球是谁唱的
export request00 = (&<music_title>)(是|是由)?<谁><唱>的 => request(title="$1", intent="request") <0.9>;

###### 对应原来的music_request1: [<我想知道>] <这首歌> <询问歌手名> [<结尾礼貌>];
###### 你知道这首歌谁唱的吗?
export request10 = (这|那)(首|个)(歌|歌曲|专辑|音乐)(是|是由)?<谁><唱>的 => request(intent="request") <0.9>;
export request11 = (这|那)(首|个)(<歌曲类别>)(是|是由)?<谁><唱>的 => request(tag="$1", intent="request") <0.9>;

###### 对应原来的music_request2: [<我想知道>] <这首歌> <询问歌曲名> [<结尾礼貌>]
export request20 = (这|那)(首|个)(歌|歌曲|专辑|音乐)的?<歌名>(叫|是)<什么> => request(intent="request") <0.9>;
export request21 = (这|那)(首|个)(歌|歌曲|专辑|音乐)(叫|是)<什么><歌名> => request(intent="request") <0.9>;
export request22 = (这|那)(首|个)(<歌曲类别>)的?<歌名>(叫|是)<什么> => request(intent="request") <0.9>;
export request23 = (这|那)(首|个)(<歌曲类别>)(叫|是)<什么><歌名> => request(intent="request") <0.9>;
