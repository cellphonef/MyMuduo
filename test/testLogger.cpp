#include "../Logger.h"

int main() {
    LOG_DEBUG("threadId = %d", 100);
    LOG_ERROR("threadId = %d", 100);
    LOG_FATAL("threadId = %d", 100);
    LOG_INFO("threadId = %d", 100);
    
}