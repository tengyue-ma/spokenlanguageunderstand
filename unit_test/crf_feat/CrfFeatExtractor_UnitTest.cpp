/*************************************************************************
	> File Name: CrfFeatExtractor_UnitTest.cpp
	> Author: leilei.zhou
	> Mail: leilei.zhou@horizon.ai
	> Created Time: 2018.12.06
 ************************************************************************/


#define CATCH_CONFIG_MAIN  
#include "catch.hpp"
#include <iostream>
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include "Feature.h"
#include "FeatExtractor.h"
#include "PreProcessor.h"
#include "CrfFeatExtractor.h"
using namespace std;
using namespace feat_extractor;

extern "C" FeatExtractor* create_feat_extr(const string confpath,PreProcessor* pre);
extern "C" void delete_feat_extr(FeatExtractor* feat_extr);

TEST_CASE ("FeatExtractor", "crf_feat") {
    
    //< 初始化ltp
	PreProcessor* pre = new PreProcessor();
    pre->create_wsg_engine("../../data/model/ltp_model/cws.model");
    pre->create_pos_engine("../../data/model/ltp_model/pos.model");
    pre->create_ner_engine("../../data/model/ltp_model/ner.model");
    pre->create_pas_engine("../../data/model/ltp_model/parser.model");
	
	int ret = 0;
	string query = "";
	feature::Feature feat;
    string conf1 = "music_dict1.conf";
	string conf2 = "music_dict2.conf";
	CrfFeatExtractor* crf_feat_extr1 = new CrfFeatExtractor(conf1, pre);
	CrfFeatExtractor* crf_feat_extr2 = new CrfFeatExtractor(conf2, pre);
	
    SECTION("测试create_feat_extr 和 delete_feat_extr"){
		FeatExtractor* feat_ext = create_feat_extr(conf1, pre);
        REQUIRE_NOTHROW(create_feat_extr(conf1, pre));
		REQUIRE_NOTHROW(delete_feat_extr(feat_ext));
    }
	
	SECTION("测试构造函数和析构函数"){
        CrfFeatExtractor* crf_feat_extr_test = new CrfFeatExtractor(conf1, pre);
        REQUIRE_NOTHROW(delete crf_feat_extr_test);
    }
	
	SECTION("feat1, query为空"){
        ret = crf_feat_extr1->get_feat(feat, "");
		REQUIRE(ret == 0);
    }
	SECTION("feat1, 有s标签, 张国荣唱的我这首歌"){
		string result = "";
        ret = crf_feat_extr1->get_feat(feat, "张国荣唱的我这首歌");
		for (int i = 0; i < feat.feats.size(); ++i){
			result += feat.feats[i] + "/";
		}
		REQUIRE(ret == 0);
		REQUIRE(result == "张 artist+b O/国 artist+i O/荣 artist+e O/唱 O O/的 O O/我 O title+s/这 O O/首 O O/歌 O O/");
    }
	
    SECTION("feat2, 词典个数为0, 张国荣唱的我这首歌"){
		string result = "";
        ret = crf_feat_extr2->get_feat(feat, "张国荣唱的我这首歌");
		for (int i = 0; i < feat.feats.size(); ++i){
			result += feat.feats[i] + "/";
		}
		REQUIRE(ret == 0);
		REQUIRE(result == "张 O/国 O/荣 O/唱 O/的 O/我 O/这 O/首 O/歌 O/");
    }
	
	//测试Trie_tree.cpp
	SECTION("测试构造函数和析构函数, 词典路径为空"){
        TrieTree* trie_tree = new TrieTree("");
        REQUIRE_NOTHROW(delete trie_tree);
    }
	SECTION("测试构造函数和析构函数, 词典路径不存在"){
		TrieTree* trie_tree = new TrieTree("notexist.dict");
        REQUIRE_NOTHROW(delete trie_tree);

    }
	SECTION("测试构造函数和析构函数, 词典路径存在"){
		TrieTree* trie_tree = new TrieTree("artist.dict");
        REQUIRE_NOTHROW(delete trie_tree);

    }
	SECTION("测试构造函数和析构函数, 词典路径为vector, 且vector为空"){
		vector<string> filepaths;
		TrieTree* trie_tree = new TrieTree(filepaths);
        REQUIRE_NOTHROW(delete trie_tree);

    }
	SECTION("测试构造函数和析构函数, 词典路径为vector, 且vector不为空, 路径无效"){
		vector<string> filepaths;
		filepaths.push_back("");
		TrieTree* trie_tree = new TrieTree(filepaths);
        REQUIRE_NOTHROW(delete trie_tree);

    }
	SECTION("测试构造函数和析构函数, 词典路径为vector, 且vector不为空"){
		vector<string> filepaths;
		filepaths.push_back("artist.dict");
		filepaths.push_back("title.dict");
		TrieTree* trie_tree = new TrieTree(filepaths);
        REQUIRE_NOTHROW(delete trie_tree);

    }
	SECTION("词典路径为空, match()函数"){
        TrieTree* trie_tree = new TrieTree("");
		vector<string> seq_vec;
		seq_vec.push_back("周");
		ret = trie_tree->match(seq_vec, 0);
		REQUIRE(ret == 0);
        REQUIRE_NOTHROW(delete trie_tree);
    }
	SECTION("has_dict()函数, 词典不为空"){
        TrieTree* trie_tree = new TrieTree("artist.dict");
		ret = trie_tree->has_dict("周杰伦");
		REQUIRE(ret == 1);
		ret = trie_tree->has_dict("咻咻咻");
		REQUIRE(ret == 0);
        REQUIRE_NOTHROW(delete trie_tree);
    }
	SECTION("has_dict()函数, 词典为空"){
        TrieTree* trie_tree = new TrieTree("");
		ret = trie_tree->has_dict("周杰伦");
		REQUIRE(ret == 0);
        REQUIRE_NOTHROW(delete trie_tree);
    }
}
