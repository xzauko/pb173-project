#include <fixedpoint.h>

#include <string>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using namespace fixedpoint;
using namespace std::literals;

TEST_CASE("Less than"){
    hexadecimal negeighth("2::-0.001",3), poseighth("2::0.001",2);
    hexadecimal possixteenth("10::0.0625",3), negsixteenth("10::-0.0625",2);
    REQUIRE( negeighth < poseighth );
    REQUIRE( negeighth < possixteenth );
    REQUIRE( negeighth < negsixteenth );
    REQUIRE_FALSE( negeighth < negeighth );
    REQUIRE( negsixteenth < poseighth );
    REQUIRE( negsixteenth < possixteenth );
    REQUIRE_FALSE( negsixteenth < negsixteenth );
    REQUIRE_FALSE( negsixteenth < negeighth );
    REQUIRE( possixteenth < poseighth );
    REQUIRE_FALSE( possixteenth < possixteenth );
    REQUIRE_FALSE( possixteenth < negsixteenth );
    REQUIRE_FALSE( possixteenth < negeighth );
    REQUIRE_FALSE( poseighth < poseighth );
    REQUIRE_FALSE( poseighth < possixteenth );
    REQUIRE_FALSE( poseighth < negsixteenth );
    REQUIRE_FALSE( poseighth < negeighth );
}

TEST_CASE("Less than or equal"){
    hexadecimal negeighth("2::-0.001",3), poseighth("2::0.001",2);
    hexadecimal possixteenth("10::0.0625",3), negsixteenth("10::-0.0625",2);
    REQUIRE( negeighth <= poseighth );
    REQUIRE( negeighth <= possixteenth );
    REQUIRE( negeighth <= negsixteenth );
    REQUIRE( negeighth <= negeighth );
    REQUIRE( negsixteenth <= poseighth );
    REQUIRE( negsixteenth <= possixteenth );
    REQUIRE( negsixteenth <= negsixteenth );
    REQUIRE_FALSE( negsixteenth <= negeighth );
    REQUIRE( possixteenth <= poseighth );
    REQUIRE( possixteenth <= possixteenth );
    REQUIRE_FALSE( possixteenth <= negsixteenth );
    REQUIRE_FALSE( possixteenth <= negeighth );
    REQUIRE( poseighth <= poseighth );
    REQUIRE_FALSE( poseighth <= possixteenth );
    REQUIRE_FALSE( poseighth <= negsixteenth );
    REQUIRE_FALSE( poseighth <= negeighth );
}

TEST_CASE("Greater than"){
    hexadecimal negeighth("2::-0.001",3), poseighth("2::0.001",2);
    hexadecimal possixteenth("10::0.0625",3), negsixteenth("10::-0.0625",2);
    REQUIRE_FALSE( negeighth > poseighth );
    REQUIRE_FALSE( negeighth > possixteenth );
    REQUIRE_FALSE( negeighth > negsixteenth );
    REQUIRE_FALSE( negeighth > negeighth );
    REQUIRE_FALSE( negsixteenth > poseighth );
    REQUIRE_FALSE( negsixteenth > possixteenth );
    REQUIRE_FALSE( negsixteenth > negsixteenth );
    REQUIRE( negsixteenth > negeighth );
    REQUIRE_FALSE( possixteenth > poseighth );
    REQUIRE_FALSE( possixteenth > possixteenth );
    REQUIRE( possixteenth > negsixteenth );
    REQUIRE( possixteenth > negeighth );
    REQUIRE_FALSE( poseighth > poseighth );
    REQUIRE( poseighth > possixteenth );
    REQUIRE( poseighth > negsixteenth );
    REQUIRE( poseighth > negeighth );
}

TEST_CASE("Greater than or equal"){
    hexadecimal negeighth("2::-0.001",3), poseighth("2::0.001",2);
    hexadecimal possixteenth("10::0.0625",3), negsixteenth("10::-0.0625",2);
    REQUIRE_FALSE( negeighth >= poseighth );
    REQUIRE_FALSE( negeighth >= possixteenth );
    REQUIRE_FALSE( negeighth >= negsixteenth );
    REQUIRE( negeighth >= negeighth );
    REQUIRE_FALSE( negsixteenth >= poseighth );
    REQUIRE_FALSE( negsixteenth >= possixteenth );
    REQUIRE( negsixteenth >= negsixteenth );
    REQUIRE( negsixteenth >= negeighth );
    REQUIRE_FALSE( possixteenth >= poseighth );
    REQUIRE( possixteenth >= possixteenth );
    REQUIRE( possixteenth >= negsixteenth );
    REQUIRE( possixteenth >= negeighth );
    REQUIRE( poseighth >= poseighth );
    REQUIRE( poseighth >= possixteenth );
    REQUIRE( poseighth >= negsixteenth );
    REQUIRE( poseighth >= negeighth );
}

TEST_CASE("Equal"){
    hexadecimal negeighth("2::-0.001",3), poseighth("2::0.001",2);
    hexadecimal possixteenth("10::0.0625",3), negsixteenth("10::-0.0625",2);
    REQUIRE_FALSE( negeighth == poseighth );
    REQUIRE_FALSE( negeighth == possixteenth );
    REQUIRE_FALSE( negeighth == negsixteenth );
    REQUIRE( negeighth == negeighth );
    REQUIRE_FALSE( negsixteenth == poseighth );
    REQUIRE_FALSE( negsixteenth == possixteenth );
    REQUIRE( negsixteenth == negsixteenth );
    REQUIRE_FALSE( negsixteenth == negeighth );
    REQUIRE_FALSE( possixteenth == poseighth );
    REQUIRE( possixteenth == possixteenth );
    REQUIRE_FALSE( possixteenth == negsixteenth );
    REQUIRE_FALSE( possixteenth == negeighth );
    REQUIRE( poseighth == poseighth );
    REQUIRE_FALSE( poseighth == possixteenth );
    REQUIRE_FALSE( poseighth == negsixteenth );
    REQUIRE_FALSE( poseighth == negeighth );
}

TEST_CASE("Not equal"){
    hexadecimal negeighth("2::-0.001",3), poseighth("2::0.001",2);
    hexadecimal possixteenth("10::0.0625",3), negsixteenth("10::-0.0625",2);
    REQUIRE( negeighth != poseighth );
    REQUIRE( negeighth != possixteenth );
    REQUIRE( negeighth != negsixteenth );
    REQUIRE_FALSE( negeighth != negeighth );
    REQUIRE( negsixteenth != poseighth );
    REQUIRE( negsixteenth != possixteenth );
    REQUIRE_FALSE( negsixteenth != negsixteenth );
    REQUIRE( negsixteenth != negeighth );
    REQUIRE( possixteenth != poseighth );
    REQUIRE_FALSE( possixteenth != possixteenth );
    REQUIRE( possixteenth != negsixteenth );
    REQUIRE( possixteenth != negeighth );
    REQUIRE_FALSE( poseighth != poseighth );
    REQUIRE( poseighth != possixteenth );
    REQUIRE( poseighth != negsixteenth );
    REQUIRE( poseighth != negeighth );
}
