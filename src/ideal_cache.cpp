#include <iostream>
#include <vector>

#include "ideal_cache.hpp"

size_t run_test(size_t cache_size, std::vector<size_t> &test_ids) {
    IdealCache<size_t> ideal(cache_size);

    for (size_t index: test_ids)
        ideal.lookup_update(index);

    return ideal.calc_hits();
}

int main(int argc, char **argv) {
    bool verbose  = false;

    for (int arg_index = 1; arg_index < argc; arg_index++) {
        std::string arg = argv[arg_index];
        if (arg == "-v")
            verbose = true;
    }

    size_t cache_size = 0;
    size_t elem_num = 0;

    std::cin >> cache_size >> elem_num;

    std::vector<size_t> test_ids(elem_num);
    for (size_t &elem: test_ids) {
        std::cin >> elem;
    }

    auto hits = run_test(cache_size, test_ids);

    if (verbose)
        std::cout << "Ideal cache hits " << hits << " out of " << elem_num << " (cache size is " << cache_size << " )" << std::endl;
    else
        std::cout << hits << std::endl;

    return 0;
}


