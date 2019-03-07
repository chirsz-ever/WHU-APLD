#include "random_wander.h"
#include <vector>
#include <cstdlib>
#include <cstdint>

using std::vector;
static vector<bool> grids;
static int directions[4][2] = {{ 0, -1}, { -1, 0}, {1, 0}, {0, 1}};

#define GRID(x,y) grids[(x)*N+(y)]
#define ON_EDGE(x,y) ((x)==0||(y)==0||(x)==(N-1)||(y)==(N-1))
#define ON_IMPASSE(x,y) (GRID((x)+1,(y))&&  \
                        GRID((x),(y)+1)&&   \
                        GRID((x)-1,(y))&&   \
                        GRID((x),(y)-1))

int64_t random_wander(const uint64_t N)
{
    // 重置“已到路口”记录
    grids.clear();
    grids.resize(N * N, false);

    uint64_t x = N / 2, y = N / 2;
    int64_t steps = 0;

    while (true)
    {
        GRID(x, y) = true;
        if (ON_EDGE(x, y))
        {
            return steps;
        }
        if (ON_IMPASSE(x, y))
        {
            return -steps;
        }

        // 随机走一步
        int entries = 0;
        int ds[4];
        // 找出可走的方向记录入ds，再随机选择一个
        for (int i = 0; i < 4; ++i)
            if (!GRID(x + directions[i][0], y + directions[i][1]))
                ds[entries++] = i;
        int r = rand() % entries;
        x += directions[ds[r]][0];
        y += directions[ds[r]][1];
        ++steps;
    }
}
