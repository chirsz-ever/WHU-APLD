#include <iostream>
#include <string>
#include <exception>
#include "expr_tree.h"
#include "lexer.h"
#include "parser.h"

using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::getline;
using namespace chirsz;

int main(int argc, char const *argv[]) {
    string line;
    while (true) {
        cout << "--> ";
        getline(cin, line);

        if (line == "quit")
        {
            cout << "quit." << endl;
            break;
        }

        ExprTree tree;
        try {
            auto lexer = Lexer(line);
            tree = Parser(lexer).build_tree();
        }
        catch (std::exception &e) {
            cout << e.what() << endl;
            continue;
        }
        print_as_infix(cout, tree) << endl;
        print_as_suffix(cout, tree) << endl;
    }
    return 0;
}
