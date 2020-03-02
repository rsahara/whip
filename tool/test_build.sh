#!/bin/sh -e

SCRIPTDIR=$(cd $(dirname "$0") && pwd)
INITDIR=$(pwd)

TMPDIR=$(mktemp -d /tmp/build.whip.XXXXXX)
echo "TMPDIR: $TMPDIR"
cd "$TMPDIR"

PREFIXDIR="$TMPDIR/install"
mkdir -p "$PREFIXDIR"

eval "$SCRIPTDIR/../configure" --prefix="\"$PREFIXDIR\""
make -j8
make install
make check

echo "Done"
cd "$INITDIR"
rm -fr "$TMPDIR"
