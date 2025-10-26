#!/bin/bash

# shellcheck disable=SC2039

DIR=bin

if [ -d $DIR ]; then
    pushd $DIR > /dev/null || exit

    # Detect platform using same logic as Makefile
    UNAME_S=$(uname -s 2>/dev/null || echo "Windows")

    case "$UNAME_S" in
        Darwin|Linux)
            # macOS or Linux - look for test_* files without extension (skip .o files)
            # Check depth 1 first (for backward compatibility), then depth 2 (subdirectories)
            files=$(find . -maxdepth 2 -type f -name "test_*" ! -name "*.o" 2>/dev/null | sed 's|^\./||')
            ;;
        *)
            # Windows - look in subdirectories (gcc/ and clang/)
            files=$(find . -maxdepth 2 -type f -name "*.exe" 2>/dev/null | sed 's|^\./||')
            ;;
    esac

    for file in $files; do
        echo "$file"
        ./"$file"
    done

    popd > /dev/null || exit
else
    echo Directory \'$DIR\' does not exist >&2
    exit 1
fi
