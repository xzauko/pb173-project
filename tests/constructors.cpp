#include <fixedpoint.h>

#include <string>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using namespace fixedpoint;
using namespace std::literals;

TEST_CASE("Default constructor"){
    decimal n1, test0(0);
    REQUIRE( n1 == test0 );
    REQUIRE( n1.str() == "10::0"s );
}

TEST_CASE("Integral numbers constructor"){
    short int ss = -31421;
    unsigned short us = 64837;
    signed char sc =  -54;
    unsigned char uc = 189;
    decimal n10('0');
    hexadecimal n16(-255);
    binary n2(6553635842296ull);
    number<13> n13(true);
    number<24> n24(-47589234944896ll);
    number<30> n30(9675785ul);
    number<27> n27(169778u);
    number<21> n21(-89124);
    duodecimal n12(us);
    number<6> n6(ss);
    octal n8(sc);
    number<3> n3(uc);
    REQUIRE( n10.str() == "10::48"s );
    REQUIRE( n16.str() == "16::-ff"s );
    REQUIRE( n2.str() == "2::1011111010111100011001000101110100011111000"s );
    REQUIRE( n13.str() == "13::1"s );
    REQUIRE( n24.str() == "24::-i0800mal2g"s);
    REQUIRE( n30.str() == "30::bsaq5"s);
    REQUIRE( n27.str() == "27::8go2"s);
    REQUIRE( n21.str() == "21::-9d20"s);
    REQUIRE( n12.str() == "12::31631"s);
    REQUIRE( n6.str() == "6::-401245"s);
    REQUIRE( n8.str() == "8::-66"s);
    REQUIRE( n3.str() == "3::21000"s);
}

TEST_CASE("Integral aliases"){
    using namespace std;
    int16_t ss = -31421;
    uint16_t us = 64837;
    int8_t sc =  -54;
    uint8_t uc = 189;
    int32_t si = -89124;
    uint32_t ui = 169778;
    int64_t sll = -47589234944896;
    uint64_t ull = 6553635842296ull;
    binary n2(ull);
    number<24> n24(sll);
    number<27> n27(ui);
    number<21> n21(si);
    duodecimal n12(us);
    number<6> n6(ss);
    octal n8(sc);
    number<3> n3(uc);
    REQUIRE( n2.str() == "2::1011111010111100011001000101110100011111000"s );
    REQUIRE( n24.str() == "24::-i0800mal2g"s);
    REQUIRE( n27.str() == "27::8go2"s);
    REQUIRE( n21.str() == "21::-9d20"s);
    REQUIRE( n12.str() == "12::31631"s);
    REQUIRE( n6.str() == "6::-401245"s);
    REQUIRE( n8.str() == "8::-66"s);
    REQUIRE( n3.str() == "3::21000"s);
}

TEST_CASE("String constructor"){
    number<30> n30("10::124,114"s);
    number<21> n21("-21::5gdf.dddg",2);
    number<16> n16("10::124.11489"s,7);
    number<12> n12("-12::5ab.841b");
    REQUIRE( n30.str() == "30::44.3ci"s );
    REQUIRE( n21.str() == "21::-5gdf.dd"s );
    REQUIRE( n16.str() == "16::7c.1d696e5"s );
    REQUIRE( n12.str() == "12::-5ab.841b"s );
}

TEST_CASE("Move and copy constructors"){
    decimal cp(255),mv(-85);
    decimal n1(cp),n2(std::move(mv));
    REQUIRE( n1.str() == "10::255"s );
    REQUIRE( n2.str() == "10::-85"s );
    REQUIRE( cp.str() == "10::255"s );
}

TEST_CASE("Operator ="){
    hexadecimal n1,n2,n3;
    n1 = 25; // impicit conversion to number, then copy
    n2 = -888888888888888888ll;
    n3 = 0xffffffffffffffffull;
    REQUIRE( n1.str() == "16::19"s );
    REQUIRE( n2.str() == "16::-c55f7bc23038e38"s );
    REQUIRE( n3.str() == "16::ffffffffffffffff"s );
    n1 = hexadecimal("12::-11ab.1243",8); // move
    n2 = n3; // copy
    REQUIRE( n1.str() == "16::-7d3.1984bda1"s );
    REQUIRE( n2       == n3 );
    REQUIRE( n2.str() == "16::ffffffffffffffff"s );
}


