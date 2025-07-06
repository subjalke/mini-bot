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

int main() {
    try {
        // 启动 Ollama 服务
        std::cout << "正在启动 Ollama 服务..." << std::endl;
        int result = std::system("powershell -ExecutionPolicy Bypass -File C:\\code2\\mini-bot\\scripts\\run_ollama.ps1");
        if (result != 0) {
            std::cerr << "警告：Ollama 服务启动可能失败，但程序将继续运行..." << std::endl;
        }

        // 等待一下让服务完全启动
        std::this_thread::sleep_for(std::chrono::seconds(2));

        // 初始化核心对象
        ChatSession session(1000); // 最大1000个token
        OllamaClient client("http://localhost:11434", "deepseek-r1:8b");
        ToolRegistry toolRegistry;
        
        // 创建BotService（会自动注册所有工具）
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
                std::string response = bot.processUserMessage(userInput);
                std::cout << std::endl;
            }
        }
        
        std::cout << "再见！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}