#ifndef STATUS_HEADER
#define STATUS_HEADER

#include <iostream>
#include <string>
#include "slice.h"

namespace wimpl {
class Status {

   public:
    // 初始化一个成功的 Status
    Status() : stat_(NULL) { std::cout << sizeof(Code) << endl; }

    // 对于基础类型, delete 和 delete[] 效果一样;
    // 但是如果是类,则需要使用 delete[],因为如果是使用delete, 那么只有 ins[0]
    // 的析构函数会被调用
    ~Status() { delete[] stat_; }

    // 复制某个 Status
    // Stauts(const Status& s);

    // 生成一个 Status 实例
    static Status OK() { return Status(); }
    static Status NotFound(const Slice& msg1, const Slice& msg2 = Slice()) {
        return Status(kNotFound, msg1, msg2);
    }
    static Status Curruption(const Slice& msg1, const Slice& msg2 = Slice()) {
        return Status(kCorruption, msg1, msg2);
    }
    static Status NotSupported(const Slice& msg1, const Slice& msg2 = Slice()) {
        return Status(kNotSupported, msg1, msg2);
    }
    static Status InvalidArgument(const Slice& msg1,
                                  const Slice& msg2 = Slice()) {
        return Status(kInvalidArgument, msg1, msg2);
    }
    static Status IOError(const Slice& msg1, const Slice& msg2 = Slice()) {
        return Status(kIOError, msg1, msg2);
    }

    // 判断方法
    bool ok() { return stat_ == NULL; }
    bool IsNotFound() { return code() == kNotFound; }
    bool IsCurruption() { return code() == kCorruption; }
    bool IsNotSupported() { return code() == kNotSupported; }
    bool IsInvalidArgument() { return code() == kInvalidArgument; }
    bool IsIOError() { return code() == kIOError; }

    Status(int code, Slice& msg);

    std::string ToString() const;

   private:
    // null, or like:
    // stat_[0, 3], length of message
    // stat_[4], code
    // stat_[5...] message
    const char* stat_;

    // 定义错误码
    enum Code {
        kOk = 0,
        kNotFound = 1,
        kCorruption = 2,
        kNotSupported = 3,
        kInvalidArgument = 4,
        kIOError = 5
    };

    // 获取当前 stat_ 中的 code
    Code code() const {
        return (stat_ == NULL) ? kOk : static_cast<Code>(stat_[4]);
    }

    Status(Code code, const Slice& msg1, const Slice& msg2);
};
}
#endif
