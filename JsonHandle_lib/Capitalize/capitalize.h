//
// Created by bys on 2022/1/29.
//

//  将传入的json转化为大写

#ifndef BYS_LIB_CAPITALIZE_H
#define BYS_LIB_CAPITALIZE_H

#include "rapidjson/reader.h"
#include "rapidjson/writer.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/error/en.h"
#include <vector>
#include <cctype>


template<typename OutputHandler>
struct CapitalizeFilter {
    CapitalizeFilter(OutputHandler& out) : out_(out), buffer_() {}

    bool Null() { return out_.Null(); }
    bool Bool(bool b) { return out_.Bool(b); }
    bool Int(int i) { return out_.Int(i); }
    bool Uint(unsigned u) { return out_.Uint(u); }
    bool Int64(int64_t i) { return out_.Int64(i); }
    bool Uint64(uint64_t u) { return out_.Uint64(u); }
    bool Double(double d) { return out_.Double(d); }
    bool RawNumber(const char* str, rapidjson::SizeType length, bool copy) { return out_.RawNumber(str, length, copy); }
    bool String(const char* str, rapidjson::SizeType length, bool) {
        buffer_.clear();
        for (rapidjson::SizeType i = 0; i < length; i++)
            buffer_.push_back(static_cast<char>(std::toupper(str[i])));
        return out_.String(&buffer_.front(), length, true); // true = output handler need to copy the string
    }
    bool StartObject() { return out_.StartObject(); }
    bool Key(const char* str, rapidjson::SizeType length, bool copy) { return String(str, length, copy); }
    bool EndObject(rapidjson::SizeType memberCount) { return out_.EndObject(memberCount); }
    bool StartArray() { return out_.StartArray(); }
    bool EndArray(rapidjson::SizeType elementCount) { return out_.EndArray(elementCount); }

    OutputHandler& out_;
    std::vector<char> buffer_;

private:
    CapitalizeFilter(const CapitalizeFilter&);
    CapitalizeFilter& operator=(const CapitalizeFilter&);
};


#endif //BYS_LIB_CAPITALIZE_H
