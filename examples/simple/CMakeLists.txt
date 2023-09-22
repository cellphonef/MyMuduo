# 生成simple_echo目标
add_executable(simple_echo echo/echo.cpp echo/main.cpp)

# 与mymuduo静态库进行链接
target_link_libraries(simple_echo mymuduo)