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
    
    std::cout << "正在初始化 OllamaClient，连接到: " << baseUrl << std::endl;
    std::cout << "使用模型: " << modelName << std::endl;
    
    // 考虑加入命令,在命令行中拉起模型
    // 发送一个空请求,提前加载模型,这个空请求不加入ChatSession管理的对话历史中
    nlohmann::json request = {
        {"model", modelName},
        {"messages", {}}
    };
    
    std::cout << "正在测试 Ollama 连接..." << std::endl;
    cpr::Response response = cpr::Post(cpr::Url{baseUrl + "/api/chat"}, cpr::Body{request.dump()}, cpr::Header{{"Content-Type", "application/json"}});
    
    std::cout << "连接测试状态码: " << response.status_code << std::endl;
    if(response.status_code != 200){
        std::cerr << "连接失败，响应内容: " << response.text << std::endl;
        throw std::runtime_error("Failed to load model: " + response.text);
    }
    
    std::cout << "Ollama 连接测试成功！" << std::endl;
}

void OllamaClient::sendChatRequest(
    const std::vector<nlohmann::json>& messages,
    const std::vector<nlohmann::json>& tools,
    std::function<bool(const nlohmann::json&)> onChunk) {

    std::cout << "正在发送聊天请求..." << std::endl;
    std::cout << "消息数量: " << messages.size() << std::endl;
    std::cout << "工具数量: " << tools.size() << std::endl;

    nlohmann::json request{
        {"model",    modelName},
        {"messages", messages},
        {"stream",   true}
    };

    // 只有当工具列表不为空时才添加tools字段
    if (!tools.empty()) {
        request["tools"] = tools;
    }

    std::cout << "请求内容: " << request.dump(2) << std::endl;

    std::string buffer;
    bool stop = false;
    bool receivedAnyData = false;

    auto writeCallback = [&](const std::string_view& data,
                             intptr_t /*userdata*/) -> bool {
        std::cout << "收到数据块，长度: " << data.length() << std::endl;
        if (data.length() > 0) {
            std::cout << "数据内容预览: " << std::string(data.data(), (data.length() < 100 ? data.length() : 100)) << std::endl;
            receivedAnyData = true;
        }
        
        buffer.append(data);
        size_t pos_crlf = 0;
        while ((pos_crlf = buffer.find("\r\n", pos_crlf)) != std::string::npos) {
            buffer.replace(pos_crlf, 2, "\n");
        }
        size_t pos;
        while ((pos = buffer.find("\n\n")) != std::string::npos) {
            std::string event = buffer.substr(0, pos);
            buffer.erase(0, pos + 2);

            std::cout << "处理事件: " << event << std::endl;

            if (event.rfind("data: ", 0) == 0) {
                std::string payload = event.substr(6);
                if (payload == "[DONE]") { 
                    std::cout << "收到结束标记" << std::endl;
                    stop = true; 
                    break; 
                }

                try {
                    nlohmann::json j = nlohmann::json::parse(payload);
                    std::cout << "解析的JSON: " << j.dump(2) << std::endl;
                    if (!onChunk(j)) { 
                        std::cout << "回调函数要求停止" << std::endl;
                        stop = true; 
                        break; 
                    }
                } catch (const std::exception& e) { 
                    std::cerr << "JSON解析错误: " << e.what() << std::endl;
                    std::cerr << "原始payload: " << payload << std::endl;
                }
            }
        }
        return !stop;           // true=继续; false=让 libcurl 中断
    };

    std::cout << "开始发送HTTP请求..." << std::endl;
    cpr::Response resp = cpr::Post(
        cpr::Url{baseUrl + "/api/chat"},
        cpr::Body{request.dump()},
        cpr::Header{{"Content-Type","application/json"},
                    {"Accept","text/event-stream"}},
        cpr::WriteCallback{writeCallback},
        cpr::Timeout{0}
    );
    
    std::cout << "HTTP请求完成，状态码: " << resp.status_code << std::endl;
    if (resp.status_code != 200) {
        std::cerr << "HTTP请求失败，错误信息: " << resp.text << std::endl;
    }
    
    if (!receivedAnyData) {
        std::cout << "警告：没有收到任何数据块" << std::endl;
    }
}
