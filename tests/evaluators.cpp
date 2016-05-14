#include <fixedpoint.h>

#include <string>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using namespace fixedpoint;
using namespace std::literals;

TEST_CASE("Postfix eval"){
    decimal easy = decimal::eval_postfix("9 2 @pow 183 + 21 -");
    decimal hard = decimal::eval_postfix("9 2 @pow 183 + 21 - 84 % -75.124 @ceil +");
    decimal answerEasy(243), answerHard(0);
    REQUIRE( easy == answerEasy );
    REQUIRE( hard == answerHard );
}

TEST_CASE("Infix eval"){
    decimal easy = decimal::eval_infix("@pow ( 9 , 2 ) + 183 - 21");
    decimal medi = decimal::eval_postfix("( @pow ( 9 , 2 ) + 183 - 21 ) % 84 + @ceil ( -75.124 )");
    decimal hard = decimal::eval_postfix("3 + @pow ( 4 * -2,3 ) / @pow [ ( 1 - 5 ) , @pow ( 2 , 2 ) ]");
    decimal answerEasy(243), answerMedi(0), answerHard(-2045);
    REQUIRE( easy == answerEasy );
    REQUIRE( medi == answerMedi );
    REQUIRE( hard == answerHard );
}
