#include "Map.h"
#include <optional>

namespace Flow {
    template<class T, class U>
    bool Map<T, U>::contains(T k) {
        return find(k) != this->cend();
    }
}