#include <fixedpoint.h>

#include <string>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using namespace fixedpoint;
using namespace std::literals;

template<unsigned char radix>
std::size_t number<radix>::scale = 0;

TEST_CASE("String constructor exc"){
    bool correctExc=false;
    try{
        decimal n1("48::25");
    }
    catch( radix_invalid & e){
        correctExc = true;
    }
    catch( ... ){
        correctExc = false;
    }
    REQUIRE( correctExc);
    correctExc = false;
    try{
        decimal n2("1::000");
    }
    catch( radix_invalid & e){
        correctExc = true;
    }
    catch( ... ){
        correctExc = false;
    }
    REQUIRE( correctExc);
    correctExc = false;
    try{
        decimal n2("10::aaa.bbb");
    }
    catch( invalid_number_format & e){
        if(e.what() == "invalid characters found in input string"s) {
            correctExc = true;
        }
        else correctExc = false;
    }
    catch( ... ){
        correctExc = false;
    }
    REQUIRE( correctExc);
    correctExc = false;
    try{
        decimal n2("10a::000");
    }
    catch( invalid_number_format & e){
        if(e.what() == "src's radix is ill formatted"s) {
            correctExc = true;
        }
        else correctExc = false;
    }
    catch( ... ){
        correctExc = false;
    }
    REQUIRE( correctExc);
    correctExc = false;
    try{
        decimal n2("10::000.12,45");
    }
    catch( invalid_number_format & e){
        if(e.what() == "contains more than one decimal separator"s) {
            correctExc = true;
        }
        else correctExc = false;
    }
    catch( ... ){
        correctExc = false;
    }
    REQUIRE( correctExc);
    correctExc = false;
    try{
        decimal n2("10:12,45");
    }
    catch( invalid_number_format & e){
        if(e.what() == "missing 2nd colon in separator"s) {
            correctExc = true;
        }
        else correctExc = false;
    }
    catch( ... ){
        correctExc = false;
    }
    REQUIRE( correctExc);
}

TEST_CASE("Division by zero"){
    decimal a(125),b(0);
    bool correctExc=false;
    try{
       a /= b;
    }
    catch(division_by_zero & e){
        correctExc = true;
    }
    catch( ... ){
        correctExc = false;
    }
    REQUIRE( correctExc );
}

TEST_CASE("Unsupported operations"){
    decimal a(125),b("0.125");
    bool correctExc=false;
    try{
       std::pow(a,b);
    }
    catch(unsupported_operation & e){
        if( e.what() == "Only integer exponent is suported for power function!"s ){
            correctExc = true;
        }
        else correctExc = false;
    }
    catch( ... ){
        correctExc = false;
    }
    REQUIRE( correctExc );

    // caveat not tested !! - is expected to be fixed
}


