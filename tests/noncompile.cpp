//          Copyright Michal Pochobradský 2016.
//          Copyright Tibor Zauko 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <fixedpoint.h>

#include <string>

using namespace fixedpoint;

template<unsigned char radix>
std::size_t number<radix>::scale = 0;

void radixHigh(){
    number<45> n;
    number<1> n1;
}

void implicitConv(){
    using namespace std::literals;
    number<21> n;
    n = "14::124ad"s;
    n += "10::10"s;
    n = 12.58;
    n -= 54.26;
}

int main(){
    radixHigh();
    implicitConv();
    return 0;
}
