#pragma once
#include <string>
#include "TestCommon/Common.h"

namespace TestCase {
    STR ADD_INTS = "10 + 34;";
    STR SUB_INTS = "443 - 2342;";
    STR MULT_INTS = "898 * 9834;";
    STR DIV_INTS = "32 / 23;";
    STR LT_TEST =
        "1 < 3;"
        "a << fTwo();";
    STR GT_TEST = 
        "3 > 2;"
        "10 >> fOne();";
    
    STR BANG_TEST = "!condition;";
}