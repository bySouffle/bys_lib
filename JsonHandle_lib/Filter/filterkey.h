//
// Created by bys on 2022/1/30.
//

#ifndef BYS_LIB_FILTERKEY_H
#define BYS_LIB_FILTERKEY_H

// JSON filterkey example with SAX-style API.

// i.e. the events flow is: reader -> filter -> writer

#include "rapidjson/reader.h"
#include "rapidjson/writer.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/error/en.h"
#include <stack>


// This handler forwards event into an output handler, with filtering the descendent events of specified key.
template <typename OutputHandler>
class FilterKeyHandler {
public:
    typedef char Ch;

    FilterKeyHandler(OutputHandler& outputHandler, const Ch* keyString, rapidjson::SizeType keyLength) :
            outputHandler_(outputHandler), keyString_(keyString), keyLength_(keyLength), filterValueDepth_(), filteredKeyCount_()
    {}

    bool Null()             { return filterValueDepth_ > 0 ? EndValue() : outputHandler_.Null()    && EndValue(); }
    bool Bool(bool b)       { return filterValueDepth_ > 0 ? EndValue() : outputHandler_.Bool(b)   && EndValue(); }
    bool Int(int i)         { return filterValueDepth_ > 0 ? EndValue() : outputHandler_.Int(i)    && EndValue(); }
    bool Uint(unsigned u)   { return filterValueDepth_ > 0 ? EndValue() : outputHandler_.Uint(u)   && EndValue(); }
    bool Int64(int64_t i)   { return filterValueDepth_ > 0 ? EndValue() : outputHandler_.Int64(i)  && EndValue(); }
    bool Uint64(uint64_t u) { return filterValueDepth_ > 0 ? EndValue() : outputHandler_.Uint64(u) && EndValue(); }
    bool Double(double d)   { return filterValueDepth_ > 0 ? EndValue() : outputHandler_.Double(d) && EndValue(); }
    bool RawNumber(const Ch* str, rapidjson::SizeType len, bool copy) { return filterValueDepth_ > 0 ? EndValue() : outputHandler_.RawNumber(str, len, copy) && EndValue(); }
    bool String   (const Ch* str, rapidjson::SizeType len, bool copy) { return filterValueDepth_ > 0 ? EndValue() : outputHandler_.String   (str, len, copy) && EndValue(); }

    bool StartObject() {
        if (filterValueDepth_ > 0) {
            filterValueDepth_++;
            return true;
        }
        else {
            filteredKeyCount_.push(0);
            return outputHandler_.StartObject();
        }
    }

    bool Key(const Ch* str, rapidjson::SizeType len, bool copy) {
        if (filterValueDepth_ > 0)
            return true;
        else if (len == keyLength_ && std::memcmp(str, keyString_, len) == 0) {
            filterValueDepth_ = 1;
            return true;
        }
        else {
            ++filteredKeyCount_.top();
            return outputHandler_.Key(str, len, copy);
        }
    }

    bool EndObject(rapidjson::SizeType) {
        if (filterValueDepth_ > 0) {
            filterValueDepth_--;
            return EndValue();
        }
        else {
            // Use our own filtered memberCount
            rapidjson::SizeType memberCount = filteredKeyCount_.top();
            filteredKeyCount_.pop();
            return outputHandler_.EndObject(memberCount) && EndValue();
        }
    }

    bool StartArray() {
        if (filterValueDepth_ > 0) {
            filterValueDepth_++;
            return true;
        }
        else
            return outputHandler_.StartArray();
    }

    bool EndArray(rapidjson::SizeType elementCount) {
        if (filterValueDepth_ > 0) {
            filterValueDepth_--;
            return EndValue();
        }
        else
            return outputHandler_.EndArray(elementCount) && EndValue();
    }

private:
    FilterKeyHandler(const FilterKeyHandler&);
    FilterKeyHandler& operator=(const FilterKeyHandler&);

    bool EndValue() {
        if (filterValueDepth_ == 1) // Just at the end of value after filtered key
            filterValueDepth_ = 0;
        return true;
    }

    OutputHandler& outputHandler_;
    const char* keyString_;
    const rapidjson::SizeType keyLength_;
    unsigned filterValueDepth_;
    std::stack<rapidjson::SizeType> filteredKeyCount_;
};


#endif //BYS_LIB_FILTERKEY_H
