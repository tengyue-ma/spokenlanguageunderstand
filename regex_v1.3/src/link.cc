#include <stdexcept>

#include "state.h"
#include "charset.h"
#include "link.h"

namespace SemanticRegex{

Link::Link(){
	start = NULL;
	end   = NULL;
	cs	  = NULL;
	ltype = STRING;
}

Link::~Link(){
	if(ltype == STRING && cs!=NULL){
		delete cs;
	}
}


Link::Link(State *start, State *end, CharSet *cs){
	this->start = start;
	this->end	= end;
	this->cs	= cs;
	this->ltype = STRING;

	start->olinks.push_back(this);
	end->ilinks.push_back(this);
	
	start->eflag = false;
	end->sflag = false;
}

Link::Link(State* start, State* end, LinkType lt, unsigned index){
	this->start = start;
	this->end   = end;
	this->cs    = NULL;
	if(lt == STRING)
		throw std::runtime_error("You should"
		" use another constructor for STRING link type!");
	this->ltype = lt;
	if(lt == CAPS || lt == CAPE)
		if(index<=0)
			throw std::runtime_error("A positive"
			" integer must be provided for CAPS and CAPE link type!");
	this->index = index;

	start->olinks.push_back(this);
	end->ilinks.push_back(this);

	start->eflag = false;
	end->sflag = false;
}

void Link::set_start(State* start){
	this->start = start;
	start->olinks.push_back(this);
}

void Link::set_end(State* end){
	this->end = end;
	end->ilinks.push_back(this);
}

}
