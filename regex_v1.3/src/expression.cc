#include <vector>
#include <string>
#include <utility>
#include <iostream>

#include "expression.h"
#include "nfa.h"

using std::vector;
using std::string;
using std::pair;
using std::cout;
using std::endl;

namespace SemanticRegex{

Expression::Expression(const string& expr_name,
                       const string& expr_value,
                       const vector< pair<string, size_t> > requests,
                       const map<string, string> requests1,
                       const double& weight,
                       NFA* pnfa) {
    expr_name_ 	= expr_name;
    expr_value_ = expr_value;
    requests_ 	= requests;
    requests1_ 	= requests1;
    weight_ 	= weight;
    pnfa_ 		= pnfa;
}

Expression::~Expression() {
    this->pnfa_->destruct();
}

bool Expression::operator == (const Expression& e2){
    if (this->expr_value_ == e2.expr_value_)
        return true;
    return false;
}

/*
 * 直接调用表达式对应的NFA进行匹配即可
 */
vector<Group> Expression::match(const string& raw_sentence, 
                                const vector<string>& sentence, 
                                map<string, int>* log, 
                                string& matched_string, 
								RedisWrapper *redis_wrapper,
								const vector< vector<size_t> >& vec_idx) {
    return this->pnfa_->match(raw_sentence, sentence, vec_idx, log, matched_string, redis_wrapper);
}

}
