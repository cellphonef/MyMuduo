#ifndef MYMUDUO_BUFFER_H
#define MYMUDUO_BUFFER_H

#include <vector>
#include <string>

// +-------------------+------------------+------------------+
// | prependable bytes |  readable bytes  |  writable bytes  |
// |                   |     (CONTENT)    |                  |
// +-------------------+------------------+------------------+
// |                   |                  |                  |
// 0      <=      readerIndex   <=   writerIndex    <=     size
//
// prependable = readerIndex
// readable = writerIndex - readerIndex
// writable = size() - writerIndex


class Buffer {
public:
    static const size_t kCheapPrepend = 8;
    static const size_t kInitialSize = 1024;

    explicit Buffer(size_t initialSize = kInitialSize);
    ~Buffer();

    ssize_t readFd(int fd, int* savedErrno);  // 从fd中读取数据到buffer中
    
    size_t readableBytes() const;
    size_t writableBytes() const;
    size_t prependableBytes() const;

    const char* peek() const;  // 返回可读数据的起始地址

    // 取走数据（本质上就是移动索引）
    void retrieve(size_t len);
    void retrieveUntil(const char* end);
    void retrieveAll();
    std::string retrieveAsString(size_t len);
    std::string retrieveAllAsString();

    // 添加数据（将数据拷贝到buffer中并移动索引）
    void append();
    void append(const char* data, size_t len);
    void append(const void* data, size_t len);

    void ensureWritableBytes(size_t len);  // 确保有足够的剩余空间用于写，如果当前剩余空间不足则扩容

    // 返回可写的起始地址
    char* beginWrite();
    const char* beginWrite() const;

    
    void hasWritten(size_t len);

private:
    char* begin();
    const char* begin() const;
    void makeSpace(size_t len);  // 根据情况进行

    std::vector<char> buffer_;  // 存储数据
    size_t readerIndex_;  // 可读起始位置，使用整型可以避免迭代器失效
    size_t writerIndex_;  // 可写起始位置，使用整型可以避免迭代器失效
};



#endif  // MYMUDUO_BUFFER_H