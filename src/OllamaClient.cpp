// Ollama监听http://localhost:11434/api/chat

#include<cpr/cpr.h>
#include "OllamaClient.hpp"
#include<nlohmann/json.hpp>
#include<string>
#include<vector>
#include <functional>
#include <iostream>


OllamaClient::OllamaClient(const std::string& baseUrl, const std::string& modelName){
    this->baseUrl = baseUrl;
    this->modelName = modelName;
    
    // 发送一个空请求,提前加载模型
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
    const std::vector<nlohmann::json>& messages,
    const std::vector<nlohmann::json>& tools,
    std::function<bool(const nlohmann::json&)> onChunk) {

    nlohmann::json request{
        {"model",    modelName},
        {"messages", messages},
        {"stream",   true}
    };

    if (!tools.empty()) {
        request["tools"] = tools;
    }

    std::string buffer;
    bool stop = false;
    bool receivedAnyData = false;

    auto writeCallback = [&](const std::string_view& data,
                             intptr_t /*userdata*/) -> bool {
        if (data.length() > 0) {
            receivedAnyData = true;
            buffer.append(data);
            
            size_t pos = 0;
            while ((pos = buffer.find('\n', pos)) != std::string::npos) {
                std::string line = buffer.substr(0, pos);
                buffer.erase(0, pos + 1);
                
                if (!line.empty()) {
                    try {
                        nlohmann::json j = nlohmann::json::parse(line);
                        if (!onChunk(j)) {
                            stop = true;
                            break;
                        }
                    } catch (const std::exception& e) {
                        // 忽略解析错误，可能是部分数据
                    }
                }
            }
        }
        return !stop;
    };

    cpr::Response resp = cpr::Post(
        cpr::Url{baseUrl + "/api/chat"},
        cpr::Body{request.dump()},
        cpr::Header{{"Content-Type","application/json"}},
        cpr::WriteCallback{writeCallback},
        cpr::Timeout{0}
    );
    
    if (resp.status_code != 200) {
        throw std::runtime_error("Request failed: " + resp.text);
    }
}

nlohmann::json OllamaClient::sendChatRequestOnce(
    const std::vector<nlohmann::json>& messages,
    const std::vector<nlohmann::json>& tools) {
    nlohmann::json request{
        {"model",    modelName},
        {"messages", messages},
        {"tools",    tools},
        {"stream",   false}
    };

    cpr::Response resp = cpr::Post(
        cpr::Url{baseUrl + "/api/chat"},
        cpr::Body{request.dump()},
        cpr::Header{{"Content-Type", "application/json"}}
    );

    if (resp.status_code != 200) {
        throw std::runtime_error("Request failed: " + resp.text);
    }

    return nlohmann::json::parse(resp.text);
}
