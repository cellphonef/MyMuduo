cmake_minimum_required(VERSION 3.10.2)

project(MyMuduo)

# 设置编译选项
set(CXX_FLAGS
  -g
  -Wall
  -Werror
  
  -std=c++11

)



# MyMuduo源文件（字典序）
set(SRCS
  Acceptor.cpp
  Buffer.cpp
  Channel.cpp
  CurrentThread.cpp
  EpollPoller.cpp
  EventLoop.cpp
  EventLoopThread.cpp
  EventLoopThreadPool.cpp
  InetAddress.cpp
  logger/Logging.cpp
  logger/LogStream.cpp
  Poller.cpp
  Socket.cpp
  SocketOps.cpp
  TcpConnection.cpp
  TcpServer.cpp
  Thread.cpp
  Timestamp.cpp
)

# 生成静态库mymuduo
add_library(mymuduo ${SRCS})

# 将静态库mymuduo安装到lib
install(TARGETS mymuduo DESTINATION lib)

# MyMuduo头文件，只包括外部可见的（字典序）
set(HEADERS
  Buffer.h
  Callback.h
  Channel.h
  EventLoop.h
  EventLoopThread.h
  EventLoopThreadPool.h
  InetAddress.h
  TcpConnection.h
  TcpServer.h
)

# 将头文件安装到include下
install(FILES ${HEADERS} DESTINATION include/mymuduo/)

# 是否构建examples
if(MYMUDUO_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()







