#pragma once  // 放在最顶

#include "ChatSession.hpp"
#include "OllamaClient.hpp"
#include "ToolRegistry.hpp"
#include "ToolRegistration.hpp"
#include "util.hpp"
class BotService {
    private:
        ChatSession session;       // 管理会话历史
        OllamaClient client;       // 模型接口客户端
        ToolRegistry toolRegistry; // 工具注册表
        // （如果需要WS，可有WebSocket服务器相关成员，但CLI版可忽略）
    public:
        BotService( const ChatSession& session, 
                    const OllamaClient& client, 
                    ToolRegistry& registry
                    );
                    //构造时注入已有的会话管理、模型客户端和工具库，并注册所有工具。
        std::string processUserMessage(const std::string& userInput);
        //处理一次用户输入，并返回最终的模型回答。
        /*
        实现流程：
        记录用户消息： 调用 session.addUserMessage(userInput) 保存到会话历史。
        准备请求模型： 从 session 获取消息JSON列表；从 toolRegistry 获取工具定义JSON列表。
        发送请求并流处理： 使用 client.sendChatRequest(messages, tools, onChunkCallback).
        onChunkCallback 是一个lambda，捕获 BotService 和一些输出缓冲，用于处理每个到来的JSON片段：
        检查 JSON 中是否有 "tool_calls" 字段：
        如果有工具调用请求： 提取函数名和参数构成的 JSON。然后：
        a. 在控制台打印一个提示（例如"[调用工具] 模型请求使用工具X...正在查询"），以告知用户过程。
        b. 查找对应 ToolInfo： auto* toolInfo = toolRegistry.getToolInfo(name)，确保不为空。
        c. 调用 toolInfo->handler(argumentsJson) 执行工具函数，得到结果字符串。
        d. 将结果打印或记录日志（CLI下可打印，比如"→ 工具返回: ..."）。
        e. 停止当前流：令回调返回false，中止 sendChatRequest（因为模型等待工具结果，我们需要重新调用模型）。
        f. 将工具结果加入会话：session.addToolResultMessage(name, resultStr)。
        g. 二次请求模型： 准备包含新messages的请求，再次调用 sendChatRequest，但这一次不需要附加tools列表了吗？（其实tools列表还是要提供，以防模型后续仍可调用其他工具）。继续使用相同的onChunkCallback处理后续。
        （可以用循环来实现：外层while，当检测到tool_calls时循环再次请求模型。）
        如果没有tool_calls： 正常的模型内容输出：
        提取 json["message"]["content"] 文本，如果非空，则将其追加到输出缓冲区。为了在CLI实时显示，也可以逐步打印该文本片段并刷新stdout，这样用户可以实时看到模型回答逐字出现。
        当 json["done"] == true 时，表示模型回答结束，onChunkCallback可以返回true并让流正常结束。我们在此可以结束外层对模型调用的循环。
        其它信息：模型输出中可能还包含一些辅助内容（例如 <think> 标签等思考过程标记)，可以选择过滤或直接显示。通常 <think> ... </think> 之间的内容可能是模型内部思考，不需要全部展示，可以按需求处理。
        返回值： onChunkCallback 对普通内容片段返回 true 表示继续，遇到工具请求返回 false 触发中断。
        输出最终结果： 累积的回答文本在函数返回前可以整理一下。由于我们在过程中可能已经实时打印，这里也可以直接返回完整字符串用于上层使用（在CLI中其实不再需要，因为已经打印，但为了接口完备还是返回）。
        */

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