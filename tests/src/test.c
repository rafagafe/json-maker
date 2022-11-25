
/*

<https://github.com/rafagafe/tiny-json>

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
  Copyright (c) 2018 Rafa Garcia <rafagarcia77@gmail.com>.

  Permission is hereby  granted, free of charge, to any  person obtaining a copy
  of this software and associated  documentation files (the "Software"), to deal
  in the Software  without restriction, including without  limitation the rights
  to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
  copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
  IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
  FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
  AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
  LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include "json-maker/json-maker.h"

// ----------------------------------------------------- Test "framework": ---

#define done() return 0
#define fail() return __LINE__
static int checkqty = 0;
#define check( x ) do { ++checkqty; if (!(x)) fail(); } while ( 0 )

struct test {
    int(*func)(void);
    char const* name;
};

static int test_suit( struct test const* tests, int numtests ) {
    printf( "%s", "\n\nTests:\n" );
    int failed = 0;
    for( int i = 0; i < numtests; ++i ) {
        printf( " %02d%s%-25s ", i, ": ", tests[i].name );
        int linerr = tests[i].func();
        if ( 0 == linerr )
            printf( "%s", "OK\n" );
        else {
            printf( "%s%d\n", "Failed, line: ", linerr );
            ++failed;
        }
    }
    printf( "\n%s%d\n", "Total checks: ", checkqty );
    printf( "%s[ %d / %d ]\r\n\n\n", "Tests PASS: ", numtests - failed, numtests );
    return failed;
}


// ----------------------------------------------------------- Unit tests: ---

static int escape( void ) {
    char buff[512];
    size_t remLen = sizeof(buff);
    char* p = json_objOpen( buff, NULL, &remLen );
    p = json_str( p, "name", "\tHello: \"man\"\n", &remLen );
    p = json_objClose( p, &remLen );
    p = json_end( p, &remLen );
    printf( "\n\n%s\n\n", buff );
    static char const rslt[] = "{\"name\":\"\\tHello: \\\"man\\\"\\n\"}";
    check( p - buff == sizeof rslt - 1 );
    check( 0 == strcmp( buff, rslt ) );
    done();
}

static int len( void ) {
    char buff[512];
    size_t remLen = sizeof(buff);
    char* p = json_objOpen( buff, NULL, &remLen );
    p = json_nstr( p, "name", "\tHello: \"man\"\n", 6, &remLen );
    p = json_objClose( p, &remLen );
    p = json_end( p, &remLen );
    static char const rslt[] = "{\"name\":\"\\tHello\"}";
    check( p - buff == sizeof rslt - 1 );
    check( 0 == strcmp( buff, rslt ) );
    done();
}

static int empty( void ) {
    char buff[512];
    {
        size_t remLen = sizeof(buff);
        char* p = json_objOpen( buff, NULL, &remLen );
        p = json_objClose( p, &remLen );
        p = json_end( p, &remLen );
        static char const rslt[] = "{}";
        check( p - buff == sizeof rslt - 1 );
        check( 0 == strcmp( buff, rslt ) );
    }
    {
        size_t remLen = sizeof(buff);
        char* p = json_objOpen( buff, NULL, &remLen );
        p = json_arrOpen( p, "a", &remLen );
        p = json_arrClose( p, &remLen );
        p = json_objClose( p, &remLen );
        p = json_end( p, &remLen );
        static char const rslt[] = "{\"a\":[]}";
        check( p - buff == sizeof rslt - 1 );
        check( 0 == strcmp( buff, rslt ) );
    }
    {
        size_t remLen = sizeof(buff);
        char* p = json_objOpen( buff, NULL, &remLen );
        p = json_arrOpen( p, "a", &remLen );
        p = json_objOpen( p, NULL, &remLen );
        p = json_objClose( p, &remLen );
        p = json_objOpen( p, NULL, &remLen );
        p = json_objClose( p, &remLen );
        p = json_arrClose( p, &remLen );
        p = json_objClose( p, &remLen );
        p = json_end( p, &remLen );
        static char const rslt[] = "{\"a\":[{},{}]}";
        check( p - buff == sizeof rslt - 1 );
        check( 0 == strcmp( buff, rslt ) );
    }
    done();
}

#ifndef LONG_LONG_MAX
#define LONG_LONG_MAX LLONG_MAX
#define LONG_LONG_MIN LLONG_MIN
#endif

static int primitive( void ) {
    char buff[512];
    size_t remLen = sizeof(buff);
    char* p = json_objOpen( buff, NULL, &remLen );
    p = json_verylong( p, "max",  LONG_LONG_MAX, &remLen );
    p = json_verylong( p, "min",  LONG_LONG_MIN, &remLen );
    p = json_bool( p, "boolvar0", 0, &remLen );
    p = json_bool( p, "boolvar1", 1, &remLen );
    p = json_null( p, "nullvar", &remLen );
    p = json_objClose( p, &remLen );
    p = json_end( p, &remLen );
    static char const rslt[] =  "{"
                                    "\"max\":9223372036854775807,"
                                    "\"min\":-9223372036854775808,"
                                    "\"boolvar0\":false,"
                                    "\"boolvar1\":true,"
                                    "\"nullvar\":null"
                                "}";
    check( p - buff == sizeof rslt - 1 );
    check( 0 == strcmp( buff, rslt ) );
    done();
}

static int integers( void ) {
    {
        char buff[64];
        size_t remLen = sizeof( buff );
        char* p = json_objOpen( buff, NULL, &remLen );
        p = json_int( p, "a", 0, &remLen );
        p = json_int( p, "b", 1, &remLen );
        p = json_objClose( p, &remLen );
        p = json_end( p, &remLen );
        static char const rslt[] = "{\"a\":0,\"b\":1}";
        check( p - buff == sizeof rslt - 1 );
        check( 0 == strcmp( buff, rslt ) );
    }
    {
        char buff[64];
        size_t remLen = sizeof( buff );
        char* p = json_objOpen( buff, NULL, &remLen );
        p = json_int( p, "max", INT_MAX, &remLen );
        p = json_int( p, "min", INT_MIN, &remLen );
        p = json_objClose( p, &remLen );
        p = json_end( p, &remLen );
        char rslt[ sizeof buff ];
        int len = snprintf( rslt, sizeof( rslt ), "{\"max\":%d,\"min\":%d}", INT_MAX, INT_MIN);
        check( len < sizeof buff );
        check( p - buff == len );
        check( 0 == strcmp( buff, rslt ) );
    }
    {
        char buff[64];
        size_t remLen = sizeof( buff );
        char* p = json_objOpen( buff, NULL, &remLen );
        p = json_uint( p, "max", UINT_MAX, &remLen );
        p = json_objClose( p, &remLen );
        p = json_end( p, &remLen );
        char rslt[ sizeof buff ];
        int len = snprintf( rslt, sizeof( rslt ), "{\"max\":%u}", UINT_MAX);
        check( len < sizeof buff );
        check( p - buff == len );
        check( 0 == strcmp( buff, rslt ) );
    }
    {
        char buff[64];
        size_t remLen = sizeof( buff );
        char* p = json_objOpen( buff, NULL, &remLen );
        p = json_long( p, "max", LONG_MAX, &remLen );
        p = json_long( p, "min", LONG_MIN, &remLen );
        p = json_objClose( p, &remLen );
        p = json_end( p, &remLen );
        char rslt[ sizeof buff ];
        int len = snprintf( rslt, sizeof( rslt ), "{\"max\":%ld,\"min\":%ld}", LONG_MAX, LONG_MIN );
        check( len < sizeof buff );
        check( p - buff == len );
        check( 0 == strcmp( buff, rslt ) );
    }
    {
        char buff[64];
        size_t remLen = sizeof( buff );
        char* p = json_objOpen( buff, NULL, &remLen );
        p = json_ulong( p, "max", ULONG_MAX, &remLen );
        p = json_objClose( p, &remLen );
        p = json_end( p, &remLen );
        char rslt[ sizeof buff ];
        int len = snprintf( rslt, sizeof( rslt ), "{\"max\":%lu}", ULONG_MAX );
        check( len < sizeof buff );
        check( p - buff == len );
        check( 0 == strcmp( buff, rslt ) );
    }
    {
        char buff[64];
        size_t remLen = sizeof( buff );
        char* p = json_objOpen( buff, NULL, &remLen );
        p = json_verylong( p, "max", LONG_LONG_MAX, &remLen );
        p = json_verylong( p, "min", LONG_LONG_MIN, &remLen );
        p = json_objClose( p, &remLen );
        p = json_end( p, &remLen );
        char rslt[ sizeof buff ];
        int len = snprintf( rslt, sizeof( rslt ), "{\"max\":%lld,\"min\":%lld}", LONG_LONG_MAX, LONG_LONG_MIN );
        check( len < sizeof buff );
        check( p - buff == len );
        check( 0 == strcmp( buff, rslt ) );
    }
    done();
}

static int array( void ) {
    char buff[64];
    size_t remLen = sizeof( buff );
    char* p = json_objOpen( buff, NULL, &remLen );
    p = json_arrOpen( p, "a", &remLen );
    for( int i = 0; i < 4; ++i )
        p = json_int( p, NULL, i, &remLen );
    p = json_arrClose( p, &remLen );
    p = json_objClose( p, &remLen );
    p = json_end( p, &remLen );
    static char const rslt[] = "{\"a\":[0,1,2,3]}";
    check( p - buff == sizeof rslt - 1 );
    check( 0 == strcmp( buff, rslt ) );
    done();
}

static int real( void ) {
    char buff[64];
    size_t remLen = sizeof( buff );
    char* p = json_objOpen( buff, NULL, &remLen );
    p = json_arrOpen( p, "data", &remLen );
    static double const lut[] = { 0.2, 2e-6, 5e6 };
    for( int i = 0; i < sizeof lut / sizeof *lut; ++i )
        p = json_double( p, NULL, lut[i], &remLen );
    p = json_arrClose( p, &remLen );
    p = json_objClose( p, &remLen );
    p = json_end( p, &remLen );
#ifdef NO_SPRINTF
    static char const rslt1[] = "{\"data\":[0,0,5000000]}";
    static char const rslt2[] = "{\"data\":[0,0,5000000]}";
#else
    static char const rslt1[] = "{\"data\":[0.2,2e-006,5e+006]}";
    static char const rslt2[] = "{\"data\":[0.2,2e-06,5e+06]}";
#endif
    check( p - buff == sizeof rslt1 - 1 || p - buff == sizeof rslt2 - 1 );
    check( 0 == strcmp( buff, rslt1 ) ||  0 == strcmp( buff, rslt2 ) );
    done();
}

static int bounds(void) {
    char buff[6];
    char fillVal = 0x7c;
    {
        size_t remLen = sizeof(buff) - 1;
        memset( buff, fillVal, sizeof( buff ) );
        char* p = json_objOpen( buff, NULL, &remLen );
        p = json_str( p, "0123456789", "value", &remLen );
        p = json_objClose( p, &remLen );
        p = json_end( p, &remLen );
        // Check we didn't write past remLen bytes
        check(buff[sizeof( buff ) - 1] == fillVal);
    }
    {
        size_t remLen = sizeof( buff ) - 1;
        memset( buff, fillVal, sizeof( buff ));
        char* p = json_objOpen( buff, NULL, &remLen );
        p = json_str( p, "v", "0123456789", &remLen );
        p = json_objClose( p, &remLen );
        p = json_end( p, &remLen );
        // Check we didn't write past remLen bytes
        check(buff[sizeof( buff ) - 1] == fillVal);
    }
    done();
}
// --------------------------------------------------------- Execute tests: ---

int main( void ) {
    static struct test const tests[] = {
        { escape,    "Escape characters"        },
        { len,       "Non-null-terminated"      },
        { empty,     "Empty objects and arrays" },
        { primitive, "Primitives values"        },
        { integers,  "Integers values"          },
        { array,     "Array"                    },
        { real,      "Real"                     },
        { bounds,    "Bounds"                   },
    };
    return test_suit( tests, sizeof tests / sizeof *tests );
}
