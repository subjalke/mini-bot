cmake_minimum_required(VERSION 3.21)
project(mini-bot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/utf-8)
else()
    add_compile_options(-finput-charset=UTF-8)
endif()

set(SHARED_SOURCES
    src/BotService.cpp
    src/ChatSession.cpp
    src/OllamaClient.cpp
    src/ToolRegistry.cpp
    src/ToolRegistration.cpp
    src/tool/tool_calc.cpp
    src/tool/tool_wearther.cpp
)

add_executable(mini-bot src/main_cli.cpp ${SHARED_SOURCES})
add_executable(mini-bot-sync src/main_cli_sync.cpp ${SHARED_SOURCES})

target_include_directories(mini-bot PRIVATE include)
target_include_directories(mini-bot-sync PRIVATE include)


set(cpr_DIR "C:/code2/mini-bot/vcpkg/installed/x64-windows/share/cpr")
find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(mini-bot PRIVATE cpr::cpr nlohmann_json::nlohmann_json)
target_link_libraries(mini-bot-sync PRIVATE cpr::cpr nlohmann_json::nlohmann_json)
