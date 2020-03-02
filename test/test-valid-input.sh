#!/bin/sh -xe

[ -x "$PROGRAM_PATH" ]

for INPUT_FILE_PATH in "$TEST_PATH"/files/valid-*.sm; {
    "$PROGRAM_PATH" --format=swift --dir=. "$INPUT_FILE_PATH"
    OUTPUT_FILE_PATH=$(basename ${INPUT_FILE_PATH%.sm}.swift)
    [ -f "$OUTPUT_FILE_PATH" ]
    [ -s "$OUTPUT_FILE_PATH" ]
    rm -f "$OUTPUT_FILE_PATH"
}
