#include <iostream>
#include "status.h"

using namespace std;

namespace wimp {

// 初始化某个状态的Status
Status::Status(Code c, const Slice& msg1, const Slice& msg2) {
    assert(c != kOk);

    const uint32_t len1 = msg1.size();
    const uint32_t len2 = msg2.size();
    const uint32_t size = len1 + (len2 ? (2 + len2) : 0);

    // + 5, 5 是 4个字节表示总消息长度, 1 个字节表示 Status 类型
    char* result = new char(size + 5);

    // 设置总消息长度
    memcpy(result, &size, sizeof(size));
    // 设置错误码
    result[4] = static_cast<char>(c);

    //设置消息内容
    memcpy(result + 5, msg1.data(), len1);

    // 如果有其他信息, 拼接起来
    if (len2) {
        result[5 + len1] = ':';
        result[6 + len1] = ' ';
        memcpy(result + 5 + len1 + 2, msg2.data(), len2);
    }

    stat_ = result;
}

/**
 * 状态转为字符串
 *
 * 该方法需要注意: string Status::ToString() const;
 * 其中要求方法体中是不能修改(const),意味着方法体里面调用的方法体也必须是 const.
 * 所以里面的获取状态码必须声明为:  Code code() const;
 *
 */
string Status::ToString() const {
    if (stat_ == NULL) {
        return "OK";
    } else {
        char tmp[30];
        const char* type;
        switch (code()) {
            case kNotFound:
                type = "Not Found";
                break;

            case kCorruption:
                type = "Corruption";
                break;
            case kNotSupported:
                type = "Not Supported";
                break;

            case kInvalidArgument:
                type = "Invalid Argument";
                break;

            case kIOError:
                type = "IO Error";
                break;

            default:
                snprintf(tmp, sizeof(tmp), "Unknown code (%d)",
                         static_cast<int>(code()));
                type = tmp;
                break;
        }

        std::string result(type);
        uint32_t length;
        memcpy(&length, stat_, sizeof(length));
        result.append(stat_ + 5, length);
        return result;
    }
}
}
