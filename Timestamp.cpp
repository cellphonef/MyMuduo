#include "Timestamp.h"
#include <sys/time.h>


Timestamp::Timestamp()
    : microSecondsSinceEpoch_(0) {}

Timestamp::Timestamp(int64_t microSecondsSinceEpoch)
    : microSecondsSinceEpoch_(microSecondsSinceEpoch) {}

Timestamp Timestamp::now() {
    timeval tv;
    gettimeofday(&tv, nullptr);
    int microSecondsSinceEpoch = tv.tv_sec * 1000 + tv.tv_usec;
    return Timestamp(microSecondsSinceEpoch);
}