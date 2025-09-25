#include <list>
#include <unordered_map>
#include <utility>

using std::size_t;

// List and hasmap combined.
// List contains pairs with key and value, hashmap
// allows quick access by the key.
template <typename KeyT, typename T>
struct HashList {
    using pairT = typename std::pair<KeyT, T>;
    std::list<pairT> list;

    using ListIt = typename std::list<pairT>::iterator;
    std::unordered_map<KeyT, ListIt> map;

    KeyT pop_back() {
        auto deleted_elem_key = list.back().first;
        map.erase(deleted_elem_key);
        list.pop_back();

        return deleted_elem_key;
    }

    void push_front(KeyT key, T value) {
        list.push_front(std::pair<KeyT, T>{key, value});
        map[key] = list.begin();
    }

    size_t size() const {
        return list.size();
    }
};

template <typename T, typename KeyT>
class Cache2Q {
    size_t size;                    // Overall size (in pages)

    size_t Kin;                     // max size of A1_in
    size_t Kout;                    // max size of A1_out

    HashList<KeyT, T> Am;
    HashList<KeyT, T> A1_in;
    HashList<KeyT, KeyT> A1_out;         // Stores only ids but still needs hashlist

    bool full() const {
        return (Am.size() + A1_in.size()) == size;
    }

    void reclaim_for_page();

public:
    Cache2Q(size_t size, double in_part = .25, double out_part = .50) : size(size) {
        Kin  = (size_t) (((double) size) * in_part);
        Kout = (size_t) (((double) size) * out_part);
    }
    ~Cache2Q() {}

    template <typename F>
    bool lookup_update(KeyT key, F slow_get_page);
};


// If there is no space, we free a page slot to
// make room for page.
template <typename T, typename KeyT>
void Cache2Q<T, KeyT>::reclaim_for_page() {
    if (! full())
        return;

    if (A1_in.size() > Kin) {
        auto to_out_page_id = A1_in.pop_back();
        A1_out.push_front(to_out_page_id, to_out_page_id);

        if (A1_out.size() > Kout)
            A1_out.pop_back();

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

    auto a1_out_hit = A1_out.map.find(key);
    if (a1_out_hit != A1_out.map.end()) {
        reclaim_for_page();
        Am.push_front(key, slow_get_page(key));

        return false;
    }

    auto a1_in_hit = A1_in.map.find(key);
    if (a1_in_hit != A1_in.map.end()) {
        return true;
    }

    reclaim_for_page();
    A1_in.push_front(key, slow_get_page(key));

    return false;
}
