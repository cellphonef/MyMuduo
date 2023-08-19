#include "Timestamp.h"
#include <sys/time.h>
#include <stdio.h>

Timestamp::Timestamp(int64_t microSecondsSinceEpoch)
    : microSecondsSinceEpoch_(microSecondsSinceEpoch) {}

Timestamp Timestamp::now() {
    timeval tv;
    gettimeofday(&tv, nullptr);
    int64_t microSecondsSinceEpoch = tv.tv_sec * kMicroSecondsPerSecond + tv.tv_usec;
    return Timestamp(microSecondsSinceEpoch);
}

std::string Timestamp::toString() const {
    char buf[64] = {0};
    time_t seconds = static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
    tm tm_time;
    gmtime_r(&seconds, &tm_time);
    snprintf(buf, 64, "%4d/%02d/%02d %02d:%02d:%02d",
             tm_time.tm_year + 1900,
             tm_time.tm_mon + 1,
             tm_time.tm_mday,
             tm_time.tm_hour,
             tm_time.tm_min,
             tm_time.tm_sec);
    return buf;
}