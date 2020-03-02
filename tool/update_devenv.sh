#!/bin/sh -e

command -v automake > /dev/null && command -v aclocal > /dev/null || {
    echo "Error: automake not found."
    echo "If you're using Homebrew on macOS, you may want to try:"
    echo "    brew install automake"
    exit 1
}

command -v autoconf > /dev/null || {
    echo "Error: autoconf not found."
    echo "If you're using Homebrew on macOS, you may want to try:"
    echo "    brew install autoconf"
    exit 1
}

aclocal
automake --add-missing --copy
autoheader
autoconf
