cmake_minimum_required(VERSION 3.21)
project(mini-bot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/main_cli.cpp
    src/BotService.cpp
    src/ChatSession.cpp
    src/OllamaClient.cpp
    src/ToolRegistry.cpp
    src/ToolRegistration.cpp
    src/tool/tool_calc.cpp
    src/tool/tool_wearther.cpp
)
add_executable(mini-bot ${SOURCES})
target_include_directories(mini-bot PRIVATE include)
find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(mini-bot PRIVATE cpr::cpr nlohmann_json::nlohmann_json)
