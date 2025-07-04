#include<nlohmann/json.hpp>
#include "BotService.hpp"
#include "ChatSession.hpp"
#include "OllamaClient.hpp"
#include "ToolRegistry.hpp"
#include <iostream>
#include <string>

//  CLI 主程序入口
/*
文件职责： 初始化整个应用并运行命令行下的演示。
对于最小Demo，此文件会创建所需对象并进行一次简单对话，
或实现一个交互循环
主要实现步骤：
1.加载配置： 使用 nlohmann::json 读取 config/settings.json 文件。
解析所需字段,例如:   port（如 11434），
                    model（模型名称，如 "qwen3"），
                    clip_threshold（最大Token阈值），
2.初始化核心对象：
创建 ChatSession session(maxTokens) 使用配置的Token上限。
创建 OllamaClient client("http://localhost:" + std::to_string(port), model). （假设Ollama提供HTTP API，此URL前缀指向聊天接口）
创建 ToolRegistry toolRegistry 对象。
调用各工具注册函数，注册工具到 toolRegistry
创建 BotService bot(session, client, toolRegistry) 将上述对象组合
3.开始CLI对话：
打印欢迎信息，例如 "欢迎使用 Mini-Bot CLI. 输入内容开始对话，
输入 'exit' 退出."。
然后进入循环
通过 std::getline 获取用户整行输入。如果用户输入退出命令则跳出循环。
否则将输入传给 BotService 处理并得到回答。
由于 BotService 内部已负责打印模型回答，main 处可以不用重复打印，仅根据需要对返回值做处理
4.结束清理： 循环结束后，打印告别信息并正常退出程序
*/

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