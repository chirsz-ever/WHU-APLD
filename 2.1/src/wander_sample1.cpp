#include "random_wander.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include <algorithm>

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::string;
using std::getline;

static void get_args(int argc, char const *argv[], uint64_t &N, uint64_t &T);

int main(int argc, char const *argv[])
{
    uint64_t N, T;

    get_args(argc, argv, N, T);
    if (N == 0 || T == 0)
    {
        cerr << "��ȡ����ʱ��������" << endl;
        return 1;
    }

    cout << "N=" << N << endl;
    cout << "T=" << T << endl;
    cout << "ִ���С���  " ;
    cout.flush();

    srand(time(0));

    uint64_t escape = 0, impasse = 0;
    double success_path_sum = 0.0, fail_path_sum = 0.0;
    for (uint64_t i = 0; i < T; ++i)
    {
        const auto steps = random_wander(N);
        if (steps > 0)
        {
            ++escape;
            success_path_sum += steps;
        }
        else
        {
            ++impasse;
            fail_path_sum += -steps;
        }
    }
    cout << "ִ�����" << endl;
    printf("�ɹ��ӳ�%llu�Σ�Ƶ��Ϊ%.4lf��ƽ��·������Ϊ%.2lf\n",
           escape, double(escape) / T, success_path_sum / escape);
    printf("��������ͬ%llu�Σ�Ƶ��Ϊ%.4lf��ƽ��·������Ϊ%.2lf\n",
           impasse, double(impasse) / T, fail_path_sum / impasse);

    return 0;
}

static void get_args(int argc, char const *argv[], uint64_t &N, uint64_t &T)
{
    switch (argc)
    {
    case 3:
        N = atol(argv[1]);
        T = atol(argv[2]);
        break;
    case 1:
    {
        N = 50;
        string line;
        while (std::all_of(line.begin(), line.end(), isspace))
        {
            cout << "������ִ�д�����";
            getline(cin, line);
        }
        std::istringstream parse_line(line);
        parse_line >> T;
        break;
    }
    default:
        N = T = 0;
    }
}
