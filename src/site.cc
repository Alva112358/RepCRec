//
// Created by 梁俊华 on 11/23/22.
//

#include "site.h"

repcrec::site_id_t Site::increase_id_ = 1;

Site::Site() : id_(increase_id_), is_available_(true) {
    ++increase_id_;
}

bool Site::is_available() const {
    return is_available_;
}

repcrec::site_id_t Site::get_site_id() const {
    return id_;
}

void Site::dump() const {
    printf("site %d - ", id_);
    for (auto iter = variables_.begin(); iter != variables_.end(); ++iter) {
        if (iter == prev(end(variables_))) {
            printf("x%d: %d", iter->second->get_id(), iter->second->get_value());
        } else {
            printf("x%d: %d ", iter->second->get_id(), iter->second->get_value());
        }
    }
    printf("\n");
}

std::shared_ptr<Variable> Site::get_variable(repcrec::var_id_t var_id) {
    return variables_.contains(var_id) ? variables_[var_id] : nullptr;
}

void Site::assign_var(repcrec::var_id_t var_id, repcrec::var_t value) {
    if (!variables_.count(var_id)) {
        variables_[var_id] = std::make_shared<Variable>(var_id);
        variables_[var_id]->set_value(value);
    } else {
        variables_[var_id]->set_value(value);
    }
}

void Site::set_available() {
    is_available_ = true;
}

void Site::set_unavailable() {
    is_available_ = false;
}
