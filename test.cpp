#include <iostream>
#include <string>

#include "argParser.h"

using namespace std;

int main(int argc, char **argv) {
    ArgParser parser;
    parser.set_opt(INT);
    parser.set_opt(STR);
    parser.set_opt("volumn", 'v', STR);
    parser.set_opt("", 'n', BOOL);
    parser.parse(argc, argv);
    cout << parser[1].get_int() << endl
        << parser[2].get_str() << endl
        << parser['v'].get_str() << endl
        << parser['n'].get_bool() << endl;
}