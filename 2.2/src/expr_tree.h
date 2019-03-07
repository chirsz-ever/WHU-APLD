#ifndef EXPR_TREE_H
#define EXPR_TREE_H

#include <token.h>
#include <iosfwd>
#include <string>

namespace chirsz {

struct TreeNode
{
    TokenType ttype;
    TokenValue tval;
    TreeNode* left;
    TreeNode* right;
};

TreeNode* new_tree_node(TokenType ttype, TreeNode* left, TreeNode* right);
void delete_tree_node(TreeNode*);

class ExprTree
{
public:
    ExprTree();
    ExprTree(TreeNode*);
    ExprTree(ExprTree&&)=default;
    ~ExprTree();

    const ExprTree& operator=(ExprTree&&);

private:
    TreeNode* head;
// friend:
    friend std::ostream& print_as_infix(std::ostream&, const ExprTree&);
    friend std::ostream& print_as_suffix(std::ostream&, const ExprTree&);
};

}
#endif