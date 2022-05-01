//
// Created by bys on 2022/3/29.
//

#ifndef BYS_LIB_SYSTEM_LIB_IO_UNITS_H_
#define BYS_LIB_SYSTEM_LIB_IO_UNITS_H_

#include<cstdio>
#include<cstdarg>

void PrintBinary(unsigned int value){
  printf("0x");
  for (int i = 31; i >= 0; i--) {
      if((i+1)%4 == 0)  printf(" ");
      printf("%u", ( (value >> i)&0x1) );
  }
};

#define PRINT_METADATA
#ifdef PRINT_METADATA
#define PRINTLN(format, ...) printf("(" __FILE__ ":%d) %s: " format "\n", __LINE__ , __FUNCTION__ , ##__VA_ARGS__)
#else
#define PRINTLN(format, ...)printf(format"\n",##__VA_ARGS__)
#endif

#define PRINT_CHAR(char_value)      PRINTLN(#char_value": %c",char_value)
#define PRINT_WCHAR(char_value)     PRINTLN(#char_value": %lc",char_value)
#define PRINT_INT(int_value)        PRINTLN(#int_value": %d",int_value)
#define PRINT_LONG(long_value)      PRINTLN(#long_value" %ld",long_value)
#define PRINT_LONGLONG(long_value)  PRINTLN(#long_value" %lld",long_value)
#define PRINT_BINARY(int_value)     PrintBinary((unsigned int) int_value);
#define PRINT_HEX(int_value)        PRINTLN(#int_value": %#x",int_value)
#define PRINT_BOOL(bool_value)      PRINTLN(#bool_value": %s",bool_value?"true":"false")
#define PRINT_DOUBLE(double_value)  PRINTLN(#double_value": %g",double_value)
#define PRINT_STRING(string_value)  PRINTLN(#string_value": %s",string_value)

#define PRINT_ARRAY(format, array, length)\
{\
    for(int array_index = 0;array_index<length;++array_index)\
    {\
        printf(format "\t" ,array[array_index]);\
    }\
    printf("\n");\
}

#define PRINT_INT_ARRAY_LN(array, length)\
{\
    for(int i = 0;i<length;++i)\
    {\
        PRINTLN(#array"[%d]: %d", i, array[i]);\
    }\
}

#define PRINT_INT_ARRAY(array, length)    PRINT_ARRAY("%d",array,length)
#define PRINT_CHAR_ARRAY(array, length)   PRINT_ARRAY("%c",array,length)
#define PRINT_DOUBLE_ARRAY(array, length) PRINT_ARRAY("%g",array,length)

#endif //BYS_LIB_SYSTEM_LIB_IO_UNITS_H_
