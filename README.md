SLU V2.0
    2018.6.29 by zhouleilei.

1. 编译
    编译前请先./clean.sh
    然后编译: ./build.sh

2. 运行
    运行前请先添加一些必要的动态库:
        source export_so.sh
    然后导入数据库:
        cd import_redis_data
        ./import_all.sh
    然后进入到test/src目录下运行:
        ./test.sh
    即可看到对应的结果和文件

3. 文件结构
    .
    ├── classifier          //< 分类模块
    │   ├── CNN             
    │   ├── LR
    │   ├── LSTM
    │   ├── ME                  //< 最大熵分类器(error code: -200~-299)
    │   └── clf_base            //< 分类接口
    │ 
    ├── data                //< 数据存储
    │   ├── conf                //< 各种配置文件
    │   ├── dict                //< 相关词典
    │   └── model               //< 相关模型(CLF、NER、LTP model)
    │ 
    ├── feat_extractor      //< 特征提取模块
    │   ├── calculator_feat     //< 计算器特征提取
    │   ├── crf_feat            //< crf特征提取(error code: -500~-599)
    │   ├── feat_base           //< 特征提取接口
    │   ├── me_feat             //< 最大熵特征提取(error code: -400~-499)
    │   └── open_domain_feat    //< 开放领域特征提取(error code: -600~-699)
    │ 
    ├── slu_log             //< 日志模块
    │ 
    ├── README.md
    │ 
    ├── slot_filling        //< slot解析模块
    │   ├── calculator          //< 计算器解析
    │   ├── Bi-LSTM
    │   ├── CRF                 //< CRF解析(error code: -300~-399)
    │   ├── LSTM
    │   ├── ner_base            //< NER接口
    │   └── open domain         //< 开放领域解析(error code: -700~-799)
    │ 
    ├── third               //< 第三方的库 && 封装的接口 && 工具包
    │   ├── basic_lib
    │   ├── crf
    │   ├── ltp-3.4.0       
    │   ├── processor
    │   └── utils
    │ 
    ├── work_flow           //< 模块调用流程 && 业务逻辑实现(error code: -100~-199)
    │    ├── include
    │    ├── lib
    │    ├── test
    │    └── test.cpp
    │
    └── import_redis_data   //< 导入词典到redis数据库的工具,在测试之前需要先执行此步骤
    │    ├── dict           //< 词典数据文件
    │    ├── import_all.sh  //< 导入音乐,诗歌,故事三个领域的词典到数据库
    │    ├── import_file_data.sh    //< 导入单个文件到redis数据库中
