#include "variable.h"

using std::string;

namespace SemanticRegex{

Variable::Variable(const string& var_name, const string& var_value) {
    this->var_name_ = var_name;
    this->var_value_ = var_value;
    this->pnfa_ = NULL;
}

Variable::~Variable() {
    this->pnfa_->destruct();
}

}
