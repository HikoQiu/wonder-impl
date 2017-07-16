#include "slice.h"

// compare min_len bytes
// 1: bigger
// 0: equal
// -1: smaller
int Slice::compare(const Slice& b) const {
    const size_t min_len = (size_ < b.size()) ? size_ : b.size();

    int r = memcmp(data_, b.data(), min_len);
    if (r == 0) {
        if (size_ < b.size()) return -1;
        if (size_ > b.size()) return 1;
    }

    return r;
}
