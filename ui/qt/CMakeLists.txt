# Qt UI CMakeLists.txt

# 查找 Qt6 组件
find_package(Qt6 REQUIRED COMPONENTS Core Quick Widgets)

# 创建 Qt 可执行文件
add_executable(mini-bot-qt main_qt.cpp ${SHARED_SOURCES})

# 设置包含目录
target_include_directories(mini-bot-qt PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# 链接 Qt 库
target_link_libraries(mini-bot-qt PRIVATE 
    Qt6::Core
    Qt6::Quick
    Qt6::Widgets
    cpr::cpr 
    nlohmann_json::nlohmann_json
)

# 设置 Qt 资源文件
qt_add_resources(mini-bot-qt "qml_resources" 
    PREFIX "/"
    FILES
        qml/ChatWindow.qml
        qml/components/Bubble.qml
        qml/components/TokenMeter.aml
)

# 设置 Qt 自动处理 MOC
set_target_properties(mini-bot-qt PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

