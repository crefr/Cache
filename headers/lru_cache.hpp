#include <iostream>
#include <list>
#include <unordered_map>

template <typename T, typename KeyT>
class LruCache {
    size_t size_;
    std::list<T> data_;

    using ListIt = typename std::list<T>::iterator;
    std::unordered_map<KeyT, ListIt> map_;

public:
    LruCache(size_t size) : size_(size) {};
    ~LruCache() {};

    bool full() const { return data_.size() == size_; };

    template <typename F>
    bool lookup_update(KeyT key, F slow_get_page);
};

template <typename T, typename KeyT>
template <typename F>
bool LruCache<T, KeyT>::lookup_update(KeyT key, F slow_get_page) {
    auto hit = map_.find(key);
    if (hit == map_.end()) { // not found
        if (full()) {
            map_.erase(data_.back().id);
            data_.pop_back();
        }
        // fallback to slow method
        data_.push_front(slow_get_page(key));
        map_[key] = data_.begin();
        return false;
    }
    auto eltit = hit->second;
    if (eltit != data_.begin())
        data_.splice(data_.begin(), data_, eltit, std::next(eltit));

    return true;
};
