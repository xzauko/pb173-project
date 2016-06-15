//          Copyright Michal Pochobradský 2016.
//          Copyright Tibor Zauko 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <fixedpoint.h>

#include <string>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using namespace fixedpoint;
using namespace std::literals;

template<unsigned char radix>
std::size_t number<radix>::scale = 0;

TEST_CASE("Addition same signs"){
    decimal a("50.15",2),b("45.256",3);
    const decimal c("5.4847",4);
    const decimal result1("55.6347"),result2("56.2254");
    const decimal result3("106.3754"),result4("111.2694");
    a += c;
    REQUIRE( a == result1 );
    REQUIRE( b+c+c == result2 );
    (b += c) += a;
    REQUIRE( b == result3 );
    a+=a;
    REQUIRE( a == result4 );

    hexadecimal x(-50),y(-45);
    const hexadecimal z(-5);
    const hexadecimal result5(-55),result6(-105);
    x += z;
    REQUIRE( x == result5 );
    REQUIRE( y+z+z == result5 );
    (y += z) += x;
    REQUIRE( y == result6 );
}

TEST_CASE("Subtraction same signs"){
    decimal a("50.15",2),b("45.256",3);
    const decimal c("5.4847",4);
    const decimal result1("44.6653"),result2("-4.8940");
    const decimal result3("34.2866"), zero(0);
    a -= c;
    REQUIRE( a == result1 );
    REQUIRE( b-c-a == result2 );
    (b -= c) -= c;
    REQUIRE( b == result3 );
    a-=a;
    REQUIRE( a == zero );

    hexadecimal x(-50),y(-45);
    const hexadecimal z(-5);
    const hexadecimal result4(5),result5(-35);
    x -= z;
    REQUIRE( x == y );
    REQUIRE( y-z-x == result4 );
    (y -= z) -= z;
    REQUIRE( y == result5 );
}

TEST_CASE("Add/subtract differing signs"){
    decimal a(50),b(-45);
    const decimal c(5), result1(-45);
    a += b;
    REQUIRE( a == c );
    REQUIRE( b+c-a == result1 );
    (b -= b) += c;
    REQUIRE( b == c );

    decimal x(-50),y(45);
    const decimal z(-5), result3(45);
    x += y;
    REQUIRE( x == z );
    REQUIRE( y+z-x == result3 );
    (y -= y) += z;
    REQUIRE( y == z );
}

TEST_CASE("Multiplication"){
    decimal a("251.354"s,3),b("-35.25"s,2);
    const decimal c("5.4",1);
    const decimal result1("-8860.228"s,3), result2("1357.311"s,3);
    REQUIRE( b*a == result1 );
    a*=c;
    REQUIRE( a == result2 );

    hexadecimal x("10::251.03125"s,3),y("10::-35.25"s,1);
    const hexadecimal z("10::5.4"s,1);
    const hexadecimal result3("16::-2290.da"s,2), result4("16::545.4b"s,2);
    REQUIRE( y*x == result3 );
    x*=z;
    REQUIRE( x == result4 );
}

TEST_CASE("Division"){
    decimal a("12.145"),b("-54.2564");
    const decimal c("12");
    const decimal result1("1.012"),result2("-4.4673");
    decimal::scale = 3;
    REQUIRE( a/c == result1 );
    decimal::scale = 4;
    b /= a;
    REQUIRE( b == result2 );
    // Step 6 test case:
    decimal::scale = 0;
    a = 4110;
    b = 588;
    REQUIRE( a/b == decimal(6) );
    // pure fractional dividend case:
    decimal::scale = 3;
    a = decimal("0.009");
    b = 3;
    REQUIRE( a/b == decimal("0.003"));
    // pure fractional divisor case:
    decimal::scale = 0;
    a = 1;
    b = decimal("0.005");
    REQUIRE( a/b == decimal(200) );
}

