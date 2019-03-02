#ifndef RANDOM_WANDER_H
#define RANDOM_WANDER_H value

#include <cstddef>

/* 模拟一次在N×N大小的网格中的自回避随机行走。
 * 如果成功走出，返回行走路径长度L；如果走入死胡同，返回一个负数，为行走路径长度的相反数。
 */
long random_wander(size_t N);

#endif
