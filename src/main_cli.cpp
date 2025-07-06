#include<nlohmann/json.hpp>
#include "BotService.hpp"
#include "ChatSession.hpp"
#include "OllamaClient.hpp"
#include "ToolRegistry.hpp"
#include <iostream>
#include <string>



int main() {
    try {
        std::system("powershell -ExecutionPolicy Bypass -File scripts/run_ollama.ps1");

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
            
            if (userInput == "exit" || userInput == "quit") {
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