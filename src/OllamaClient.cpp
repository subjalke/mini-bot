// Ollama监听http://localhost:11434/api/chat

#include<cpr/cpr.h>
#include "include/OllamaClient.hpp"
#include<nlohmann/json.hpp>
#include<string>
#include<vector>

OllamaClient::OllamaClient(const std::string& baseUrl, const std::string& modelName){
    this->baseUrl = baseUrl;
    this->modelName = modelName;
    // 考虑加入命令,在命令行中拉起模型
    // 发送一个空请求,提前加载模型,这个空请求不加入ChatSession管理的对话历史中
    nlohmann::json request = {
        {"model", modelName},
        {"messages", {}}
    };
    cpr::Response response = cpr::Post(cpr::Url{baseUrl + "/api/chat"}, cpr::Body{request.dump()}, cpr::Header{{"Content-Type", "application/json"}});
    if(response.status_code != 200){
        throw std::runtime_error("Failed to load model: " + response.text);
    }
}

void OllamaClient::sendChatRequest(
    const std::vector<nlohmann::json>& messages, // 处理好的信息列表
    const std::vector<nlohmann::json>& tools, 
    std::function<bool(const nlohmann::json&)> onChunk
    ){
    // 创建请求
    nlohmann::json request = {
        {"model", modelName},
        {"messages", messages},
        {"tools", tools},
        {"stream", true}
    };
    
    std::string buffer;
    auto writeCallback = [&](std::string data) -> bool {
        buffer += data;
        // 假设每个 JSON 块以换行符分隔
        size_t pos;
        while ((pos = buffer.find('\n')) != std::string::npos) {
            std::string jsonStr = buffer.substr(0, pos);
            buffer.erase(0, pos + 1);
            
            if (!jsonStr.empty()) {
                try {
                    nlohmann::json responseJson = nlohmann::json::parse(jsonStr);
                    if (!onChunk(responseJson)) {
                        return false; // 中断流式读取
                    }
                } catch (const std::exception& e) {
                    // 忽略解析错误，继续处理
                }
            }
        }
        return true; // 继续读取
    };
    
    cpr::Response response = cpr::Post(
        cpr::Url{baseUrl + "/api/chat"}, 
        cpr::Body{request.dump()}, 
        cpr::Header{{"Content-Type", "application/json"}},
        cpr::WriteCallback{writeCallback}  
    );
    
    if (response.status_code != 200) {
        throw std::runtime_error("Chat request failed: " + response.text);
    }
}
