//
// Created by bysou on 2021/12/25.
//

#ifndef BYS_LIB_ARCHIVER_H
#define BYS_LIB_ARCHIVER_H

#include <cstddef>
#include <string>

///     json读取器
class JsonReader {
public:
    
    //! <Constructor>   
    //! \param json 读取的json
    JsonReader(const char* json);
    
    //!  <Destructor>
    ~JsonReader();
    
    //  >>>>>>>>>>>>    Json concept (Start)    >>>>>>>>>>>>>>>>>>
    operator bool() const { return !mError; }
    JsonReader& StartObject();
    JsonReader& Member(const char* name);
    bool HasMember(const char* name) const;
    JsonReader& EndObject();

    JsonReader& StartArray(size_t* size = 0);
    JsonReader& EndArray();

    JsonReader& operator&(bool& b);
    JsonReader& operator&(unsigned& u);
    JsonReader& operator&(int& i);
    JsonReader& operator&(double& d);
    JsonReader& operator&(std::string& s);

    JsonReader& SetNull();

    static const bool IsReader = true;
    static const bool IsWriter = !IsReader;

    //  <<<<<<<<<<<<    Json concept ( End )    <<<<<<<<<<<<<<<<<<

private:
    JsonReader(const JsonReader&);
    JsonReader& operator=(const JsonReader&);

    void Next();

    // PIMPL(Pointer to Implementation)
    void* mDocument;              ///< DOM result of parsing.
    void* mStack;                 ///< Stack for iterating the DOM
    bool mError;                  ///< Whether an error has occurred.
};


///     json生成器
class JsonWriter {
public:
    /// Constructor.
    JsonWriter();

    /// Destructor.
    ~JsonWriter();

    /// Obtains the serialized JSON string.
    const char* GetString() const;

    // Archive concept

    operator bool() const { return true; }

    JsonWriter& StartObject();
    JsonWriter& Member(const char* name);
    bool HasMember(const char* name) const;
    JsonWriter& EndObject();

    JsonWriter& StartArray(size_t* size = 0);
    JsonWriter& EndArray();

    JsonWriter& operator&(bool& b);
    JsonWriter& operator&(unsigned& u);
    JsonWriter& operator&(int& i);
    JsonWriter& operator&(double& d);
    JsonWriter& operator&(std::string& s);
    JsonWriter& SetNull();

    static const bool IsReader = false;
    static const bool IsWriter = !IsReader;

private:
    JsonWriter(const JsonWriter&);
    JsonWriter& operator=(const JsonWriter&);

    // PIMPL idiom
    void* mWriter;      ///< JSON writer.
    void* mStream;      ///< Stream buffer.
};

#endif //BYS_LIB_ARCHIVER_H
