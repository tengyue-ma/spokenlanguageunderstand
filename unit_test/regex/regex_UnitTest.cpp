/*************************************************************************
	> File Name: regex_UnitTest.cpp
	> Author: leilei.zhou
	> Mail: leilei.zhou@horizon.ai
	> Created Time: 2018.12.17
 ************************************************************************/


#define CATCH_CONFIG_MAIN  
#include "catch.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <map>
#include <vector>
#include <string>
#include <ctime>
#include <stdexcept>
#include <pthread.h>

#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>

#include <algorithm>
#include <set>
#include <queue>

#include "logger.h"
#include "comm_def.h"
#include "spregex.h"

#include "utils.h"
#include "nfa.h"
#include "group.h"
#include "domain_lex.h"
#include "expression.h"
#include "variable.h"
#include "thread_pool.h"
#include "semantic.h"
#include "link.h"

using namespace std;
using namespace hobotlog;
using namespace SemanticRegex;
using namespace Utils;

extern NFA* build_NFA(const char*, map<string, Variable*>&, bool expression);

TEST_CASE ("regex", "regex") {
    
    LogModule *loginstance = new LogModule("../conf/log.conf");
	Regex *parser = new Regex("regex.cfg", loginstance);
    
	//测试文件：spregex.cc
	SECTION("测试析构函数, log文件存在"){
	    REQUIRE_NOTHROW(delete parser);
    }
	SECTION("测试构造函数和析构函数, log文件不存在"){
		LogModule *loginstance = new LogModule("");
		Regex *parser = new Regex("./regex.cfg", loginstance);
	    REQUIRE_NOTHROW(delete parser);
    }
	SECTION("测试构造函数和析构函数, regex.cfg有重复"){
		LogModule *loginstance = new LogModule("../conf/log.conf");
		Regex *parser = new Regex("./regex_redeclared.cfg", loginstance);
	    REQUIRE_NOTHROW(delete parser);
    }
    SECTION("use_domain为空, utf8格式, n_best = 2, 周杰伦的青花瓷"){
		vector<string> sems = parser->parse("周杰伦的青花瓷", "", 2);
		REQUIRE(sems[0] == "{\"query\": \"周杰伦的青花瓷\", \"domain\": \"music\", \"intent\": \"inform\", \"slots\": {\"artist\": \"周杰伦[0, 3]\", \"title\": \"青花瓷[4, 7]\"}, \"weight\": \"0.900000\"}");
		REQUIRE(sems[1] == "{\"query\": \"周杰伦的青花瓷\", \"domain\": \"music\", \"intent\": \"inform\", \"slots\": {\"artist\": \"周杰伦[0, 3]\", \"title\": \"青花瓷[4, 7]\"}, \"weight\": \"0.600000\"}");
    }
	SECTION("use_domain为空, utf8格式, n_best = -1, 周杰伦的青花瓷"){
		vector<string> sems = parser->parse("周杰伦的青花瓷", "", -1);
		REQUIRE(sems[0] == "{\"query\": \"周杰伦的青花瓷\", \"domain\": \"music\", \"intent\": \"inform\", \"slots\": {\"artist\": \"周杰伦[0, 3]\", \"title\": \"青花瓷[4, 7]\"}, \"weight\": \"0.900000\"}");
    }
	SECTION("use_domain为music, utf8格式, n_best = 1, 周杰伦的青花瓷"){
		vector<string> sems = parser->parse("周杰伦的青花瓷", "music", 1);
		REQUIRE(sems[0] == "{\"query\": \"周杰伦的青花瓷\", \"domain\": \"music\", \"intent\": \"inform\", \"slots\": {\"artist\": \"周杰伦[0, 3]\", \"title\": \"青花瓷[4, 7]\"}, \"weight\": \"0.900000\"}");
    }
	SECTION("use_domain为notexist, utf8格式, n_best = 1, 周杰伦的青花瓷"){
		vector<string> sems = parser->parse("周杰伦的青花瓷", "notexist", 1);
		REQUIRE(sems.size() == 0);
    }
	SECTION("use_domain为空, gbk格式, n_best = 1, 周杰伦的青花瓷"){
		//读文件
		string line;
		ifstream inf("test.gbk");
		if(!inf){
			std::cerr << ">>>>>> ERROR: Cannot open input file!" << endl;
		}
		while(getline(inf, line)){
			vector<string> sems;
			vector<Semantic*> results = parser->match(line, "", 1, true);
			for(size_t i=0; i<results.size(); i++){
				sems.push_back(results[i]->dumps());
			}
			REQUIRE(sems[0] == "{\"query\": \"周杰伦的青花瓷\", \"domain\": \"music\", \"intent\": \"inform\", \"slots\": {\"artist\": \"周杰伦[0, 3]\", \"title\": \"青花瓷[4, 7]\"}, \"weight\": \"0.900000\"}");
		}
	}
	
	//测试文件: domain_lex.cc
	
	
	
	
	//测试文件：nfa.cc
	SECTION("测试构造函数, 无传入参数"){
		NFA *pnfa = new NFA();
	    REQUIRE_NOTHROW(delete pnfa);
    }
	//SECTION("cout重载, 输出group"){
	//	Group* group = new Group();
	//	//*out << group;
	//}
	//SECTION("print nfa"){
	//	string line = "";
	//	map<string, Variable*> map_var_;
	//	size_t i = 0;
	//	vector<string> split_line;
	//	utf8strs(line.c_str(), split_line);
    //
	//	string expr_name;
	//	while(split_line[i] != " ") ++i;
	//	while(split_line[i] != "="){
	//		if(split_line[i] == " "){
	//			++i;
	//			continue;
	//		}else{
	//			expr_name += split_line[i];
	//			++i;
	//		}
	//	}++i;
	//	
	//	string expr_value;
	//	while(split_line[i] != "="){
	//	    if(split_line[i] == " "){
	//	        ++i;
	//	        continue;
	//	    }else{
	//	        expr_value += split_line[i];
	//	        ++i;
	//	    }
	//	}i+=2;
	//	
	//	
	//	//NFA* pnfa = build_NFA(expr_value.c_str(), map_var_, true);
	//    //pnfa->print("nfa.dot");
    //}
	
	
	//测试文件：utils.cc
	SECTION("strip函数"){
		string str = "     来一个周杰伦的歌     ";
		unsigned idx1 = 0;
		unsigned idx2 = str.size()-1;
		Utils::strip(str.c_str(), idx1, idx2);
		REQUIRE(idx1 == 5);
		REQUIRE(idx2 == 28);
	}
	SECTION("split函数, 分隔符为非空"){
		vector<string> strs;
		Utils::split("周杰伦/的/歌", strs, "/");
		REQUIRE(strs[0] == "周杰伦");
		REQUIRE(strs[1] == "的");
		REQUIRE(strs[2] == "歌");
	}
	SECTION("split函数, 分隔符为空"){
		vector<string> strs;
		Utils::split("周杰伦 的 歌", strs, NULL);
		REQUIRE(strs[0] == "周杰伦");
		REQUIRE(strs[1] == "的");
		REQUIRE(strs[2] == "歌");
	}
	SECTION("gbkstrs函数"){
		string line;
		ifstream inf("test.gbk");
		if(!inf){
			std::cerr << ">>>>>> ERROR: Cannot open input file!" << endl;
		}
		while(getline(inf, line)){
			vector<string> strs;
			Utils::gbkstrs(line.c_str(), strs);
		}
	}
	SECTION("utf8strs函数"){
		vector<string> strs;
		Utils::utf8strs("周杰伦的青花瓷圪巾愚", strs);
		REQUIRE(strs[0] == "周");
		REQUIRE(strs[1] == "杰");
		REQUIRE(strs[2] == "伦");
		REQUIRE(strs[3] == "的");
		REQUIRE(strs[4] == "青");
		REQUIRE(strs[5] == "花");
		REQUIRE(strs[6] == "瓷");
		REQUIRE(strs[7] == "圪");
		REQUIRE(strs[8] == "巾");
		REQUIRE(strs[9] == "愚");
	}
	
	//测试文件：semantic.cc
	SECTION("构造函数, 输入参数为空"){
		Slot* slot = new Slot();
		REQUIRE_NOTHROW(delete slot);
	}
	SECTION("构造函数, 输入参数为Slot类型"){
		Slot my_slot;
		my_slot.start = 0;
		my_slot.end = 10;
		my_slot.name = "artist";
		my_slot.value = "周杰伦";
		Slot* slot = new Slot(my_slot);
		REQUIRE(slot->start == 0);
		REQUIRE(slot->end == 10);
		REQUIRE(slot->name == "artist");
		REQUIRE(slot->value == "周杰伦");
		REQUIRE_NOTHROW(delete slot);
	}
	SECTION("重载 =="){
		Slot slot1, slot2, slot3, slot4, slot5, slot6;
		slot1.start = 0;
		slot1.end = 10;
		slot1.name = "artist";
		slot1.value = "周杰伦";
		
		slot2.start = 1;
		slot2.end = 10;
		slot2.name = "artist";
		slot2.value = "周杰伦";
		
		slot3.start = 0;
		slot3.end = 11;
		slot3.name = "artist";
		slot3.value = "周杰伦";
		
		slot4.start = 0;
		slot4.end = 10;
		slot4.name = "artist1";
		slot4.value = "周杰伦";
		
		slot5.start = 0;
		slot5.end = 10;
		slot5.name = "artist";
		slot5.value = "周杰伦1";
		
		slot6.start = 0;
		slot6.end = 10;
		slot6.name = "artist";
		slot6.value = "周杰伦";
		
		REQUIRE((slot1 == slot2) == false);
		REQUIRE((slot1 == slot3) == false);
		REQUIRE((slot1 == slot4) == false);
		REQUIRE((slot1 == slot5) == false);
		REQUIRE((slot1 == slot6) == true);
	}
	SECTION("重载 =, add_slot()函数, has_slot()函数"){
		Semantic sem ;
		sem.query = "周杰伦的歌";
		sem.intent = "inform";
		sem.domain = "music";
		sem.match_len = 10;
		Slot* slot = new Slot();
		slot->start = 0;
		slot->end = 10;
		slot->name = "artist";
		slot->value = "周杰伦";
		sem.add_slot(slot);
		Semantic my_sem = sem;
		int idx = sem.has_slot("artist");
		REQUIRE(my_sem.query == "周杰伦的歌");
		REQUIRE(my_sem.intent == "inform");
		REQUIRE(my_sem.domain == "music");
		REQUIRE(my_sem.match_len == 10);
		REQUIRE(my_sem.slots[0]->start == 0);
		REQUIRE(my_sem.slots[0]->end == 10);
		REQUIRE(my_sem.slots[0]->name == "artist");
		REQUIRE(my_sem.slots[0]->value == "周杰伦");
		REQUIRE(idx == 0);
	}
}
