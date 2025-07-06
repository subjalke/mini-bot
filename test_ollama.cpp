#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

int main() {
    std::cout << "测试 Ollama 连接..." << std::endl;
    
    // 测试服务是否运行
    std::cout << "1. 检查 Ollama 服务状态..." << std::endl;
    cpr::Response response = cpr::Get(cpr::Url{"http://localhost:11434/api/tags"});
    std::cout << "状态码: " << response.status_code << std::endl;
    std::cout << "响应: " << response.text << std::endl;
    
    if (response.status_code != 200) {
        std::cout << "Ollama 服务未运行或无法访问" << std::endl;
        return 1;
    }
    
    // 测试模型
    std::cout << "\n2. 测试模型 deepseek-r1:8b..." << std::endl;
    nlohmann::json request = {
        {"model", "deepseek-r1:8b"},
        {"messages", {{{"role", "user"}, {"content", "hello"}}}},
        {"stream", false}
    };
    
    cpr::Response chatResponse = cpr::Post(
        cpr::Url{"http://localhost:11434/api/chat"},
        cpr::Body{request.dump()},
        cpr::Header{{"Content-Type", "application/json"}}
    );
    
    std::cout << "聊天请求状态码: " << chatResponse.status_code << std::endl;
    std::cout << "聊天响应: " << chatResponse.text << std::endl;
    
    if (chatResponse.status_code == 200) {
        try {
            auto json = nlohmann::json::parse(chatResponse.text);
            if (json.contains("message") && json["message"].contains("content")) {
                std::cout << "模型响应: " << json["message"]["content"] << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "JSON解析错误: " << e.what() << std::endl;
        }
    }
    
    return 0;
} 