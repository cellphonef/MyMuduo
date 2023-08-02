#ifndef MYMUDUO_TIMESTAMP_H
#define MYMUDUO_TIMESTAMP_H

#include <string>

//  时间戳类
// 
//  实现和设计要点：
//    ①存储时间
//      （1）计算机中表示时间采用一种简单有效的方式，即以一个时间点为原点，
//          用一个整数来表示当前时间与原点的时间的差值。差值的单位和整数的位数决定了能够表示的时间范围。
//    ②可以利用该类获取当前时间
//    ③时间转字符串
//      （1）为了便于阅读，需要将整数转为字符串。
// 
class Timestamp {
public:

    static Timestamp now();
    std::string toString(); 
private:

};




#endif  // MYMUDUO_TIMESTAMP_H