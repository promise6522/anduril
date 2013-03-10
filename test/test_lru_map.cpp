#include <assert.h>
#include <string>

#include "../util/lru_map.h"


int main() {
    anduril::lru_map<int, std::string, 5> cache;
    assert(cache.capacity() == 5);

    cache.insert(std::make_pair(1, "China"));
    cache.insert(std::make_pair(2, "America"));
    cache.insert(std::make_pair(3, "Russia"));
    cache.insert(std::make_pair(4, "UK"));
    cache.insert(std::make_pair(5, "Canada"));
    assert(cache.size() == 5);

    // The first-inserted item will be evicted
    cache.insert(std::make_pair(6, "Singapore"));
    assert(cache.size() == 5);
    assert(cache.exists(6));
    assert(!cache.exists(1));

    // Update second item, then insert a new item, cause the third item to be evicted
    cache[2] = "Japan";
    assert(cache[2] == "Japan");
    assert(cache.size() == 5);
    cache.insert(std::make_pair(7, "Egypt"));
    assert(cache.size() == 5);
    assert(cache.exists(2));
    assert(!cache.exists(3));

}
