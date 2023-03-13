#!/usr/bin/env bash

# TODO: use Bazel to build instead of doing this manually
# See here as an example: https://github.com/ElectricRCAircraftGuy/eRCaGuy_gtest_practice

FULL_PATH_TO_SCRIPT="$(realpath "${BASH_SOURCE[-1]}")"
SCRIPT_DIRECTORY="$(dirname "$FULL_PATH_TO_SCRIPT")"
SCRIPT_FILENAME="$(basename "$FULL_PATH_TO_SCRIPT")"

RETURN_CODE_SUCCESS=0
RETURN_CODE_ERROR=1

SRC_FILES_COMMON=(
    "src/simulation.cpp"
    "src/vehicle.cpp"
)


build_and_run_program()
{
    SRC_FILES=(
        "src/main.cpp"
        "${SRC_FILES_COMMON[@]}"
    )
    CUSTOM_DEFINES=(
        # uncomment to turn debug prints ON throughout the whole program (see "utils.h")
        "-DDEBUG"
    )
    EXECUTABLE_NAME="evtol_simulation"

    echo "================================================="
    echo "Building and running $EXECUTABLE_NAME."
    echo "================================================="
    cd "$SCRIPT_DIRECTORY"
    mkdir -p bin

    echo "Building..."
    time ccache g++ -Wall -Wextra -Werror -O3 -std=gnu++17 "${CUSTOM_DEFINES[@]}" \
        "${SRC_FILES[@]}" -I"src" -o "bin/$EXECUTABLE_NAME"

    return_code="$?"
    if [ "$return_code" -eq 0 ]; then
        echo -e "\nRunning..."
        "bin/$EXECUTABLE_NAME"
    else
        echo "Failed to build."
        exit "$RETURN_CODE_ERROR"
    fi
}

build_and_run_unit_tests()
{
    SRC_FILES=(
        "src/main_unittest.cpp"
        "${SRC_FILES_COMMON[@]}"
    )
    CUSTOM_DEFINES=(
        # uncomment to turn debug prints ON throughout the whole program (see "utils.h")
        # "-DDEBUG"
    )
    EXECUTABLE_NAME="evtol_simulation_unittest"


    echo "================================================="
    echo "Building and running $EXECUTABLE_NAME."
    echo "================================================="
    cd "$SCRIPT_DIRECTORY"
    mkdir -p bin

    echo "Building..."
    time ccache g++ -Wall -Wextra -Werror -O3 -std=gnu++17 -pthread "${CUSTOM_DEFINES[@]}" \
        "${SRC_FILES[@]}" -lgtest -lgtest_main -o "bin/$EXECUTABLE_NAME"

    return_code="$?"
    if [ "$return_code" -eq 0 ]; then
        echo -e "\nRunning..."
        "bin/$EXECUTABLE_NAME"
    else
        echo "Failed to build."
        exit "$RETURN_CODE_ERROR"
    fi
}

main() {
    if [ "$#" -eq 0 ]; then
        # if no args, build and run both
        echo "No arguments; building and running both unit tests and the main program."
        build_and_run_unit_tests
        build_and_run_program
    elif [ "$1" = "tests" ]; then
        # build and run tests only
        echo "Building and running unit tests only."
        build_and_run_unit_tests
    elif [ "$1" = "main" ]; then
        # build and run the main program only
        echo "Building and running the main program only."
        build_and_run_program
    fi
}

# Determine if the script is being sourced or executed (run).
# See:
# 1. "eRCaGuy_hello_world/bash/if__name__==__main___check_if_sourced_or_executed_best.sh"
# 1. My answer: https://stackoverflow.com/a/70662116/4561887
if [ "${BASH_SOURCE[0]}" = "$0" ]; then
    # This script is being run.
    __name__="__main__"
else
    # This script is being sourced.
    __name__="__source__"
fi

# Only run `main` if this script is being **run**, NOT sourced (imported).
# - See my answer: https://stackoverflow.com/a/70662116/4561887
if [ "$__name__" = "__main__" ]; then
    main "$@"
fi
