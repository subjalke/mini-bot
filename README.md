# mini-bot 项目说明

## 项目结构

mini-bot/
├─ CMakeLists.txt          # 根 CMake，统一编译规则
├─ vcpkg.json              # vcpkg manifest：声明依赖（cpr / nlohmann-json / qtbase …）
├─ README.md               # 项目说明 & 运行指南
│
├─ config/                 # 可运行时热改的配置
│  └─ settings.json        # 端口、模型名、裁剪阈值、UI 主题等
│
├─ include/                # 头文件，对外接口
│  ├─ ChatSession.hpp      # 保存 / 裁剪 messages，统计 token
│  ├─ OllamaClient.hpp     # 封装 cpr 调用，支持流式回调
│  ├─ BotService.hpp       # 整合 ChatSession + OllamaClient + Tool 调度
│  ├─ ToolRegistry.hpp     # 注册本地工具（函数调用）
│  └─ util.hpp             # 通用助手：token 估算、字符串工具等
│
├─ src/                    # 对应实现文件
│  ├─ main_cli.cpp         # 命令行入口（最小 Demo 就靠它）
│  ├─ ChatSession.cpp
│  ├─ OllamaClient.cpp
│  ├─ BotService.cpp
│  ├─ ToolRegistry.cpp
│  └─ tools/               # 每个 tool 一个 cpp，易于增删
│     ├─ tool_weather.cpp
│     └─ tool_calc.cpp
│
├─ ui/                     # 可视化层（可以二选一，也可都保留）
│  ├─ qt/                  # 纯本地桌面
│  │  ├─ CMakeLists.txt    # 添加 QtQuick、QtWebSockets
│  │  ├─ main_qt.cpp       # QApplication + QQmlApplicationEngine
│  │  ├─ qml/              # QML 资源
│  │  │  ├─ ChatWindow.qml
│  │  │  └─ components/
│  │  │     ├─ Bubble.qml
│  │  │     └─ TokenMeter.qml
│  │  └─ res/              # 图标、字体
│  │
│  
│
├─ tests/                  # 单元测试（Catch2 / GoogleTest）
│  ├─ test_session.cpp
│  └─ test_ollama_mock.cpp
│
└─ scripts/                # 辅助脚本
   ├─ run_ollama.ps1       # 一键启动 serve + pull 模型（Windows）
   ├─ run_ollama.sh        # Linux/macOS 同等脚本
   └─ format_all.sh        # clang-format 自动排版