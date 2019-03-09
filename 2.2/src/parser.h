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
    TreeNode* expr();
    TreeNode* expr_tail(TreeNode* ltree);
    TreeNode* term();
    TreeNode* term_tail(TreeNode* ltree);
    TreeNode* power();
    TreeNode* power_tail(TreeNode* ltree);
    TreeNode* factor();
};

}

#endif
