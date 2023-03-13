/*
General utilities module
*/

#pragma once

// local includes
// NA

// Linux includes
// NA

// C++ includes
#include <cstdio>


/// Debug printf function.
/// See https://stackoverflow.com/a/67667132/4561887
#ifdef DEBUG
    #define DEBUG_PRINTF(...) printf("DEBUG: " __VA_ARGS__)
#else
    #define DEBUG_PRINTF(...) \
        do                    \
        {                     \
        } while (0)
#endif
