#include <expr_tree.h>
#include <string>
#include <iostream>

#define UNEXPECT(sym) std::runtime_error( \
    string(__FILE__)+":"+to_string(__LINE__) \
    +":Unexpected symbol '"+to_str(sym)+\
    "' in "+__func__)

using std::string;
using std::to_string;
using std::ostream;
using std::unique_ptr;

namespace chirsz {

ExprTree::ExprTree(): head(nullptr) {}

ExprTree::ExprTree(TreeNode* tn): head(tn) {}

const ExprTree& ExprTree::operator=(ExprTree&& et) {
    this->head.reset(nullptr);
    this->head.swap(et.head);
    return *this;
}

// ExprTree::~ExprTree() {
//     delete_tree_node(this->head);
//     this->head = nullptr;
// }

unique_ptr<TreeNode> new_tree_node(TokenType ttype, TreeNode* left, TreeNode* right) {
    return unique_ptr<TreeNode>(new TreeNode{ttype, {0}, TNHdl(left), TNHdl(right)});
}

// void delete_tree_node(TreeNode *tn) {
//     if (tn) {
//         if (tn->left) delete_tree_node(tn->left);
//         if (tn->right) delete_tree_node(tn->right);
//         delete tn;
//     }
// }


inline bool is_item(TokenType tt) {
    return tt == TokenType::Num || tt == TokenType::Symbol;
}

inline bool is_binop(TokenType tt) {
    return !is_item(tt) && tt != TokenType::LPt && tt != TokenType::RPt;
}

static const char* to_str(TokenType tt);

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
        throw UNEXPECT(tt);
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
        break;
    default:
        throw UNEXPECT(tt);
    }
    return a;
}

static const char* to_str(TokenType tt) {
    static string opcache;
    switch (tt) {
    case TokenType::Pow:        opcache = "^"; break;
    case TokenType::Mul:        opcache = "*"; break;
    case TokenType::Div:        opcache = "/"; break;
    case TokenType::Mod:        opcache = "%"; break;
    case TokenType::ExactDiv:   opcache = "//"; break;
    case TokenType::Add:        opcache = "+"; break;
    case TokenType::Sub:        opcache = "-"; break;
    case TokenType::LPt:        opcache = "("; break;
    case TokenType::RPt:        opcache = ")"; break;
    case TokenType::Num:        opcache = "Num"; break;
    case TokenType::Symbol:     opcache = "Symbol"; break;
    case TokenType::None:       opcache = "None"; break;
    default:
        throw UNEXPECT(tt);
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
                throw UNEXPECT(tn->ttype);
            }
        }
        else if (is_binop(tn->ttype)) {
            int pl = tt_priority(tn->left->ttype);
            int pn = tt_priority(tn->ttype);
            int pr = tt_priority(tn->right->ttype);
            if (pl < pn
                    || (tn->left->ttype == tn->ttype && assctvt(tn->ttype) == Right)) {
                et_print_as_infix(out << '(', tn->left.get()) << ')';
            }
            else {
                et_print_as_infix(out, tn->left.get());
            }
            out << to_str(tn->ttype);
            if (pr < pn
                    || (tn->right->ttype == tn->ttype && assctvt(tn->ttype) == Left)) {
                et_print_as_infix(out << '(', tn->right.get()) << ')';
            }
            else {
                et_print_as_infix(out, tn->right.get());
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
                throw UNEXPECT(tn->ttype);
            }
        }
        else if (is_binop(tn->ttype))
        {
            et_print_as_suffix(out, tn->left.get());
            et_print_as_suffix(out, tn->right.get());
            out << to_str(tn->ttype);
        }
    }
    return out << ' ';
}

std::ostream& print_as_infix(std::ostream &out, const ExprTree &et) {
    return et_print_as_infix(out, et.head.get());
}

std::ostream& print_as_suffix(std::ostream &out, const ExprTree &et) {
    return et_print_as_suffix(out, et.head.get());
}

}
