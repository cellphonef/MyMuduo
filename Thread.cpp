#include "Thread.h"


Thread::Thread(ThreadFunc func) 
    : func_(func),
      started_(false),
      threadPtr_(nullptr) {}


void Thread::start() {
    started_ = true;

    threadPtr_.reset(new std::thread(func_));

}

