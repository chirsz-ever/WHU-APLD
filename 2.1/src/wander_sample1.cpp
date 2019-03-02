#include "random_wander.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using std::cout;
using std::cin;
using std::cerr;
using std::endl;

int main(int argc, char const *argv[])
{
    size_t N, T;
    switch (argc)
    {
    case 3:
        N = atol(argv[1]);
        T = atol(argv[2]);
        break;
    case 1:
        N = 50;
        cout << "������ִ�д�����";
        cin >> T;
        break;
    default:
        N = T = 0;
    }
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

    size_t escape = 0, impasse = 0;
    double success_path_sum = 0.0, fail_path_sum = 0.0;
    for (int i = 0; i < T; ++i)
    {
        const long steps = random_wander(N);
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
    printf("�ɹ��ӳ�%ld�Σ�Ƶ��Ϊ%.4lf��ƽ��·������Ϊ%.2lf\n",
           escape, double(escape) / T, success_path_sum / escape);
    printf("��������ͬ%ld�Σ�Ƶ��Ϊ%.4lf��ƽ��·������Ϊ%.2lf\n",
           impasse, double(impasse) / T, fail_path_sum / impasse);

    return 0;
}
