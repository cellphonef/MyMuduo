#ifndef MYMUDUO_CHANNEL_H
#define MYMUDUO_CHANNEL_H

#include <functional>

class Channel {
public:
    using readCallback = std::function<void()> ;
    using closeCallback = std::function<void()>;
    Channel();
    ~Channel();


    void handleEvent();

    void enableReading();
    void enableWriting();

    


private:
    int fd_;
    int events_;
    int revents_;
    readCallback readCb_;
    closeCallback closeCb_;

};




#endif  // MYMUDUO_CHANNEL_H