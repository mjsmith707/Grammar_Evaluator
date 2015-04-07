//
//  main.cpp
//  CFS_Grammar
//
//  Created by Matt on 4/6/15.
//  Copyright (c) 2015 Matt. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <set>
#include <unordered_set>
#include <queue>
#include "Rule.h"

typedef std::vector<Rule> rule_t;

void evaluate(rule_t& rules, std::set<std::string>& productions, size_t maxlen, std::string input);
bool isTerminal(rule_t& rules, std::string& rhs);
bool checkStart(Rule& start, std::string& input);
std::string production(std::string& lhs, std::string& rhs, std::string input);
inline std::string& ltrim(std::string &s);
inline std::string& rtrim(std::string &s);
inline std::string& trim(std::string &s);
inline std::string& fulltrim(std::string& s);
void makeRule(rule_t& rules, std::string str);
void printResults(std::set<std::string>& productions);
void searchProductions(std::set<std::string>& productions);

struct compare {
    bool operator()(const std::string& first, const std::string& second) {
        return first.size() < second.size();
    }
};

int main(int argc, const char * argv[]) {
    rule_t rules;
    std::set<std::string> productions;
    
    makeRule(rules, "S->abc|aAbc");
    makeRule(rules, "Ab->bA");
    makeRule(rules, "Ac->Bbcc");
    makeRule(rules, "bB->Bb");
    makeRule(rules, "aB->aa|aaA");
   
    for (auto r : rules) {
        if (r.lhs.compare("S") == 0) {
            for (auto s : r.rhs) {
                evaluate(rules, productions, 100, s);
            }
        }
    }

    printResults(productions);
    searchProductions(productions);
    
    return 0;
}

void evaluate(rule_t& rules, std::set<std::string>& productions, size_t maxlen, std::string input) {
    std::queue<std::string> intermediates;
    std::unordered_set<std::string> checked;
    intermediates.push(input);
    
    while (!intermediates.empty()) {
        std::string temp = intermediates.front();
        intermediates.pop();
        if (temp.length() > maxlen) {
            continue;
        }
        for (size_t i=0; (i<rules.size()) && (!isTerminal(rules, temp)); i++) {
            if (i==0 && !checkStart(rules.at(i), temp)) {
                continue;
            }
            for (size_t j=0; j<rules.at(i).rhs.size(); j++) {
                std::string temp2 = production(rules.at(i).lhs, rules.at(i).rhs.at(j), temp);
                temp2 = fulltrim(temp2);
                if ((checked.find(temp2) == checked.end()) && temp.compare(temp2) != 0) {
                    checked.emplace(temp2);
                    i = 0;
                    j = 0;
                    intermediates.push(temp2);
                }
            }
        }
        if (isTerminal(rules, temp)) {
            auto it = productions.find(temp);
            if (it == productions.end()) {
                productions.insert(temp);
            }
        }
    }

}

bool checkStart(Rule& start, std::string& input) {
    for (auto i : start.rhs) {
        size_t pos = i.find(input);
        if (pos != i.npos) {
            return true;
        }
        size_t pos2 = input.find(i);
        if (pos2 != input.npos) {
            return true;
        }
    }
    return false;
}

bool isTerminal(rule_t& rules, std::string& rhs) {
    for (auto i : rules) {
        size_t pos = rhs.find(i.lhs);
        if (pos != rhs.npos) {
            return false;
        }
    }
    return true;
}

std::string production(std::string& lhs, std::string& rhs, std::string input) {
    size_t pos = input.find(lhs);
    if (pos != input.npos) {
        std::string temp = input;
        temp = input.replace(pos, lhs.size(), rhs);
        return temp;
    }
    size_t pos2 = lhs.find(input);
    if (pos2 != lhs.npos) {
        std::string temp = input;
        temp = input.replace(pos, lhs.size(), rhs);
        return temp;
    }
    return input;
}

void makeRule(rule_t& rules, std::string str) {
    Rule rule(str);
    rules.push_back(rule);
}

// http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim from start
inline std::string& ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
inline std::string& rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
inline std::string& trim(std::string &s) {
    return ltrim(rtrim(s));
}

inline std::string& fulltrim(std::string& s) {
    s = trim(s);
    for (size_t i=0; i<s.length(); i++) {
        if (s.at(i) == ' ') {
            s.replace(i, 1, "");
            i = 0;
        }
    }
    if (s.length() == 0) {
        s = " ";
    }
    return s;
}

void printResults(std::set<std::string>& productions) {
    std::vector<std::string> dumbass;
    for (auto i : productions) {
        dumbass.push_back(i);
    }
    compare c;
    std::sort(dumbass.begin(), dumbass.end(), c);
    
    std::cout << "===Productions===" << std::endl;
    size_t length = 0;
    std::cout << "Length      String";
    for (auto i : dumbass) {
        
        if (i.length() == length) {
            std::cout << ", " << i;
        }
        else {
            if (i.at(0) == ' ') {
                length = 0;
            }
            else {
                length = i.length();
            }
            std::cout << std::endl << length << char(9) << char(9) << char(9) << i;
        }
    }
    std::cout << std::endl;
}

void searchProductions(std::set<std::string>& productions) {
    std::string search = "";
    while (search.compare("q") != 0) {
        search = "";
        std::cout << "Enter Search Item (q for quit): ";
        std::cin >> search;
        if (search.compare("q") == 0) {
            break;
        }
        auto it = productions.find(search);
        if (it == productions.end()) {
            std::cout << search << " was not found in the results." << std::endl;
        }
        else {
            std::cout << search << " was found!" << std::endl;
        }
    }
}