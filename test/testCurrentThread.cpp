#include "../CurrentThread.h"

#include <thread>

#include <iostream>


void func() {
    std::cout << CurrentThread::tid() << " " << std::endl;
}

int main() {
    std::thread t1(func);
    std::thread t2(func);

    t1.join();
    t2.join();
    std::cout << "end!" << std::endl;
}