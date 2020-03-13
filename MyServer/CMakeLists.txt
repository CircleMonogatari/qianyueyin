# 项目信息
project(myserver)

# 添加库
link_libraries(ws2_32)

# 生成目标
add_executable(server main.cpp Server.hpp Server.cpp)