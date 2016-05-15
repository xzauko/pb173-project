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
    const hexadecimal result3("16::-2290.da0"s,3), result4("16::545.4b"s,2);
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

TEST_CASE("Power"){
    decimal a("-1.5"),b(2),e(15);
    const decimal c(3),d(15),o(1),t(2);
    const decimal result1("-3.30"),result2("32768");
    REQUIRE( std::pow(a,c) == result1 );
    b.pow(d);
    REQUIRE( b == result2 );
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
