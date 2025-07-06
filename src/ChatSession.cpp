#include "ChatSession.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

ChatSession::ChatSession(size_t maxSize) : maxSize(maxSize) {}

void ChatSession::addUserMessage(const std::string& content) {
    messages.push_back({{"role", "user"}, {"content", content}});
    trimHistory();
}

void ChatSession::addAssistantMessage(const std::string& content) {
    messages.push_back({{"role", "assistant"}, {"content", content}});
    trimHistory();
}

void ChatSession::addToolResultMessage(const std::string& toolName, const std::string& result) {
    messages.push_back({{"role", "tool"}, {"content", result}, {"name", toolName}});
    trimHistory();
}

std::vector<nlohmann::json> ChatSession::getMessagesJson() const {
    return messages;
}

void ChatSession::trimHistory() {
    while (messages.size() > maxSize) {
        messages.erase(messages.begin());
    }
}
