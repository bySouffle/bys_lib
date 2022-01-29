//
// Created by bys on 2022/1/29.
//

//  校验json文件流并不带空格格式化输出标准json文件流

#ifndef BYS_LIB_CONDENSE_H
#define BYS_LIB_CONDENSE_H

#include "rapidjson/reader.h"
#include "rapidjson/writer.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/error/en.h"

int validation_stdout(rapidjson::FileReadStream &is, rapidjson::FileWriteStream &os) {

    rapidjson::Reader reader;

    rapidjson::Writer<rapidjson::FileWriteStream> writer(os);

    //! JSON reader 从输入流中解析并从 writer 输出
    if (!reader.Parse(is, writer)) {
        fprintf(stderr, "\nError(%u): %s\n", static_cast<unsigned>(reader.GetErrorOffset()),
                GetParseError_En(reader.GetParseErrorCode()));
        return 1;
    }
    return 0;
}

#endif //BYS_LIB_CONDENSE_H
