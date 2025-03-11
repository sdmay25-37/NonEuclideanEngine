//
//  Author: tagrinnell
//  3/11/25
//
//  Example Using Catch2WithMain Testing.  This is an example test case from the Catch2
//  authors, meant to test the CMake integration.
//
#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include <cmath>

static double sum(int x, int y) {
    return x + y;
}

TEST_CASE( "Add 0 + 1") {
    REQUIRE (sum(0, 1) == 1);
}

TEST_CASE( "" ) {
    REQUIRE (sum(24, 32) == 56);
}