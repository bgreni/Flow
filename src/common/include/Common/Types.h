#pragma once
#include <memory>
#include <sys/_types/_int64_t.h>

using flow_int = int64_t;

template<class T>
using ptr = std::unique_ptr<T>;