#ifndef TOKEN_TYPE_H
#define TOKEN_TYPE_H

namespace chirsz {

enum TokenType {
    LPt = '(' , RPt = ')',
    Num = 128,
    Symbol,
    // + -
    Add, Sub,
    // * /    %    //
    Mul, Div, Mod, ExactDiv,
    // ^
    Pow,

    None,
};

union TokenValue {
    double num;
    char symbol;
};

}
#endif