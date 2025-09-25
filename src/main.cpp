#include <iostream>
#include <ostream>
#include <string>
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

size_t run_2q_test(size_t cache_size, std::vector<size_t> &test_ids) {
    Cache2Q<CacheCell, size_t> cache(cache_size);

    size_t hits = 0;
    for (size_t index: test_ids) {
        if (cache.lookup_update(index, slow_get_page_mock)) {
            hits++;
        }
    }

    return hits;
}

size_t run_lru_test(size_t cache_size, std::vector<size_t> &test_ids) {
    LruCache<CacheCell, size_t> cache(cache_size);

    size_t hits = 0;
    for (size_t index: test_ids) {
        if (cache.lookup_update(index, slow_get_page_mock)) {
            hits++;
        }
    }

    return hits;
}

int main(int argc, char **argv) {
    bool verbose  = false;
    bool test_lru = false;

    for (int arg_index = 1; arg_index < argc; arg_index++) {
        std::string arg = argv[arg_index];
        if (arg == "-v")
            verbose = true;
        else if (arg == "-l")
            test_lru = true;
    }

    size_t cache_size = 0;
    size_t elem_num = 0;

    std::cin >> cache_size >> elem_num;

    std::vector<size_t> test_ids(elem_num);
    for (size_t &elem: test_ids) {
        std::cin >> elem;
    }

    if (verbose) {
        auto hits_2q  = run_2q_test(cache_size, test_ids);
        std::cout << "2Q cache hits number is " << hits_2q << " out of " << elem_num << " (cache size is " << cache_size << ")" << std::endl;

        if (test_lru) {
            auto hits_lru = run_lru_test(cache_size, test_ids);
            std::cout << "LRU cache hits number is " << hits_lru << " out of " << elem_num << " (cache size is " << cache_size << ")" << std::endl;
        }

        return 0;
    }

    auto hits_2q  = run_2q_test(cache_size, test_ids);
    std::cout << hits_2q << std::endl;

    return 0;
}

