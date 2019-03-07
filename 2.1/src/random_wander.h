#ifndef RANDOM_WANDER_H
#define RANDOM_WANDER_H 

#include <cstdint>

/* 模拟一次在N×N大小的网格中的自回避随机行走。
 * 如果成功走出，返回行走路径长度L；如果走入死胡同，返回一个负数，为行走路径长度的相反数。
 */
int64_t random_wander(uint64_t N);

#endif
