#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::size_t;

template <typename KeyT>
class IdealCache {
    size_t size;
    std::vector<KeyT> pages;

    // key = page id, value = steps (in reversed order)
    std::unordered_map<KeyT, std::vector<size_t>> access_steps;

    // Set of pages in cache
    std::unordered_set<KeyT> cache;

    // By number of step find page, also is used to sort
    std::map<size_t, KeyT> step_page_cache;

    /// For every page store access steps
    void precalculate() {
        for (size_t step = pages.size(); step > 0;) {
            step--;
            access_steps[pages[step]].push_back(step);
        }
    }

public:
    IdealCache(size_t size) : size(size) {}
    ~IdealCache() {}

    void lookup_update(KeyT page_id) {
        pages.push_back(page_id);
    }

    size_t calc_hits() {
        precalculate();

        const size_t INF = ~0;

        size_t hits = 0;

        for (size_t step = 0; step < pages.size(); step++) {
            auto page = pages[step];
            std::vector<size_t>& key_accesses = access_steps[page];
            key_accesses.pop_back();

            size_t next_step = (key_accesses.empty()) ? INF : key_accesses.back();

            if (cache.contains(page)){
                hits++;

                if (next_step != INF) {
                    step_page_cache.erase(step);
                    step_page_cache[next_step] = page;
                } else {
                    // erasing the page to give space for others
                    cache.erase(page);
                    step_page_cache.erase(step);
                }
                continue;
            }

            // Miss here
            if (next_step == INF)
                continue;

            // Miss and this page will be seen later
            cache.insert(page);
            step_page_cache[next_step] = page;

            if (cache.size() > size) {
                // Need to free space
                auto page_to_erase = std::prev(step_page_cache.end())->second;
                step_page_cache.erase(std::prev(step_page_cache.end()));
                cache.erase(page_to_erase);
            }
        }

        return hits;
    }

};
