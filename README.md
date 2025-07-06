# Mini-Bot

一个基于 Ollama 的简单聊天机器人。

## 功能特性

- 🤖 基于 Ollama 的聊天对话
- 💬 会话历史管理
- 🔄 流式响应显示
- 🚀 简洁的命令行界面

## 项目结构

```
mini-bot/
├── src/
│   ├── main.cpp              # 主程序入口
│   ├── OllamaClient.cpp      # Ollama API 客户端
│   ├── ChatSession.cpp       # 会话管理
│   └── BotService.cpp        # 机器人服务
├── include/
│   ├── OllamaClient.hpp
│   ├── ChatSession.hpp
│   └── BotService.hpp
├── CMakeLists.txt           # CMake 配置
└── README.md
```

## 前置要求

1. **Ollama** - 本地大语言模型服务
   - 下载地址：https://ollama.ai/
   - 安装后需要下载模型：`ollama pull deepseek-r1:8b`

2. **C++ 编译器**
   - Windows: Visual Studio 2019+ 或 MinGW
   - Linux: GCC 7+
   - macOS: Clang 10+

3. **CMake** 3.21+

4. **vcpkg** - C++ 包管理器
   - 安装依赖：`vcpkg install cpr nlohmann-json`

## 构建步骤

1. **克隆项目**
   ```bash
   git clone <repository-url>
   cd mini-bot
   ```

2. **配置项目**
   ```bash
   cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
   ```

3. **构建项目**
   ```bash
   cmake --build build
   ```

4. **运行程序**
   ```bash
   ./build/Debug/mini-bot.exe  # Windows
   ./build/mini-bot            # Linux/macOS
   ```

## 使用方法

1. 启动程序后，会自动启动 Ollama 服务
2. 输入消息开始对话
3. 输入 `exit` 退出程序

## 示例对话

```
用户: 你好
助手: 你好！我是 Mini-Bot，很高兴为您服务。有什么我可以帮助您的吗？

用户: 请介绍一下你自己
助手: 我是一个基于 Ollama 的聊天机器人，使用 deepseek-r1:8b 模型。我可以进行自然语言对话，帮助您解答问题、进行交流等。有什么具体的问题或话题想和我讨论吗？

用户: exit
再见！
```

## 技术栈

- **C++17** - 核心语言
- **Ollama** - 本地大语言模型服务
- **cpr** - HTTP 客户端库
- **nlohmann/json** - JSON 处理库
- **CMake** - 构建系统

## 许可证

MIT License