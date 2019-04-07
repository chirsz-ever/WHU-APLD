#include <parser.h>
#include <stdexcept>
#include <string>

using std::to_string;
using std::string;
using std::unique_ptr;

#define ERROR (std::runtime_error(string(__FILE__)+":"+to_string(__LINE__)+":"\
    + " unexpect token near position " \
    + to_string(lexer.position()+1)))

namespace chirsz {
Parser::Parser(Lexer& _l): lexer(_l) {}


void Parser::match(TokenType tt) {
    if (tt != lexer.next())
    {
        throw ERROR;
    }
}

TNHdl Parser::expr() {
    auto ltree = term();
    return expr_tail(ltree.release());
}

TNHdl Parser::expr_tail(TNHdl::pointer ltree) {
    TokenType tt = lexer.peek();
    switch (tt) {
    case TokenType::Add:
    case TokenType::Sub:
        lexer.consume();
        return expr_tail(new_tree_node(tt, ltree, term().release()).release());
    default:
        return TNHdl(ltree);
    }
}

TNHdl Parser::term() {
    auto ltree = power();
    return term_tail(ltree.release());
}

TNHdl Parser::term_tail(TNHdl::pointer ltree) {
    TokenType tt = lexer.peek();
    switch (tt) {
    case TokenType::Mul:
    case TokenType::Div:
    case TokenType::Mod:
    case TokenType::ExactDiv:
        lexer.consume();
        return term_tail(new_tree_node(tt, ltree, power().release()).release());
    default:
        return TNHdl(ltree);
    }
}

TNHdl Parser::power() {
    auto ltree = factor();
    return power_tail(ltree.release());
}

TNHdl Parser::power_tail(TNHdl::pointer ltree) {
    TokenType tt = lexer.peek();
    switch (tt) {
    case TokenType::Pow:
        lexer.consume();
        return new_tree_node(tt, ltree, power_tail(factor().release()).release()); // 注意乘方运算是右结合的
    default:
        return TNHdl(ltree);
    }
}

TNHdl Parser::factor() {
    auto tt = lexer.peek();
    switch (tt) {
    case TokenType::LPt: {
        lexer.consume();
        auto tnode = expr();
        match(TokenType::RPt);
        return tnode;
    }
    case TokenType::Num:
    case TokenType::Symbol: {
        lexer.consume();
        auto tnode = new_tree_node(tt, nullptr, nullptr);
        tnode->tval = lexer.last_value();
        return tnode;
    }
    default:
        throw ERROR;
    }
}


ExprTree Parser::build_tree() {
    auto tree = ExprTree(expr().release());

    // 解析完表达式而没有到达结尾，是输入字符串异常
    if (lexer.peek() != TokenType::None)
        throw ERROR;
    return tree;
}

}
