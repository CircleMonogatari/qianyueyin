# 项目信息
project(myserver)


# 生成目标
add_executable(server main.cpp Server.hpp Server.cpp)


# 添加库
target_link_libraries(server ws2_32)