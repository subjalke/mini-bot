#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

// ChatSession 用于维护对话历史并在超出阈值时裁剪最旧的记录
class ChatSession {
public:
    explicit ChatSession(size_t maxSize);
    void addUserMessage(const std::string& content);
    void addAssistantMessage(const std::string& content);
    void addToolResultMessage(const std::string& toolName, const std::string& result);
    std::vector<nlohmann::json> getMessagesJson() const;
    void trimHistory();
private:
    std::vector<nlohmann::json> messages;
    size_t maxSize;
};
