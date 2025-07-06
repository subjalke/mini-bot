#include <iostream>
#include <string>
#include <locale>
#include <thread>
#include <chrono>
#ifdef _WIN32
#include <windows.h>
#endif

#include "OllamaClient.hpp"
#include "ChatSession.hpp"
#include "BotService.hpp"

int main() {
    try {
        // 设置控制台编码
#ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
#endif
        std::setlocale(LC_ALL, ".UTF-8");
        
        // 启动 Ollama 服务
        int result = std::system("powershell -ExecutionPolicy Bypass -File C:\\code2\\mini-bot\\scripts\\run_ollama.ps1");
        if (result != 0) {
            std::cerr << "警告：Ollama 服务启动可能失败，但程序将继续运行..." << std::endl;
        }

        // 等待服务启动
        std::this_thread::sleep_for(std::chrono::seconds(3));

        // 初始化组件
        ChatSession session(1000);
        OllamaClient client("http://localhost:11434", "deepseek-r1:8b");
        BotService bot(session, client);
        
        std::cout << "欢迎使用 Mini-Bot！输入内容开始对话，输入 'exit' 退出." << std::endl;
        
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
                        std::cout << "[无响应] 请检查 Ollama 服务状态" << std::endl;
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
        std::cerr << "请确保 Ollama 已正确安装并已下载 deepseek-r1:8b 模型" << std::endl;
        return 1;
    }
    
    return 0;
} 