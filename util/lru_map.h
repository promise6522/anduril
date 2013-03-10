#ifndef __ANDURIL_LRU_MAP_

#include <list>
#include <map>

#include <boost/static_assert.hpp>

namespace anduril {

template<typename K, typename V, size_t N>
class lru_map {
    BOOST_STATIC_ASSERT(N > 0);
public:
    typedef std::pair<K, V> value_type;

    bool insert(const value_type& val) {
        typename map_type::iterator it = map_.find(val.first);
        if (it != map_.end()) {
            return false;
        } else {
            reserve_space(1);
            lru_list_.push_back(val.first);
            typename lru_list_t::iterator lru_it = lru_list_.end();
            bool succ = map_.insert(
                std::make_pair(val.first,
                               std::make_pair(val.second, --lru_it))).second;
            assert(succ);
        }
    }

    V& operator[] (const K& key) {
        typename map_type::iterator it = map_.find(key);
        typename lru_list_t::iterator lru_it;
        VT& val = map_[key];
        if (it == map_.end()) {
            reserve_space(1);
        } else {
            // the key already exists, refresh the lru order
            lru_it = val.second;
            if (++lru_it == lru_list_.end()) {
                return val.first;
            }
            lru_list_.erase(val.second);
        }
        lru_list_.push_back(key);
        lru_it = lru_list_.end();

        val.second = --lru_it;
        return val.first;
    }

    bool exists(const K& key) const {
        return map_.find(key) != map_.end();
    }

    size_t size() const {
        return map_.size();
    }

    size_t capacity() const {
        return N;
    }

    void evict() {
        assert(map_.size() == lru_list_.size());
        if (map_.empty())
            return;

        K key = lru_list_.front();
        lru_list_.pop_front();
        assert(map_.erase(key) == 1);
    }

private:
    void reserve_space(size_t n) {
        assert(n <= N);
        size_t nfree = capacity() - size();
        while (nfree++ < n) {
            evict();
        }
    }

    typedef std::list<K> lru_list_t;
    typedef std::pair<V, typename lru_list_t::iterator> VT;
    typedef std::map<K, VT> map_type;

    map_type map_;
    lru_list_t lru_list_;
};

}

#endif
