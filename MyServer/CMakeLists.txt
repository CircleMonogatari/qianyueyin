# 项目信息
project(myserver)
add_definitions(-std=c++11)

# 生成目标
add_executable(server main.cpp Server.hpp Server.cpp)


# 添加库
target_link_libraries(server ws2_32)