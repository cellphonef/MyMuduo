#ifndef MYMUDUO_TIMESTAMP_H
#define MYMUDUO_TIMESTAMP_H

#include <string>

// 时间戳类
// 
// 实现和设计要点：
// (1) 存储时间
// 计算机中表示时间采用一种简单有效的方式，即以一个时间点为原点，
// 然后用一个整数来表示具体时间与原点的时间的差值。差值的单位（如微秒）和整数的位数决定了能够表示的时间范围。
// (2) 获取当前时间
// (3) 时间的计算机表示转易读字符串表示，便于输出
class Timestamp {
public:
    static const int kMicroSecondsPerSecond = 1000 * 1000;  // 每秒的微秒数

    explicit Timestamp(int64_t microSecondsSinceEpoch = 0);
    ~Timestamp() = default;
    static Timestamp now();  // 返回当前时间
    std::string toString() const;  // 将计算机时间表示为人类易读的时间
private:
    int64_t microSecondsSinceEpoch_;  // 从1970年0时0分到现在的毫秒数

};

#endif  // MYMUDUO_TIMESTAMP_H