#ifndef ARG_PARSER_H
#define ARG_PARSER_H
#include <string>
#include <stdexcept>
#include <set>
#include <map>
#include <vector>

enum ArgType {INT, BOOL, STR};


class ParseError : public std::logic_error {
public:
    ParseError(std::string what) : std::logic_error(what) {}
};


class Value {
public:
    Value()=default;
    inline int get_int() {
        return (tag == INT) ? int_v :
                              throw std::logic_error("Value is not a int type.");
    }
    inline void set_int(int val) {
        int_v = val;
        tag = INT;
    }
    inline bool get_bool() {
        return (tag == BOOL) ? bool_v :
                               throw std::logic_error("Value is not a bool type.");
    }
    inline void set_bool(bool val) {
        bool_v = val;
        tag = BOOL;
    }
    inline std::string get_str() {
        return (tag == STR) ? str_v :
                              throw std::logic_error("Value is not a std::string type.");
    }
    inline void set_str(const std::string &val) {
        str_v = val;
        tag = STR;
    }
private:
    ArgType tag;
    int int_v;
    bool bool_v;
    std::string str_v;
};


class ArgParser {
public:
    ArgParser()=default;
    void parse(int argc, char **argv);
    
    void set_opt(std::string name, char shortName, ArgType type,
        std::string description="");
    /*
    * type: int, bool, string
    * Trailing arg(Arg type->int/string), not Trailing arg -> bool
    */

    void set_opt(ArgType type, std::string description="");

    Value operator[](char shortName);
    Value operator[](int position);
private:
    struct PosParam {
        ArgType type;
        std::string description;
    };
    struct KeyParam {
        std::string name;
        ArgType type;
        std::string description;
    };
    std::vector<PosParam> posParameters;
    std::map<char, KeyParam> keyParameters;
    std::map<std::string, char> translateTable;

    std::vector<Value> positionArgs;
    std::map<char, Value> keyArgs;
};

#endif
