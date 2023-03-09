#!/usr/bin/env bash

# TODO: use Bazel to build instead of doing this manually
# See here as an example: https://github.com/ElectricRCAircraftGuy/eRCaGuy_gtest_practice

FULL_PATH_TO_SCRIPT="$(realpath "${BASH_SOURCE[-1]}")"
SCRIPT_DIRECTORY="$(dirname "$FULL_PATH_TO_SCRIPT")"
SCRIPT_FILENAME="$(basename "$FULL_PATH_TO_SCRIPT")"

RETURN_CODE_SUCCESS=0
RETURN_CODE_ERROR=1

SRC_FILES=(
    "src/evtol_simulation.cpp"
)
EXECUTABLE_NAME="evtol_simulation"

main() {
    echo "Building and running $EXECUTABLE_NAME."
    cd "$SCRIPT_DIRECTORY"
    mkdir -p bin

    echo "Building..."
    time ccache g++ -Wall -Wextra -Werror -O3 -std=gnu++17 "${SRC_FILES[@]}" -o "bin/$EXECUTABLE_NAME"

    return_code="$?"
    if [ "$return_code" -eq 0 ]; then
        echo -e "\nRunning..."
        "bin/$EXECUTABLE_NAME"
    else
        echo "Failed to build."
        exit "$RETURN_CODE_ERROR"
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
    main
fi
