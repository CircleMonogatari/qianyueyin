# 项目信息
project(myserver)


# 生成目标
add_executable(server main.cpp Server.hpp Server.cpp Chat.cpp Chat.h)
add_definitions(-std=c++11)
