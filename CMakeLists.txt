cmake_minimum_required(VERSION 3.21)
project(mini-bot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置 UTF-8 编码
if(MSVC)
    add_compile_options(/utf-8)
else()
    add_compile_options(-finput-charset=UTF-8)
endif()

# 只保留必要的源文件
set(SOURCES
    src/main.cpp
    src/OllamaClient.cpp
    src/ChatSession.cpp
    src/BotService.cpp
)

# 头文件
set(HEADERS
    include/OllamaClient.hpp
    include/ChatSession.hpp
    include/BotService.hpp
    include/ToolRegistry.hpp
    include/tools/Calculator.hpp
    include/tools/Weather.hpp
)

# 创建可执行文件
add_executable(mini-bot ${SOURCES})

# 设置包含目录
target_include_directories(mini-bot PRIVATE include)

# 查找依赖包
# 在此处设置 cpr_DIR 路径，例如：
set(cpr_DIR "C:/code2/mini-bot/vcpkg/installed/x64-windows/share/cpr")

find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# 链接库
target_link_libraries(mini-bot PRIVATE cpr::cpr nlohmann_json::nlohmann_json)
