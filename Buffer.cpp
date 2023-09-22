#include "Buffer.h"
#include "SocketOps.h"

Buffer::Buffer(size_t initialSize)
    : buffer_(kCheapPrepend + initialSize),
      readerIndex_(kCheapPrepend),
      writerIndex_(readerIndex_) {}

size_t Buffer::readableBytes() const {
    return writerIndex_ - readerIndex_;
}

size_t Buffer::writableBytes() const {
    return buffer_.size() - writerIndex_;
}

size_t Buffer::prependableBytes() const {
    return readerIndex_;
}

const char* Buffer::peek() const {
    return begin() + readerIndex_;
}

void Buffer::retrieve(size_t len) {
    if (len < readableBytes()) {
        readerIndex_ += len;
    } else {
        retrieveAll();
    }
}

void Buffer::retrieveUntil(const char* end) {
    retrieve(end - peek());
}

void Buffer::retrieveAll() {
    readerIndex_ = kCheapPrepend;
    writerIndex_ = kCheapPrepend;
}

void Buffer::append(const char* data, size_t len) {
    ensureWritableBytes(len);
    std::copy(data, data+len, beginWrite());
    writerIndex_ += len;
}

char* Buffer::beginWrite() {
    return begin() + writerIndex_;
}
const char* Buffer::beginWrite() const {
    return begin() + writerIndex_;

}

void Buffer::ensureWritableBytes(size_t len) {
    if (writableBytes() < len) {
        makeSpace(len);
    }
}

ssize_t Buffer::readFd(int fd, int* savedErrno) {
    char extrabuf[65535];
    struct iovec vec[2];
    const size_t writable = writableBytes();
    vec[0].iov_base = begin()+writerIndex_;
    vec[0].iov_len = writable;
    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof extrabuf;

    const int iovCnt = (writable < sizeof(extrabuf)) ? 2 : 1;

    const ssize_t n = sockets::readv(fd, vec, iovCnt);  // 从fd中读取数据

    if (n < 0) {  // 读错误
        *savedErrno = errno;
    } else if (n <= writable) {  // buffer剩余空间可以存放
        writerIndex_ += n;
    } else {  // buffer剩余空间不够，扩容再添加
        writerIndex_ = buffer_.size();
        append(extrabuf, n - writable);
    }
    return n;

}

ssize_t Buffer::writeFd(int fd, int* savedErrno) {
    ssize_t n = sockets::write(fd, peek(), readableBytes());

    if (n < 0) {
        *savedErrno = errno;
    }
    return n;
}

char* Buffer::begin() {
    return &*buffer_.begin();
}

const char* Buffer::begin() const {
    return &*buffer_.begin();
}

void Buffer::makeSpace(size_t len) {
    if (writableBytes() + prependableBytes() < len + kCheapPrepend) {  // 内部剩余空间不足，重新分配
        buffer_.resize(writerIndex_ + len);
    } else {  // 如果内部剩余空间大于len，则搬移数据而不重新分配空间
        size_t readable = readableBytes();
        std::copy(begin()+readerIndex_,
                  begin()+writerIndex_,
                  begin()+kCheapPrepend);
        
        readerIndex_ = kCheapPrepend;
        writerIndex_ = readerIndex_ + readable;
    }
}




