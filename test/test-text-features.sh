#!/bin/sh -xe

[ -x "$TESTER_PATH" ]

[ `"$TESTER_PATH" lowerCamelCaseIdentifier abcDEFGhi` = abcDEFGhi ]
[ `"$TESTER_PATH" lowerCamelCaseIdentifier AbcDEFGhi` = abcDEFGhi ]
[ `"$TESTER_PATH" lowerCamelCaseIdentifier ABCdefGhi` = abCdefGhi ]
[ `"$TESTER_PATH" lowerCamelCaseIdentifier ABC` = abc ]
[ `"$TESTER_PATH" lowerCamelCaseIdentifier abc` = abc ]
[ `"$TESTER_PATH" lowerCamelCaseIdentifier AB` = ab ]
[ `"$TESTER_PATH" lowerCamelCaseIdentifier ab` = ab ]
[ `"$TESTER_PATH" lowerCamelCaseIdentifier Ab` = ab ]
[ `"$TESTER_PATH" lowerCamelCaseIdentifier aB` = aB ]
[ `"$TESTER_PATH" lowerCamelCaseIdentifier A` = a ]
[ `"$TESTER_PATH" lowerCamelCaseIdentifier a` = a ]

[ `"$TESTER_PATH" upperCamelCaseIdentifier abcDEFGhi` = AbcDEFGhi ]
[ `"$TESTER_PATH" upperCamelCaseIdentifier AbcDEFGhi` = AbcDEFGhi ]
[ `"$TESTER_PATH" upperCamelCaseIdentifier ABCdefGhi` = ABCdefGhi ]
[ `"$TESTER_PATH" upperCamelCaseIdentifier ABC` = ABC ]
[ `"$TESTER_PATH" upperCamelCaseIdentifier abc` = Abc ]
[ `"$TESTER_PATH" upperCamelCaseIdentifier AB` = AB ]
[ `"$TESTER_PATH" upperCamelCaseIdentifier ab` = Ab ]
[ `"$TESTER_PATH" upperCamelCaseIdentifier Ab` = Ab ]
[ `"$TESTER_PATH" upperCamelCaseIdentifier aB` = AB ]
[ `"$TESTER_PATH" upperCamelCaseIdentifier A` = A ]
[ `"$TESTER_PATH" upperCamelCaseIdentifier a` = A ]

[ `"$TESTER_PATH" identifierCapitalizedWithUnderscores abcDEFGhi` = ABC_DEF_GHI ]
[ `"$TESTER_PATH" identifierCapitalizedWithUnderscores AbcDEFGhi` = ABC_DEF_GHI ]
[ `"$TESTER_PATH" identifierCapitalizedWithUnderscores ABCdefGhi` = AB_CDEF_GHI ]
[ `"$TESTER_PATH" identifierCapitalizedWithUnderscores ABC` = ABC ]
[ `"$TESTER_PATH" identifierCapitalizedWithUnderscores abc` = ABC ]
[ `"$TESTER_PATH" identifierCapitalizedWithUnderscores AB` = AB ]
[ `"$TESTER_PATH" identifierCapitalizedWithUnderscores ab` = AB ]
[ `"$TESTER_PATH" identifierCapitalizedWithUnderscores Ab` = AB ]
[ `"$TESTER_PATH" identifierCapitalizedWithUnderscores aB` = A_B ]
[ `"$TESTER_PATH" identifierCapitalizedWithUnderscores abC` = AB_C ]
[ `"$TESTER_PATH" identifierCapitalizedWithUnderscores aBC` = A_BC ]
[ `"$TESTER_PATH" identifierCapitalizedWithUnderscores A` = A ]
[ `"$TESTER_PATH" identifierCapitalizedWithUnderscores a` = A ]
