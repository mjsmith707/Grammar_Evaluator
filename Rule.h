//
//  Rule.h
//  CFS_Grammar
//
//  Created by Matt on 4/6/15.
//  Copyright (c) 2015 Matt. All rights reserved.
//

#ifndef __CFS_Grammar__Rule__
#define __CFS_Grammar__Rule__

#include <string>
#include <vector>

class Rule {
    public:
        std::string lhs;
        std::vector<std::string> rhs;
        Rule(std::string&);
};

#endif /* defined(__CFS_Grammar__Rule__) */
