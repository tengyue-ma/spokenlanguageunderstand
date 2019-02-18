#include <iostream>
#include <cstring>
#include <stdexcept>
#include <fstream>

#include "utils.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;

void Utils::lstrip(const char* pstr, unsigned& index){
	while(pstr[index]!='\0' && (' '==pstr[index] || '\t'==pstr[index]))
		index++;
}

void Utils::rstrip(const char* pstr, unsigned& index){
	while(index>=0 && (' '==pstr[index] || '\t'==pstr[index]))
		index--;
}

void Utils::strip(const char* pstr, unsigned& idx1, unsigned& idx2){
	Utils::lstrip(pstr, idx1);
	Utils::rstrip(pstr, idx2);
}

string Utils::strip(const string& str){
	size_t i=0;
	for(; i<str.size() && (str[i]==' ' || str[i]=='\t'); i++);
	size_t j=str.size()-1;
	for(; j>=0 && (str[j]==' ' || str[j]=='\t'); j--);
	if(i>j)
		return "";
	else
		return str.substr(i, j-i+1);
}

void Utils::split(const char* pstr, vector<string>& strs, const char* sep){
	strs.clear();
	string tmp = "";

	if(NULL!=sep){
		int sep_len = strlen(sep);
		int str_len = strlen(pstr);
		for(int i=0; i<str_len;){
			if(strncmp(pstr+i, sep, sep_len)==0){
				if(tmp.size()>0)
					strs.push_back(tmp);
				tmp = "";
				i += sep_len;
			}else{
				tmp += pstr[i++];
			}
		}
		if(tmp.size()>0)
			strs.push_back(tmp);
	}else{
		int str_len = strlen(pstr);
		for(int i=0; i<str_len;){
			if(' '==pstr[i] || '\t'==pstr[i]){
				if(tmp.size()>0)
					strs.push_back(tmp);
				tmp = "";
				i += 1;
			}else{
				tmp += pstr[i++];
			}
		}
		if(tmp.size()>0)
			strs.push_back(tmp);
	}
}

int Utils::gbkstrs(const char* query, vector<string>& split_strs){
	split_strs.clear();
	int len = strlen(query);
	int n = 1;
    for(int idx=0; query[idx]!='\0';){
        if(query[idx]>0)
			n = 1;
		else
			n = 2;
		if(idx+n>len)
			break;
		string str(query, idx, n);
		idx += n;
		if(str.length() == 0) {continue;}
		split_strs.push_back(str);
    }
    return 0;
}

int Utils::utf8strs(const char* str, vector<string>& split_strs){
	split_strs.clear();
    int n=1;
    for(int i=0; str[i]!='\0';){
		//< binary operation
        char tmp = ~str[i];
		//< one byte
        if(tmp<0){
            n = 1;
		//< six bytes
        }else if(tmp<4){
            n = 6;
		//< five bytes
        }else if(tmp<8){
            n = 5;
		//< four bytes
        }else if(tmp<16){
            n = 4;
		//< three bytes
        }else if(tmp<32){
            n = 3;
		//< two bytes
        }else{
            n = 2;
        }
        string s(str, i, n); 
        split_strs.push_back(s);
        i += n;
    }   
    return 0;
}

template<typename T>
void Utils::extend(vector<T>& v, const vector<T>& v1){
	for(size_t i=0; i<v1.size(); i++){
		v.push_back(v1[i]);
	}
}

unsigned Utils::str2uint(const string& str){
	unsigned number = 0;
	unsigned base = 1;
	for(int i=str.size()-1; i>=0; i--){
		if(str[i]>=48 && str[i]<=57)
			number += base * (str[i] - 48);
		else
			throw std::runtime_error("expression illegal!");
		base *= 10;
	}
	return number;
}

string Utils::uint2str(unsigned number){
	string str = "";
	while(number!=0){
		char n = static_cast<char>(number%10 + 48);
		str = n + str;
		number = number/10;
	}
	return str;
}

string Utils::join(const vector<string>& strs, int start, int end){
	string s = "";
	if(end == -1 || end>strs.size())
		end = strs.size();
	for(size_t i=start; i<end; i++){
		s += strs[i];
	}
	return s;
}

void Utils::replace(string& str, char subc, char repc){
	for(size_t i=0; i<str.size(); i++){
		if(str[i] == subc)
			str[i] = repc;
	}
}

