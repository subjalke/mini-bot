#include<nlohmann/json.hpp>
#include "BotService.hpp"
#include "ChatSession.hpp"
#include "OllamaClient.hpp"
#include "ToolRegistry.hpp"
#include <iostream>
#include <string>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <locale>
#ifdef _WIN32
#include <windows.h>
#endif
int main() {
    try {
#ifdef _WIN32
        // 设置控制台编码为 UTF-8
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
        // 设置控制台模式以支持 UTF-8
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode;
        GetConsoleMode(hConsole, &mode);
        SetConsoleMode(hConsole, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
        std::setlocale(LC_ALL, ".UTF-8");
        
        std::cout << "=== Mini-Bot 启动 ===" << std::endl;
        
        // 启动 Ollama 服务
        std::cout << "正在启动 Ollama 服务..." << std::endl;
        int result = std::system("powershell -ExecutionPolicy Bypass -File C:\\code2\\mini-bot\\scripts\\run_ollama.ps1");
        if (result != 0) {
            std::cerr << "警告：Ollama 服务启动可能失败，但程序将继续运行..." << std::endl;
        }

        // 等待一下让服务完全启动
        std::cout << "等待服务启动..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));

        // 初始化核心对象
        std::cout << "初始化 ChatSession..." << std::endl;
        ChatSession session(1000); // 最大1000个token
        
        std::cout << "初始化 OllamaClient..." << std::endl;
        OllamaClient client("http://localhost:11434", "deepseek-coder:6.7b");
        
        std::cout << "初始化 ToolRegistry..." << std::endl;
        ToolRegistry toolRegistry;
        
        // 创建BotService（会自动注册所有工具）
        std::cout << "创建 BotService..." << std::endl;
        BotService bot(session, client, toolRegistry);
        
        std::cout << "欢迎使用 Mini-Bot CLI. 输入内容开始对话，输入 'exit' 退出." << std::endl;
        
        std::string userInput;
        while (true) {
            std::cout << "\n用户: ";
            std::getline(std::cin, userInput);
            
            if (userInput == "exit") {
                break;
            }
            
            if (!userInput.empty()) {
                std::cout << "助手: ";
                try {
                    std::string response = bot.processUserMessage(userInput);
                    if (response.empty()) {
                        std::cout << "[无响应] 模型没有返回任何内容，请检查 Ollama 服务状态" << std::endl;
                    }
                    std::cout << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "处理消息时出错: " << e.what() << std::endl;
                }
            }
        }
        
        std::cout << "再见！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "程序启动错误: " << e.what() << std::endl;
        std::cerr << "请确保:" << std::endl;
        std::cerr << "1. Ollama 已正确安装" << std::endl;
        std::cerr << "2. 模型 deepseek-r1:8b 已下载" << std::endl;
        std::cerr << "3. Ollama 服务正在运行" << std::endl;
        return 1;
    }
    
    return 0;
}