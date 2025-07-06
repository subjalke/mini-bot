// Ollama监听http://localhost:11434/api/chat

#include<cpr/cpr.h>
#include "OllamaClient.hpp"
#include<nlohmann/json.hpp>
#include<string>
#include<vector>
#include <functional>

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
    const std::vector<nlohmann::json>& messages,
    const std::vector<nlohmann::json>& tools,
    std::function<bool(const nlohmann::json&)> onChunk) {

    nlohmann::json request{
        {"model",    modelName},
        {"messages", messages},
        {"tools",    tools},
        {"stream",   true}
    };

    std::string buffer;
    bool stop = false;

    auto writeCallback = [&](const std::string_view& data,
                             intptr_t /*userdata*/) -> bool {
        buffer.append(data);
        size_t pos;
        while ((pos = buffer.find("\n\n")) != std::string::npos) {
            std::string event = buffer.substr(0, pos);
            buffer.erase(0, pos + 2);

            if (event.rfind("data: ", 0) == 0) {
                std::string payload = event.substr(6);
                if (payload == "[DONE]") { stop = true; break; }

                try {
                    nlohmann::json j = nlohmann::json::parse(payload);
                    if (!onChunk(j)) { stop = true; break; }
                } catch (...) { /* ignore parse errors */ }
            }
        }
        return !stop;           // true=继续; false=让 libcurl 中断
    };

    cpr::Response resp = cpr::Post(
        cpr::Url{baseUrl + "/api/chat"},
        cpr::Body{request.dump()},
        cpr::Header{{"Content-Type","application/json"},
                    {"Accept","text/event-stream"}},
        cpr::WriteCallback{writeCallback},
        cpr::Timeout{0}
    );

    if (!stop && resp.status_code != 200) {
        throw std::runtime_error("Chat request failed: " + resp.text);
    }
}
