#!/bin/sh -xe

[ -x "$PROGRAM_PATH" ]

TMP_PATH=$(mktemp -d "$TEST_PATH/test-valid-input.XXXXXX")
mkdir -p "$TMP_PATH"

for INPUT_FILE_PATH in "$TEST_PATH"/files/valid-*.sm; {
    "$PROGRAM_PATH" --format=swift --dir="$TMP_PATH" "$INPUT_FILE_PATH"
    OUTPUT_FILE_PATH="$TMP_PATH"/$(basename ${INPUT_FILE_PATH%.sm}.swift)
    [ -f "$OUTPUT_FILE_PATH" ]
    [ -s "$OUTPUT_FILE_PATH" ]
    rm "$OUTPUT_FILE_PATH"

    "$PROGRAM_PATH" --format=kotlin --dir="$TMP_PATH" "$INPUT_FILE_PATH"
    OUTPUT_FILE_PATH="$TMP_PATH"/$(basename ${INPUT_FILE_PATH%.sm}.kt)
    [ -f "$OUTPUT_FILE_PATH" ]
    [ -s "$OUTPUT_FILE_PATH" ]
    rm "$OUTPUT_FILE_PATH"

    "$PROGRAM_PATH" --format=dotgraph --dir="$TMP_PATH" "$INPUT_FILE_PATH"
    OUTPUT_FILE_PATH="$TMP_PATH"/$(basename ${INPUT_FILE_PATH%.sm}.dot)
    [ -f "$OUTPUT_FILE_PATH" ]
    [ -s "$OUTPUT_FILE_PATH" ]
    rm "$OUTPUT_FILE_PATH"
}

rmdir "$TMP_PATH"
