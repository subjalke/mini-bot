#include "BotService.hpp"
#include <iostream>
#include <nlohmann/json.hpp>

BotService::BotService(
    const ChatSession& session,
    const OllamaClient& client,
    ToolRegistry& registry
    ):session(session),client(client),toolRegistry(registry){
    // 在构造函数中注册所有工具
    registerAllTools(toolRegistry);
    std::cout << "BotService 初始化完成，已注册所有工具" << std::endl;
}

std::string BotService::processUserMessage(const std::string& userInput){
    session.addUserMessage(userInput);
    auto messages = session.getMessagesJson();
    auto tools = toolRegistry.getToolDefinitionsJson(); 
    
    std::string accumulatedResponse;
    bool hasToolCalls = false;
    
    // 使用循环处理可能的多次工具调用
    while (true) {
        hasToolCalls = false;
        accumulatedResponse.clear();
        
        client.sendChatRequest(messages, tools, [this, &accumulatedResponse, &hasToolCalls](const nlohmann::json& json) -> bool {
            // 检查是否有工具调用
            if (json.contains("tool_calls")) {
                hasToolCalls = true;
                std::cout << "[调用工具] 模型请求使用工具..." << std::endl;
                
                for (const auto& toolCall : json["tool_calls"]) {
                    std::string toolName = toolCall["name"].get<std::string>();
                    nlohmann::json arguments =
                        nlohmann::json::parse(toolCall["arguments"].get<std::string>());

                    // 查找并执行工具
                    auto* toolInfo = toolRegistry.getToolInfo(toolName);
                    if (toolInfo) {
                        std::string result = toolInfo->handler(arguments);
                        std::cout << "→ 工具返回: " << result << std::endl;
                        session.addToolResultMessage(toolName, result);
                    }
                }
                return false; // 中断流，需要重新请求模型
            }
            
            // 处理普通内容输出
            if (json.contains("message") && json["message"].contains("content")) {
                std::string content = json["message"]["content"].get<std::string>();
                if (!content.empty()) {
                    accumulatedResponse += content;
                    std::cout << content << std::flush; // 实时显示
                }
            }
            
            // 检查是否完成
            if (json.contains("done") && json["done"].get<bool>()) {
                return true; // 正常结束流
            }
            
            return true; // 继续读取
        });
        
        // 如果没有工具调用，跳出循环
        if (!hasToolCalls) {
            break;
        }
        
        // 更新消息列表，包含工具结果
        messages = session.getMessagesJson();
    }
    
    // 将最终的助手回复添加到会话历史
    if (!accumulatedResponse.empty()) {
        session.addAssistantMessage(accumulatedResponse);
    }
    
    return accumulatedResponse;
}

std::string BotService::processUserMessageSync(const std::string& userInput){
    session.addUserMessage(userInput);
    auto messages = session.getMessagesJson();
    auto tools = toolRegistry.getToolDefinitionsJson();

    std::string accumulatedResponse;

    while (true) {
        nlohmann::json resp = client.sendChatRequestOnce(messages, tools);

        if (resp.contains("tool_calls")) {
            for (const auto& call : resp["tool_calls"]) {
                std::string toolName = call["name"].get<std::string>();
                nlohmann::json args = nlohmann::json::parse(call["arguments"].get<std::string>());
                auto* info = toolRegistry.getToolInfo(toolName);
                if (info) {
                    std::string result = info->handler(args);
                    session.addToolResultMessage(toolName, result);
                }
            }
            messages = session.getMessagesJson();
            continue;
        }

        if (resp.contains("message") && resp["message"].contains("content")) {
            accumulatedResponse = resp["message"]["content"].get<std::string>();
        }

        break;
    }

    if (!accumulatedResponse.empty()) {
        session.addAssistantMessage(accumulatedResponse);
    }

    return accumulatedResponse;
}