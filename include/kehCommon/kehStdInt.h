#ifndef keh_StdInt_h
#define keh_StdInt_h

#ifndef _MSC_VER
#  include <stdint.h>
#else

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short int uint16_t;
typedef signed short int int16_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;


#endif

#endif