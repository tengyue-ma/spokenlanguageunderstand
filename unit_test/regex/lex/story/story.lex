
export inform = (播放)?(安徒生|格林)的?(白雪公主|灰姑娘) => request(author="$2", title="$3") <0.3>;
export inform = (播放)?(周杰伦)的?(青花瓷) => request(artist="$2", title="$3", domain="music") <0.3>;
