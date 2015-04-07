//
//  Rule.cpp
//  CFS_Grammar
//
//  Created by Matt on 4/6/15.
//  Copyright (c) 2015 Matt. All rights reserved.
//

#include "Rule.h"
#include <iostream>

Rule::Rule(std::string& rule) {
    size_t delim = rule.find("->");
    if ((delim == std::string::npos) || (rule.length() < 4)) {
        throw std::runtime_error("Invalid production rule: " + rule);
    }
    
    lhs = rule.substr(0, delim);
    rule = rule.substr(delim+2);
    
    for (size_t i=0; i<rule.length(); i++) {
        size_t delim = rule.find('|');
        if (delim != std::string::npos) {
            std::string temp = rule.substr(0, delim);
            rhs.push_back(temp);
            rule = rule.substr(delim+1);
        }
    }
    if (rule.length() > 0) {
        rhs.push_back(rule);
    }
}