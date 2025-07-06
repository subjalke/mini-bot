#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

int main() {
    std::cout << "=== Ollama 连接测试 ===" << std::endl;
    
    try {
        // 测试 1: 检查服务是否运行
        std::cout << "1. 检查 Ollama 服务..." << std::endl;
        cpr::Response response = cpr::Get(cpr::Url{"http://localhost:11434/api/tags"});
        std::cout << "状态码: " << response.status_code << std::endl;
        
        if (response.status_code == 200) {
            std::cout << "✓ Ollama 服务正在运行" << std::endl;
            std::cout << "可用模型: " << response.text << std::endl;
        } else {
            std::cout << "✗ Ollama 服务未运行 (状态码: " << response.status_code << ")" << std::endl;
            std::cout << "请确保 Ollama 已安装并运行" << std::endl;
            return 1;
        }
        
        // 测试 2: 检查模型是否存在
        std::cout << "\n2. 检查模型 deepseek-r1:8b..." << std::endl;
        if (response.text.find("deepseek-r1:8b") != std::string::npos) {
            std::cout << "✓ 模型 deepseek-r1:8b 已安装" << std::endl;
        } else {
            std::cout << "✗ 模型 deepseek-r1:8b 未安装" << std::endl;
            std::cout << "请运行: ollama pull deepseek-r1:8b" << std::endl;
            return 1;
        }
        
        // 测试 3: 发送简单聊天请求
        std::cout << "\n3. 发送测试聊天请求..." << std::endl;
        nlohmann::json request = {
            {"model", "deepseek-r1:8b"},
            {"messages", {{{"role", "user"}, {"content", "你好，请简单回复一下"}}}},
            {"stream", false}
        };
        
        cpr::Response chatResponse = cpr::Post(
            cpr::Url{"http://localhost:11434/api/chat"},
            cpr::Body{request.dump()},
            cpr::Header{{"Content-Type", "application/json"}}
        );
        
        std::cout << "聊天请求状态码: " << chatResponse.status_code << std::endl;
        
        if (chatResponse.status_code == 200) {
            std::cout << "✓ 聊天请求成功" << std::endl;
            try {
                auto json = nlohmann::json::parse(chatResponse.text);
                if (json.contains("message") && json["message"].contains("content")) {
                    std::string content = json["message"]["content"];
                    std::cout << "模型响应: " << content << std::endl;
                } else {
                    std::cout << "响应格式异常: " << chatResponse.text << std::endl;
                }
            } catch (const std::exception& e) {
                std::cout << "JSON解析错误: " << e.what() << std::endl;
                std::cout << "原始响应: " << chatResponse.text << std::endl;
            }
        } else {
            std::cout << "✗ 聊天请求失败" << std::endl;
            std::cout << "错误响应: " << chatResponse.text << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cout << "异常: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "\n=== 测试完成 ===" << std::endl;
    return 0;
} 