###### 对应原来的music_reqalts0: [<我想知道>] <这个歌手>|<歌手名> <还唱过> <什么歌> [<结尾礼貌>]
###### 他还有什么歌

这那     = 这|那;
歌手     = 人|歌手|艺人|歌星|艺术家|音乐家;
还       = 还有|还;
唱       = 唱|演唱|演奏;
什么     = 什么|哪些|啥|啥子;
歌       = 歌|歌曲|音乐|曲子;
export reqalts00 = <这那>个?<歌手>(他|她|它)?<还><唱>过<什么><歌> => request(intent="reqalts") <0.9>;
export reqalts00 = (&<music_artist>)(<这那>个?<歌手>)?(他|她|它)?<还><唱>过<什么><歌> => request(artist="$1", intent="reqalts") <0.9>;
