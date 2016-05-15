//          Copyright Michal Pochobradský 2016.
//          Copyright Tibor Zauko 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <fixedpoint.h>

#include <sstream>
#include <string>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

template<unsigned char radix>
std::size_t fixedpoint::number<radix>::scale = 0;

TEST_CASE("Output formatting"){
    using std::string; using std::stringstream;
    const string res1("10::-34.134"),res2("2::-111.0101");
    fixedpoint::decimal n1("-10::34.134");
    fixedpoint::binary n2("2::-111.0101");
    stringstream n1t, n2t;
    n1t << n1;
    n2t << n2;
    REQUIRE( res1 == n1t.str() );
    REQUIRE( res2 == n2t.str() );
}

TEST_CASE("Input formatting"){
    using std::string; using std::stringstream;
    stringstream inp1("10::-34.134"), inp2("2::-111.0101");
    const fixedpoint::decimal n1t("-10::34.134");
    const fixedpoint::binary n2t("-2::111.0101");
    fixedpoint::decimal res1;
    fixedpoint::binary res2;
    inp1 >> res1;
    inp2 >> res2;
    REQUIRE( res1 == n1t );
    REQUIRE( res2 == n2t );
}
