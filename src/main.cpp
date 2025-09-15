#include <iostream>

#include "lru_cache.hpp"

struct CacheCell {
    size_t id;
    // something specific here...
};

CacheCell slow_get_page_mock(size_t id) {
    return CacheCell{id};
}

int main() {
    LruCache<CacheCell, size_t> my_cache(4);

    const size_t ids[] = {
        1, 2, 3, 4, 1, 2, 5, 1, 2, 4, 3, 4
    };

    size_t hits = 0;
    for (size_t index: ids)
        if (my_cache.lookup_update(index, slow_get_page_mock)) hits++;

    std::cout << "Number of hits is " << hits << std::endl;

    return 0;
}
