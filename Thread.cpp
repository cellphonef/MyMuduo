#include "Thread.h"


Thread::Thread(ThreadFunc func) 
    : func_(func),
      started_(false),
      thread_(nullptr) {}


void Thread::start() {
    started_ = true;

    thread_ = new std::thread(func_);

}

