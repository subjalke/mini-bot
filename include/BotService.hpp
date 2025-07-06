#pragma once  // 放在最顶

#include "ChatSession.hpp"
#include "OllamaClient.hpp"
#include <string>

class BotService {
    public:
        BotService(ChatSession& session, OllamaClient& client);
        std::string processUserMessage(const std::string& message);
        
    private:
        ChatSession& session;
        OllamaClient& client;
};
/*
模块间交互：
BotService几乎连接了所有模块：
使用ChatSession保存对话并管理Token。
通过OllamaClient与模型对话，ToolRegistry提供工具定义及工具处理函数。
BotService也是 CLI 主程序和 UI 层进行交互的接口。
在CLI下，它直接和main_cli.cpp交互；
在UI下，它可能通过WebSocket事件处理用户消息和返回结果
*/