#!/bin/sh -xe

[ -x "$PROGRAM_PATH" ]
TMP_PATH=$(mktemp -d)
INPUT_FILE_PATH="$TEST_PATH/files/valid-sample.sm"

cp -R "$TEST_PATH/files/swift/valid-sample" "$TMP_PATH"
chmod -R +w "$TMP_PATH/valid-sample"
"$PROGRAM_PATH" --format=swift --dir="$TMP_PATH/valid-sample/Sources/swift-valid-sample" "$INPUT_FILE_PATH"
"$PROGRAM_PATH" --format=swiftenum --param=Page --dir="$TMP_PATH/valid-sample/Sources/swift-valid-sample" "$INPUT_FILE_PATH"

WD=`pwd`
cd "$TMP_PATH/valid-sample"
swift run | tee "$TMP_PATH/valid-sample.out"
diff "$TMP_PATH/valid-sample.out" "$TEST_PATH/files/swift/valid-sample.expected-out"

cd "$WD"
rm -fr "$TMP_PATH"