TEST_CASE("Modulo"){
    decimal a("12.145"),b("-54.2564");
    const decimal c("12");
    const decimal result1("0.001"),result2("-0.0010415");
    decimal::scale = 3;
    REQUIRE( a%c == result1 );
    decimal::scale = 4;
    b %= a;
    REQUIRE( b == result2 );
}

TEST_CASE("Binomial"){
    const duodecimal r1(56);
    const decimal r2("92.20573984375"s),tolerance("0.00001");
    const decimal r3("22.0229561046886265625");
    const decimal n3("12.3"),k3(11);
    REQUIRE(duodecimal::binomial(8,3) == r1 );
    decimal::scale = 10;
    const decimal b2 = decimal::binomial(decimal("8.45"),4);
    const decimal b3 = decimal::binomial(n3,k3);
    // scale affects precision, do not expect exact value
    REQUIRE( b2 > (r2-tolerance) );
    REQUIRE( b2 < (r2+tolerance) );
    REQUIRE( b3 > (r3-tolerance) );
    REQUIRE( b3 < (r3+tolerance) );
}

TEST_CASE("Power"){
    // Integer powers
    decimal a("-1.5"),b(2),e(15);
    const decimal c(3),d(15),o(1),z(0);
    const decimal result1("-3.300"),result2("32768");
    REQUIRE( std::pow(a,c) == result1 );
    b.pow(d);
    REQUIRE( b == result2 );
    // Real powers
    decimal::scale = 50;
    decimal realPow1("0.5"), realPow2("0.73"),realPow3("12.4");
    const decimal realExp1("-1.25"),realExp2("1.2345678"),realExp3("-1.4");
    const decimal realRes1("2.378414230005442"), tolerance0("0.001");
    const decimal realRes2("0.6780516921132284"),realRes3("0.029458432955048974");
    const decimal tolerance1("0.000001");
    realPow1.pow(realExp1);
    realPow2.pow(realExp2);
    // 3rd case fails with default precision (deviation exceeds tolerance)
    // causes long calculation - real_pow is O(n^2) over precision
    realPow3.pow(realExp3,200);
    REQUIRE( realPow1 > realRes1-tolerance0 );
    REQUIRE( realPow1 < realRes1+tolerance0 );
    REQUIRE( realPow2 > realRes2-tolerance1 );
    REQUIRE( realPow2 < realRes2+tolerance1 );
    REQUIRE( realPow3 > realRes3-tolerance1 );
    REQUIRE( realPow3 < realRes3+tolerance1 );
    // Corner cases
    // x^0, 1^n, (-1)^n - n from N, x from R
    const decimal minus1(-1);
    REQUIRE( std::pow(realPow1,z) == o );
    REQUIRE( std::pow(o,result2) == o);
    REQUIRE( std::pow(minus1,decimal(-124)) == o);
    REQUIRE( std::pow(minus1,decimal(731)) == minus1 );
}

TEST_CASE("Floor"){
    decimal a("12.145"),b("-54.2564");
    decimal c("12.000"),d("-55.000");
    const decimal acexp(12),bdexp(-55);
    REQUIRE( std::floor(a) == acexp );
    b.floor();
    REQUIRE( b == bdexp );
    c.floor();
    REQUIRE( c == acexp);
    REQUIRE( std::floor(d) == bdexp );
}

TEST_CASE("Ceil"){
    decimal a("12.145"),b("-54.2564");
    decimal c("13.000"),d("-54.000");
    const decimal acexp(13),bdexp(-54);
    REQUIRE( std::ceil(a) == acexp );
    b.ceil();
    REQUIRE( b == bdexp );
    c.ceil();
    REQUIRE( c == acexp);
    REQUIRE( std::ceil(d) == bdexp );
}

TEST_CASE("Trunc"){
    decimal a("12.145"),b("-54.2564");
    decimal c("12.000"),d("-54.000");
    const decimal acexp(12),bdexp(-54);
    REQUIRE( std::trunc(a) == acexp );
    b.trunc();
    REQUIRE( b == bdexp );
    c.trunc();
    REQUIRE( c == acexp);
    REQUIRE( std::trunc(d) == bdexp );

}
