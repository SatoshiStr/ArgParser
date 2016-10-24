#include "argParser.h"

using namespace std;


void ArgParser::parse(int argc, char **argv) {
    int pos = 0;
    for(int i = 1; i < argc; ++i) {
        bool isKeyArg = false;
        char argShortName;
        string argLongName;
        if(argv[i][0] == '-' && argv[i][1] == '-') {
            // --
            string name = string(&argv[i][2]);
            if(translateTable.find(name) == translateTable.end()) {
                throw ParseError("Arg '" + name + "' is not defined.");
            }
            char shortName = translateTable[name];
            if(keyParameters[shortName].type == BOOL) {
                Value val;
                val.set_bool(true);
                keyArgs[shortName] = val;
            } else {
                isKeyArg = true;
                argShortName = shortName;
                argLongName = name;
            }
        } else if(argv[i][0] == '-') {
            // -
            int j = 1;
            while(argv[i][j] != '\0') {
                if(keyParameters.find(argv[i][j]) == keyParameters.end()) {
                    throw ParseError("Arg '" + string(1, argv[i][j]) + "' is not defined.");
                }
                if(keyParameters[argv[i][j]].type == BOOL) {
                    Value val;
                    val.set_bool(true);
                    keyArgs[argv[i][j]] = val;
                } else {
                    if(argv[i][j-1] == '-' && argv[i][j+1] == '\0') {
                        // arg must be seperate. Ex. `-n hi`
                        isKeyArg = true;
                        argShortName = argv[i][j];
                    } else {
                        throw ParseError("Arg '"+string(1, argv[i][j])+"' should be seperated.");
                    }
                }
                ++j;
            }
        } else {
            // position args
            Value val;
            if(pos >= posParameters.size()) {
                throw ParseError("Too many position args");
            }
            if(posParameters[pos].type == INT) {
                size_t pos;
                int int_v;
                try {
                   int_v = stoi(string(argv[i]), &pos);
                } catch(invalid_argument &e) {
                    throw ParseError("Position arg '" + string(argv[i]) + "' is not an integer.");
                }
                if(argv[i][pos] != '\0') {
                    throw ParseError("Position arg '" + string(argv[i]) + "' is not an integer.");
                }
                val.set_int(int_v);
            } else if(posParameters[pos].type == STR) {
                val.set_str(string(argv[i]));
            }
            positionArgs.push_back(val);
            pos++;
        }
        if(isKeyArg) {
            string name = (argLongName.size()) ? argLongName : string(1, argShortName);
            if(i + 1 < argc) {
                Value val;
                if(keyParameters[argShortName].type == STR) {
                    val.set_str(string(argv[i+1]));
                } else if(keyParameters[argShortName].type == INT) {
                    size_t pos;
                    int int_v;
                    try {
                       int_v = stoi(string(argv[i+1]), &pos);
                    } catch(invalid_argument &e) {
                        throw ParseError("Arg following '" + name + "' is not an integer.");
                    }
                    if(argv[i+1][pos] != '\0') {
                        throw ParseError("Arg following '" + name + "' is not an integer.");
                    }
                    val.set_int(int_v);
                }
                keyArgs[argShortName] = val;
                i++;
            } else {
                throw ParseError(name + " need a value.");
            }
        }
    }
}

    
void ArgParser::set_opt(std::string name, char shortName, ArgType type,
                        std::string description) {
    if(keyParameters.find(shortName) != keyParameters.end()) {
        throw logic_error("Short name '" + string(1, shortName) + "' is duplicate.");
    }
    if(translateTable.find(name) != translateTable.end()) {
        throw logic_error("Name '" + name + "' is duplicate.");
    }
    keyParameters[shortName] = KeyParam{.name=name, .type=type,
                                        .description=description};
    if(name.size() != 0) {
        translateTable[name] = shortName;
    }
    if(type == BOOL) {
        Value val;
        val.set_bool(false);
        keyArgs[shortName] = val;
    }
}


void ArgParser::set_opt(ArgType type, std::string description) {
    if(type == BOOL) {
        throw logic_error("Position arg shouldn't be BOOL type.");
    }
    posParameters.push_back(ArgParser::PosParam{.type=type,
                                                .description=description});
}


Value ArgParser::operator[](char shortName) {
    if(keyArgs.find(shortName) != keyArgs.end()) {
        return keyArgs[shortName];
    } else {
        throw logic_error("key '"+string(1, shortName)+"' not found.");
    }
}


Value ArgParser::operator[](int position) {
    if(position <= positionArgs.size() && position >= 1) {
        return positionArgs[position-1];
    } else {
        throw logic_error("Position out of bound.");
    }
}
