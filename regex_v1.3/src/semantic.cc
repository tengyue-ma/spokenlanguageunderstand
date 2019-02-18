#include <stdio.h>
#include <map>

#include "semantic.h"

using std::map;

namespace SemanticRegex{

Slot::Slot(){
	start = -1;
	end   = -1;
}
Slot::Slot(const Slot& slot){
	start = slot.start;
	end   = slot.end;
	name  = slot.name;
	value = slot.value;
}
Slot::~Slot(){
}

Slot::Slot(string name, string value, int start, int end){
	this->start = start;
	this->end   = end;

	this->name  = name;
	this->value = value;
}

bool Slot::operator==(const Slot& s){
	if(start != s.start)
		return false;
	if(end != s.end)
		return false;
	if(name != s.name)
		return false;
	if(value != s.value)
		return false;
	return true;
}


Semantic::Semantic(){
	weight = 0.0;
}
Semantic::~Semantic(){
	for(size_t i=0; i<slots.size(); i++){
		delete slots[i];
	}
	slots.clear();
}

Semantic& Semantic::operator=(const Semantic& sem){
	this->query 	= sem.query;
	this->intent 	= sem.intent;
	this->domain	= sem.domain;
    this->match_len = sem.match_len;
	for(size_t i=0; i<sem.slots.size(); i++){
		add_slot(new Slot(*(sem.slots[i])));
	}
	return *this;
}

bool Semantic::operator==(const Semantic& sem){
	if(this->query != sem.query || this->domain !=sem.domain || this->intent != sem.intent)
		return false;
	map<string, string> map1;
	map<string, string> map2;
	for(size_t i=0; i<slots.size(); i++){
		map1[slots[i]->name] = slots[i]->value;
	}
	for(size_t i=0; i<sem.slots.size(); i++){
		map2[sem.slots[i]->name] = sem.slots[i]->value;
	}

	if(map1.size() != map2.size())
		return false;
	for(map<string, string>::iterator it=map1.begin();
		it!=map1.end(); it++){
		if(map2.find(it->first) == map2.end() || map2[it->first] != it->second)
			return false;
	}
	return true;
}

void Semantic::add_slot(Slot* slot){
	slots.push_back(slot);
}

void Semantic::add_slot(string name, string value, int start, int end){
	Slot* slot = new Slot(name, value, start, end);
	slots.push_back(slot);
}

string Semantic::dumps(){
	char tmp[30];
	string sem_res = "{";
	sem_res += "\"query\": \""  + query + "\", " ;
	sem_res += "\"domain\": \"" + domain + "\", ";
	sem_res += "\"intent\": \"" + intent + "\", ";
	sem_res += "\"slots\": {";
	for(size_t i=0; i<slots.size(); i++){
		sem_res += "\"" + slots[i]->name + "\": \"" + slots[i]->value;
		sprintf(tmp, "[%d, %d]\", ", slots[i]->start, slots[i]->end);
		sem_res += tmp;
	}
	if(sem_res[sem_res.size()-1] == ' ')
		sem_res = sem_res.substr(0, sem_res.size()-2);
	sem_res += "}, ";
	sprintf(tmp, "%f", weight);
	sem_res = sem_res + "\"weight\": \"" + tmp + "\"";
	sem_res += "}";
	return sem_res;
}

int Semantic::has_slot(string name){
	int idx = -1;
	for(size_t i=0; i<slots.size(); i++){
		if(slots[i]->name == name){
			idx = i;
			break;
		}
	}
	return idx;
}

}	// end of namespace SemanticRegex

