#ifndef EXPR_TREE_H
#define EXPR_TREE_H

#include <token.h>
#include <iosfwd>
#include <string>
#include <memory>

namespace chirsz {

struct TreeNode
{
    TokenType ttype;
    TokenValue tval;
    std::unique_ptr<TreeNode> left;
    std::unique_ptr<TreeNode> right;
};

using TNHdl = std::unique_ptr<TreeNode>;

TNHdl new_tree_node(TokenType ttype, TNHdl::pointer left, TNHdl::pointer right);

class ExprTree
{
public:
    ExprTree();
    ExprTree(TreeNode*); // 获得所有权
    ExprTree(ExprTree&&) = default;
    ~ExprTree() = default;

    const ExprTree& operator=(ExprTree&&);

private:
    TNHdl head;
// friend:
    // 以中缀形式输出表达式树
    friend std::ostream& print_as_infix(std::ostream&, const ExprTree&);

    // 以后缀形式输出表达式树
    friend std::ostream& print_as_suffix(std::ostream&, const ExprTree&);
};

}
#endif
