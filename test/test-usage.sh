#!/bin/sh -xe

[ -x "$PROGRAM_PATH" ]
"$PROGRAM_PATH" && true
[ $? -eq 1 ]
