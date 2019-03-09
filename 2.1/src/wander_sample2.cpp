#include "random_wander.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cctype>
#include <string>
#include <sstream>

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::string;
using std::getline;

static void input_error()
{
    cerr << "读取参数时发生错误" << endl;
    exit(1);
}

static void get_args(int argc, char const *argv[], uint64_t &T, std::vector<uint64_t> &Ns);

int main(int argc, char const *argv[])
{
    uint64_t T = 0;
    std::vector<uint64_t> Ns;
    get_args(argc, argv, T, Ns);

    if (Ns.empty() || T == 0) input_error();

    cout << "网格大小：";
    for (size_t N : Ns)
        cout << N << " ";
    cout << endl;
    cout << "执行次数：" << T << endl;

    srand(time(0));

    printf("N\t逃出频率\n");
    for (auto N : Ns)
    {
        uint64_t escape = 0;
        for (uint64_t i = 0; i < T; ++i)
            if (random_wander(N) > 0)
                ++escape;

        printf("%llu\t%%%0.3lf\n", N, double(escape) / T * 100.0 );
        fflush(stdout);
    }

    return 0;
}

static void get_args(int argc, char const *argv[], uint64_t &T, std::vector<uint64_t> &Ns)
{
    switch (argc)
    {
    case 1: {
        uint64_t N = 0;
        string line;

        while (line.empty())
        {
            cout << "请输入一组网格数：";
            getline(cin, line);
        }
        std::istringstream parse_line(line);
        while (parse_line >> N)
        {
            if (N == 0) input_error();
            Ns.push_back(N);
        }

        line.clear();
        while (line.empty())
        {
            cout << "请输入执行次数：";
            getline(cin, line);
        }
        parse_line = std::istringstream(line);
        parse_line >> T;
        break;
    }
    case 2:
        input_error();
        break;
    default:
        Ns.reserve(argc - 2);
        for (int i = 1; i < argc - 1; ++i)
        {
            size_t N = atol(argv[i]);
            if (N == 0) input_error();
            Ns.push_back(N);
        }
        T = atol(argv[argc - 1]);
    }
}
