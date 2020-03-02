//
//  main.cpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#include "Command.hpp"

int main(int argc, const char* argv[]) {
    Command command(argc, argv);
    return command.execute();
}
