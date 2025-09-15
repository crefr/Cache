#include <iostream>
#include <ostream>
#include <vector>

#include "lru_cache.hpp"
#include "2q_cache.hpp"

struct CacheCell {
    size_t id;
    // something specific here...
};

CacheCell slow_get_page_mock(size_t id) {
    return CacheCell{id};
}

int main(int argc, char **argv) {
    size_t cache_size = 0;
    size_t elem_num = 0;

    std::cin >> cache_size >> elem_num;

    std::vector<size_t> test_ids(elem_num);
    for (size_t &elem: test_ids) {
        std::cin >> elem;
    }

    Cache2Q<CacheCell, size_t> my_cache(cache_size);

    size_t hits = 0;
    for (size_t index: test_ids) {
        std::cout << "size is " << my_cache.get_real_size() << std::endl;
        std::cout << "Current element is " << index << " ";
        if (my_cache.lookup_update(index, slow_get_page_mock)) {
            hits++;
            std::cout << "Hit!" << std::endl;
        }
        else {
            std::cout << "No Hit!" << std::endl;
        }
        std::cout << "size is " << my_cache.get_real_size() << std::endl;
    }

    std::cout << hits << std::endl;

    return 0;
}
