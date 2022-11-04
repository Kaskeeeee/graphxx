/**
 * @brief This file is essential for the Catch2 library to work properly.
 * This tells Catch2 to provide a main() for the test environment.
 * WARNING! This must not be done in other cpp files.
 * Each additional file need only `#include "catch2/catch.hpp"`- do not repeat
 * the `#define`.
 *
 */

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#undef CATCH_CONFIG_MAIN
