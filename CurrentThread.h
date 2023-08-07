#ifndef MYMUDUO_CURRENTTHREAD_H
#define MYMUDUO_CURRENTTHREAD_H

#include <unistd.h>
#include <sys/syscall.h>

namespace CurrentThread {
    extern __thread int t_cachedTid;  // 缓存tid，避免重复调用系统调用

    void cacheTid();

    // 避免违反ORD原则，将函数定义为inline
    inline int tid() {
        if (__builtin_expect(t_cachedTid == 0, 0)) {  // __builtin_expect作用：https://www.jianshu.com/p/2684613a300f
            cacheTid();
        }
        return t_cachedTid;
    }


}  // namespace CurrentThread

#endif  // MYMUDUO_CURRENTTHREAD_H 