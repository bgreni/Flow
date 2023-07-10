#pragma once
#include <string>
#include "Token/Token.h"

#define STR static const std::string

#define TOK(t, l) FlowToken::Token(FlowToken::Type::t, l, 0, 0)