唱		 = 唱|演唱|演奏|弹奏;
操作     = <播放>;
一首	 = (一|几)?(首|个|段|下|些|点);
播放	 = 播|放|唱|听|播放|点播|(来<一首>);

export inform50 = <操作><一首>?(&<music_artist>)(<唱>?的)?(&<music_title>) => request(artist="$1", title="$3") <0.4>;

export inform52 = ^(&<music_artist>)<唱>?的(&<music_title>)			 => request(artist="$1", title="$2") <0.4>;

# export exp = (&<music_artist>)的(&<music_title>) => request(artist="$1", title="$2") <0.9>;
