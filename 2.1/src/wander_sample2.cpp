#include "random_wander.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

using std::cout;
using std::cin;
using std::cerr;
using std::endl;

static void input_error()
{
    cerr << "读取参数时发生错误" << endl;
    exit(1);
}

int main(int argc, char const *argv[])
{
    uint64_t T;
    std::vector<uint64_t> Ns;
    switch (argc)
    {
    case 1:
        uint64_t N;
        cout << "请输入一组网格数：";
        while (cin >> N)
        {
            if (N == 0) input_error();
            Ns.push_back(N);
            if (cin.peek()=='\n')
                break;
        }
        cout << "请输入执行次数：";
        cin >> T;
        if (cin.fail()) input_error();
        break;
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

    if (Ns.empty() || T == 0) input_error();

    cout << "网格大小：";
    for (size_t N : Ns)
        cout << N << " ";
    cout << endl;
    cout << "T=" << T << endl;

    srand(time(0));

    printf("N\t频率\n");
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
