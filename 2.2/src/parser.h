#ifndef PARSER_H
#define PARSER_H

#include <lexer.h>
#include <expr_tree.h>

namespace chirsz {

class Parser
{
public:
    Parser(Lexer&);

    ExprTree build_tree();

private:
    Lexer& lexer;

    // 读取下一个标记后匹配类型,
    // 如不匹配则抛出异常。
    void match(TokenType);

    // 为每个非终结符编写一个函数
    TNHdl expr();
    TNHdl expr_tail(TNHdl::pointer ltree);
    TNHdl term();
    TNHdl term_tail(TNHdl::pointer ltree);
    TNHdl power();
    TNHdl power_tail(TNHdl::pointer ltree);
    TNHdl factor();
};

}

#endif
