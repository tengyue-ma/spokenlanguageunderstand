#>>>>>>>>> 单线程, 跑一遍query.txt
./bin/slu_test -f query.txt -o result.txt

#>>>>>>>>> 单线程, 跑一遍query.txt, 指定workflow的配置为ME分类
./bin/slu_test -w ../data/conf/workflow.conf -f query.txt -o result.txt

#>>>>>>>>> 单线程, 跑一遍query.txt, 指定workflow的配置为cnn分类
./bin/slu_test -w ../data/conf/workflow_nn.conf -f query.txt -o result.txt

#>>>>>>>>> 单线程, 跑一遍query.txt, 5个n_best结果(如果是匹配到规则的情况下, 模型只有一个结果)
#./bin/slu_test -n 5 -f query.txt -o result.txt

#>>>>>>>>> 单线程, 跑一句sentence
#./bin/slu_test -s 我想听青花瓷

#>>>>>>>>> 单线程, 跑一句sentence, 5个n_best结果(如果是匹配到规则的情况下, 模型只有一个结果)
#./bin/slu_test -n 5 -s 唱一首薛之谦的演员吧

#>>>>>>>>> 15个线程, 5个slu, 跑一遍query.txt
#./bin/slu_test -p 15 -u 5 -f query.txt -o result.txt

#>>>>>>>>> 15个线程, 5个slu, 5个n_best结果, 无限循环跑query.txt, 用于系统稳定性测试
#./bin/slu_test -p 15 -u 5 -n 5 -y 1 -f query.txt -o result.txt
