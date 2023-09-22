#ifndef MYMUDUO_LOGGER_FIXEDBUFFER_H
#define MYMUDUO_LOGGER_FIXEDBUFFER_H

#include <functional>

const int kSmallBuffer = 4000;
const int kLargeBuffer = 4000 * 1000;

// 固定大小缓冲区
// 
// 作为双缓存异步模块的前后端使用缓冲区
template <int SIZE>
class FixedBuffer {
public:
    FixedBuffer();
    ~FixedBuffer();

    void append(const char* buf, size_t len);


private:
    char data_[SIZE];
    char* cur_;

};

template <int SIZE>
FixedBuffer<SIZE>::FixedBuffer()
    : cur_(data_) {}

template <int SIZE>
FixedBuffer<SIZE>::~FixedBuffer() {

}

template <int SIZE>
void FixedBuffer<SIZE>::append(const char* buf, size_t len) {
    std::copy(buf, buf+len, cur_);
    cur_ += len;
}










#endif  // MYMUDUO_LOGGER_FIXEDBUFFER_H