#ifndef MUMUDUO_CALLBACK_H
#define MYMUDUO_CALLBACK_H

#include "Timestamp.h"

#include <functional>
#include <memory> 

class TcpConnection;
class Buffer;

using TcpConnectionPtr = std::shared_ptr<TcpConnection>;

using ConnectionCallback = std::function<void(const TcpConnectionPtr&)>;
using CloseCallback = std::function<void(const TcpConnectionPtr&)>;
using WriteCompleteCallback = std::function<void(const TcpConnectionPtr&)>;

using MessageCallback = std::function<void(const TcpConnectionPtr&,
                                      Buffer*,
                                      Timestamp)>;

#endif  // MYMUDUO_CALLBACK_H