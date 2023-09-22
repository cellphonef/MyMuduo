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
    ~Buffer() = default;
    
    size_t readableBytes() const;
    size_t writableBytes() const;
    size_t prependableBytes() const;

    const char* peek() const;  // 窥探数据，返回可读数据的起始地址

    // 读数据
    void retrieve(size_t len);  // 读一定长度
    void retrieveUntil(const char* end);  // 读到某个位置
    void retrieveAll();  // 读所有
    std::string retrieveAsString(size_t len);  // 读一定长度并返回string 
    std::string retrieveAllAsString();  // 读所有并返回string

    // 写数据
    void append(const char* data, size_t len);

    // 返回可写的起始地址
    char* beginWrite();
    const char* beginWrite() const;
    
    void ensureWritableBytes(size_t len);  // 确保有足够的剩余空间用于写，如果当前剩余空间不足则扩容

    ssize_t readFd(int fd, int* savedErrno);  // 从fd中读取数据到buffer中
    ssize_t writeFd(int fd, int* savedErrno);  // 将buffer中的数据往fd写

private:
    // 返回缓冲区起始地址
    char* begin();  
    const char* begin() const;

    void makeSpace(size_t len);  // 根据情况进行扩容或数据搬移

    std::vector<char> buffer_;  // 存储数据
    size_t readerIndex_;  // 可读起始位置，使用整型可以避免迭代器失效
    size_t writerIndex_;  // 可写起始位置，使用整型可以避免迭代器失效
};



#endif  // MYMUDUO_BUFFER_H