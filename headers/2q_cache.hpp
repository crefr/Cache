#include <list>
#include <unordered_map>

using std::size_t;

// List and hasmap combined
//! T must have id as its field (type of id is KeyT)
template <typename KeyT, typename T>
struct HashList {
    std::list<T> list;

    using ListIt = typename std::list<T>::iterator;
    std::unordered_map<KeyT, ListIt> map;

    KeyT pop_back() {
        auto deleted_elem_key = list.back().id;
        map.erase(deleted_elem_key);
        list.pop_back();

        return deleted_elem_key;
    }
};

template <typename T, typename KeyT>
class Cache2Q {
    struct CacheCell {
        KeyT id;
        T data;
    };

    size_t size;                    // Overall size (in pages)

    size_t Kin;                     // max size of A1_in
    size_t Kout;                    // max size of A1_out

    HashList<KeyT, CacheCell> Am;
    HashList<KeyT, CacheCell> A1_in;

    std::list<KeyT> A1_out;         // Stores only ids

    using ListItKeyT = typename std::list<KeyT>::iterator;
    std::unordered_map<KeyT, ListItKeyT> A1_out_map;

    bool full() {
        return (Am.list.size() + A1_in.list.size()) == size;
    }

    void reclaim_for_page();

public:
    Cache2Q(size_t size, double In_part = .25, double Out_part = .50) : size(size) {
        Kin = ((double) size) * In_part;
        Kout = ((double) size) * Out_part;
    }
    ~Cache2Q() {}

    size_t get_real_size() const { return Am.list.size() + A1_in.list.size(); }

    template <typename F>
    bool lookup_update(KeyT key, F slow_get_page);
};


// If there is no space, we free a page slot to
// make room for page.
template <typename T, typename KeyT>
void Cache2Q<T, KeyT>::reclaim_for_page() {
    if (! full())
        return;

    if (A1_in.list.size() > Kin) {
        auto to_out_page_id = A1_in.pop_back();

        A1_out.push_front(to_out_page_id);
        A1_out_map[to_out_page_id] = A1_out.begin();

        if (A1_out.size() > Kout) {
            auto deleted_id = A1_out.back();
            A1_out_map.erase(deleted_id);
            A1_out.pop_back();
        }

        return;
    }

    Am.pop_back();
}

template <typename T, typename KeyT>
template <typename F>
bool Cache2Q<T, KeyT>::lookup_update(KeyT key, F slow_get_page) {
    auto am_hit = Am.map.find(key);
    if (am_hit != Am.map.end()) {
        auto eltit = am_hit->second;

        if (eltit != Am.list.begin())
            Am.list.splice(Am.list.begin(), Am.list, eltit, std::next(eltit));

        return true;
    }

    auto a1_out_hit = A1_out_map.find(key);
    if (a1_out_hit != A1_out_map.end()) {
        reclaim_for_page();

        Am.list.push_front(CacheCell{key, slow_get_page(key)});
        Am.map[key] = Am.list.begin();

        return false;
    }

    auto a1_in_hit = A1_in.map.find(key);
    if (a1_in_hit != A1_in.map.end()) {
        return true;
    }

    reclaim_for_page();
    Am.list.push_front(CacheCell{key, slow_get_page(key)});
    Am.map[key] = Am.list.begin();

    return false;
}
