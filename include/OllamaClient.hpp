#pragma once  // 放在最顶

#include<string>
#include<vector>
#include<nlohmann/json.hpp>
/*
OllamaClient 类封装了通过HTTP接口与本地 Ollama 模型服务交互的细节，
利用库 cpr 发送请求并处理响应。
该类支持流式回调，即逐步接收模型输出，
以便及时处理工具调用和实时显示
*/
class OllamaClient{
    public:
        OllamaClient(const std::string& baseUrl, const std::string& modelName);
        //设置模型服务URL和模型名称。
        void sendChatRequest(const std::vector<nlohmann::json>& messages, const std::vector<nlohmann::json>& tools, std::function<bool(const nlohmann::json&)> onChunk);
        /*
        发送聊天请求并流式处理响应。
        参数说明：
        messages：对话消息列表（JSON数组形式，由ChatSession.getMessagesJson()提供）。
        tools：工具列表的JSON数组定义（由ToolRegistry提供，每个元素包含工具schema，如名称、参数类型等）。
        onChunk：回调函数，接受每一块流式数据解析后的JSON对象。当回调返回false时，中断流读取（用于在需要时提前终止，例如遇到工具调用需要暂停）。
        */

   private:
        std::string baseUrl;// Ollama API 基础URL，例如 "http://localhost:11434"
        std::string modelName; // 使用的模型名称（从配置读取, 如 "qwen3" 或 "llama2" 等）
        
        // 其他配置：如是否启用流式(stream)等，可直接在请求中指定
    };
/*
BotService 会调用 OllamaClient.sendChatRequest(...) 来与模型交互。
BotService 会构建好 messages 和 tools 的 JSON，并提供一个 onChunk 回调，用于实时处理模型输出：
例如调用工具或将部分回答输出到终端。
OllamaClient并不知道上层业务逻辑，只负责HTTP通信和数据流解析，将每个JSON片段传给回调函数处理。
这样设计使得对于UI/WebSocket等不同上层，处理方式可以通过不同回调灵活应对
*/