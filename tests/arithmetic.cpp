#include <fixedpoint.h>

#include <string>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using namespace fixedpoint;
using namespace std::literals;

TEST_CASE("Addition same signs"){
    decimal a(50),b(45);
    const decimal c(5);
    const decimal result1(55),result2(105);
    a += c;
    REQUIRE( a == result1 );
    REQUIRE( b+c+c == result1 );
    (b += c) += a;
    REQUIRE( b == result2 );
    a+=a;
    REQUIRE( a == b+c );

    hexadecimal x(-50),y(-45);
    const hexadecimal z(-5);
    const hexadecimal result3(-55),result4(-105);
    x += z;
    REQUIRE( x == result3 );
    REQUIRE( y+z+z == result3 );
    (y += z) += x;
    REQUIRE( y == result4 );
}

TEST_CASE("Subtraction same signs"){
    decimal a(50),b(45);
    const decimal c(5);
    const decimal result1(-5),result2(35), zero(0);
    a -= c;
    REQUIRE( a == b );
    REQUIRE( b-c-a == result1 );
    (b -= c) -= c;
    REQUIRE( b == result2 );
    a-=a;
    REQUIRE( a == zero );

    hexadecimal x(-50),y(-45);
    const hexadecimal z(-5);
    const hexadecimal result3(5),result4(-35);
    x -= z;
    REQUIRE( x == y );
    REQUIRE( y-z-x == result3 );
    (y -= z) -= z;
    REQUIRE( y == result4 );
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
