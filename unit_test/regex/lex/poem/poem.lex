
export inform = (播放)?(李白|杜甫)的?(春晓) => request(author="$2", title="$3") <0.2>;
export inform = (播放)?(周杰伦)的?(青花瓷) => request(artist="$2", title="$3", domain="music") <0.2>;
