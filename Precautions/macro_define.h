//
// Created by bys on 2022/3/25.
//

#ifndef BYS_LIB_PRECAUTIONS_MACRO_DEFINE_H_
#define BYS_LIB_PRECAUTIONS_MACRO_DEFINE_H_

#include<cstdio>
#include<cstdarg>
//  终端输出颜色配置宏   =========================================================================================
//the following are UBUNTU/LINUX ONLY terminal color codes.
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

//  终端输出颜色配置宏 END =========================================================================================
//  输出宏   =====================================================================================================
#define PRINT_METADATA
#ifdef PRINT_METADATA

#define PRINTLNF(format,...) printf("("__FILE__":%ld) %s: "  format "\n",__LINE__, __FUNCTION__,##__VA_ARGS__)
#else
#define PRINTLNF(format, ...)printf(format"\n",##__VA_ARGS__)
#endif

#define PRINT_CHAR(char_value) PRINTLNF(#char_value": %c",char_value)
#define PRINT_WCHAR(char_value)PRINTLNF(#char_value": %lc",char_value)
#define PRINT_INT(int_value)PRINTLNF(#int_value": %d",int_value)
#define PRINT_LONG(long_value)PRINTLNF(#long_value"%ld",long_value)
#define PRINT_LLONG(long_value)PRINTLNF(#long_value"%lld",long_value)
#define PRINT_BINARY(int_value)PrintBinary((unsigned int) int_value);
#define PRINT_HEX(int_value)PRINTLNF(#int_value": %#x",int_value)
#define PRINT_BOOL(bool_value)PRINTLNF(#bool_value": %s",bool_value?"true":"false")
#define PRINT_DOUBLE(double_value)PRINTLNF(#double_value": %g",double_value)
#define PRINT_STRING(string_value)PIRNTLNF(#string_value": %s",string_value)

#define PRINT_ARRAY(format, array, length)\
{\
for(int array_index = 0;array_index<length;++array_index)\
{\
printf(format,array[array_index]);\
}\
printf("\n");\
}

#define PRINT_INT_ARRAY_LN(array, length)\
{\
for(int i = 0;i<length;++i)\
{\
PRINTLNF(#array[i]"[%d]: %d",i,array[i]);\
}\
}

#define PRINT_INT_ARRAY(array, length)PRINT_ARRAY("%d",array,length)
#define PRINT_CHAR_ARRAY(array, length)PRINT_ARRAY("%c",array,length)
#define PRINT_DOUBLE_ARRAY(array, length)PRINT_ARRAY("%g",array,length)

//  输出宏 END   =========================================================================================



#endif //BYS_LIB_PRECAUTIONS_MACRO_DEFINE_H_
