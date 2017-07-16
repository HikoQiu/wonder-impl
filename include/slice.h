
#ifndef SLICE_H
#define SLICE_H

#include <iostream>
#include <assert.h>
#include <string.h>
#include <string>

using namespace std;

class Slice {
   public:
    Slice() : data_(""), size_(0) {}

    Slice(const char* d, size_t n) : data_(d), size_(n) {}

    Slice(const char* d) : data_(d), size_(strlen(d)) {}

    // convert std::string to Slice 
    Slice(const string& s) : data_(s.data()), size_(s.size()) {}

    size_t size() const {
        return size_;
    };

    const char* data() const {
        return data_;
    };

    bool empty() { return size_ == 0; }

    void clear() {
        data_ = "";
        size_ = 0;
    }

    // remove prefix n bytes
    void remove_prefix(size_t n) {
        assert(n < size());

        data_ += n;
        size_ -= n;
    }

    string ToString() { return string(data_, size_); }

    // compare with b Slice
    int compare(const Slice& b) const;

    bool start_with(Slice& x) const {
        return (x.size() <= size_) && (memcmp(data_, x.data(), x.size()) == 0);
    }

    // support operator []
    char operator[](size_t n) {
        assert(n >= 0 && n < size_ - 1);
        return data_[n];
    }

   private:
    const char* data_;
    size_t size_;
};

inline bool operator==(Slice& x, Slice& y) {
    return (x.size() == y.size() && memcmp(x.data(), y.data(), x.size()) == 0);
}

inline bool operator!=(Slice& x, Slice& y) { return !(x == y); }


#endif

