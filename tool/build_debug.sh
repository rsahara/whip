#!/bin/sh -e

SCRIPTDIR=$(cd $(dirname "$0") && pwd)

TMPDIR=$(mktemp -d /tmp/build.whip.XXXXXX)
echo "TMPDIR: $TMPDIR"
cd "$TMPDIR"

PREFIXDIR="$TMPDIR/install"
mkdir -p "$PREFIXDIR"

eval "$SCRIPTDIR/../configure" --prefix="\"$PREFIXDIR\"" CPPFLAGS="-DDEBUG" CXXFLAGS="-g"
make
make install

echo "Built in: $TMPDIR"
echo "Installed in: $PREFIXDIR"
cd "$INITDIR"
