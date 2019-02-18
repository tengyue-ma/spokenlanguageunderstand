
export inform = (告诉我)?(明天|今天)的?(天气) => request(time="$2") <0.5>;
export inform = (播放)?(周杰伦)的?(青花瓷) => request(artist="$2", title="$3", domain="music") <0.5>;
