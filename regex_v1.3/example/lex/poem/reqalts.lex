
我 =  我|俺|大爷|爷|咱|咱们|老娘|本小姐|本大小姐|我们|本人|给朕;
想 = 想|要|想要;
还写过 = 还有|还写过|还创作过;
什么 = 什么|哪些|啥;
诗 = 古诗|诗|词|诗词;
诗词类别 = 唐诗|宋词|元曲|七言律诗|七言绝句|五言律诗|五言绝句;


#0.他还写过什么诗/他还有什么诗
#<poem_reqalts0>= [<我想知道>] <这个诗人>|<诗人> <还写过> <什么诗> [<结尾礼貌>];
export inform00 = (<我><想>知道|你知道|告诉<我>)(&<poem_author>)<还写过><什么><诗>? => request(author="$2")<0.9>;
export inform00 = (<我><想>知道|你知道|告诉<我>)(&<poem_author>)<还写过><什么><诗词类别>? => request(author="$2")<0.9>;

#1.其他未覆盖到的说法，方便快速解bug
#<poem_reqalts1> = 嘻嘻他还有什么诗;
export inform00 = 还有什么诗? => request(domain="poem")<0.9>;

