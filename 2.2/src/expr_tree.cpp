#include <expr_tree.h>
#include <string>
#include <iostream>

using std::string;
using std::ostream;

namespace chirsz {

ExprTree::ExprTree(): head(nullptr) {}

ExprTree::ExprTree(TreeNode* tn): head(tn) {}

const ExprTree& ExprTree::operator=(ExprTree&& et) {
    this->head = et.head;
    et.head = nullptr;
    return *this;
}

ExprTree::~ExprTree() {
    delete_tree_node(this->head);
    this->head = nullptr;
}

TreeNode* new_tree_node(TokenType ttype, TreeNode* left, TreeNode* right) {
    return new TreeNode{ttype, {0}, left, right};
}

void delete_tree_node(TreeNode *tn) {
    if (tn) {
        if (tn->left) delete_tree_node(tn->left);
        if (tn->right) delete_tree_node(tn->right);
        delete tn;
    }
}


inline bool is_item(TokenType tt) {
    return tt == TokenType::Num || tt == TokenType::Symbol;
}

inline bool is_binop(TokenType tt) {
    return !is_item(tt) && tt != TokenType::LPt && tt != TokenType::RPt;
}

static int tt_priority(TokenType tt) {
    int p;
    switch (tt) {
    case TokenType::Num:
    case TokenType::Symbol:
        p = 0x3f3f3f3f;
        break;
    case TokenType::Pow:
        p = 300;
        break;
    case TokenType::Mul:
    case TokenType::Div:
    case TokenType::Mod:
    case TokenType::ExactDiv:
        p = 200;
        break;
    case TokenType::Add:
    case TokenType::Sub:
        p = 100;
        break;
    default:
        throw std::runtime_error("unreachable!");
    }
    return p;
}

enum Associativity
{
    Left, Right
};

Associativity assctvt(TokenType tt) {
    Associativity a;
    switch (tt) {
    case TokenType::Pow:
        a = Right;
        break;
    case TokenType::Mul:
    case TokenType::Div:
    case TokenType::Mod:
    case TokenType::ExactDiv:
    case TokenType::Add:
    case TokenType::Sub:
        a = Left;
    default:
        throw std::runtime_error("unreachable!");
    }
    return a;
}

static const char* to_str(TokenType tt) {
    string opcache;
    switch (tt) {
    case TokenType::Pow: opcache = "^"; break;
    case TokenType::Mul: opcache = "*"; break;
    case TokenType::Div: opcache = "/"; break;
    case TokenType::Mod: opcache = "%"; break;
    case TokenType::ExactDiv: opcache = "//"; break;
    case TokenType::Add: opcache = "+"; break;
    case TokenType::Sub: opcache = "-"; break;
    default:
        throw std::runtime_error("unreachable!");
    }
    return opcache.c_str();
}

static ostream& et_print_as_infix(ostream &out, const TreeNode* tn) {
    if (tn) {
        if (is_item(tn->ttype)) {
            switch (tn->ttype) {
            case TokenType::Num:
                out << tn->tval.num;
                break;
            case TokenType::Symbol:
                out << tn->tval.symbol;
                break;
            default:
                throw std::runtime_error("unreachable!");
            }
        }
        else if (is_binop(tn->ttype)) {
            int pl = tt_priority(tn->left->ttype);
            int pn = tt_priority(tn->ttype);
            int pr = tt_priority(tn->right->ttype);
            if (pl < pn
                    || (tn->left->ttype == tn->ttype && assctvt(tn->ttype) == Right)) {
                et_print_as_infix(out << '(', tn->left) << ')';
            }
            else {
                et_print_as_infix(out, tn->left);
            }
            out << to_str(tn->ttype);
            if (pr < pn
                    || (tn->right->ttype == tn->ttype && assctvt(tn->ttype) == Left)) {
                et_print_as_infix(out << '(', tn->right) << ')';
            }
            else {
                et_print_as_infix(out, tn->right);
            }
        }
    }
    return out;
}

static ostream& et_print_as_suffix(ostream &out, const TreeNode* tn) {
    if (tn) {
        if (is_item(tn->ttype)) {
            switch (tn->ttype) {
            case TokenType::Num:
                out << tn->tval.num;
                break;
            case TokenType::Symbol:
                out << tn->tval.symbol;
                break;
            default:
                throw std::runtime_error("unreachable!");
            }
        }
        else if (is_binop(tn->ttype))
        {
            et_print_as_suffix(out, tn->left);
            et_print_as_suffix(out, tn->right);
            out << to_str(tn->ttype);
        }
    }
    return out << ' ';
}

std::ostream& print_as_infix(std::ostream &out, const ExprTree &et) {
    return et_print_as_infix(out, et.head);
}

std::ostream& print_as_suffix(std::ostream &out, const ExprTree &et) {
    return et_print_as_suffix(out, et.head);
}

}