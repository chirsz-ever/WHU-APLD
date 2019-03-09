#include <iostream>
#include <string>
#include <exception>
#include "expr_tree.h"
#include "lexer.h"
#include "parser.h"
#include <algorithm>

using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::getline;
using namespace chirsz;

int main(int argc, char const *argv[]) {
    string line;

    // REPL
    while (true) {
        cout << "--> ";
        getline(cin, line);

        // 空行则重新输入
        if (std::all_of(line.begin(), line.end(), isspace))
            continue;

        // 如果输入 "quit" 则退出
        if (line == "quit")
        {
            cout << "quit." << endl;
            break;
        }

        // 解析输入内容为表达式树
        ExprTree tree;
        try {
            auto lexer = Lexer(line);
            tree = Parser(lexer).build_tree();
        }
        catch (std::exception &e) {
            cout << e.what() << endl;
            continue;
        }

        // 以中缀形式输出表达式树
        print_as_infix(cout, tree) << endl;

        // 以后缀形式输出表达式树
        print_as_suffix(cout, tree) << endl;
    }
    return 0;
}
