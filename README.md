# MyMuduo

使用C++11重构Muduo库。

## 目前待解答的疑问

### std::bind复制

std::bind会复制两次。

### 智能指针的使用

- TcpConnection中的socket_成员和channel_成员为什么使用unique_ptr不直接保存对象？
- TcpServer中的threadPool_成员为什么使用shared_ptr？
- 
