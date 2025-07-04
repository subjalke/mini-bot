#include "ChatSession.hpp"
#include<string>
#include<vector>
ChatSession::ChatSession(size_t maxSize){
    this->maxSize = maxSize;
    this->messages.clear();
}

void ChatSession::addUserMessage(const std::string& content){
    this->messages.push_back({{"role", "user"}, {"content", content}});
    trimHistory();
}
void ChatSession::addAssistantMessage(const std::string& content){
    this->messages.push_back({{"role", "assistant"}, {"content", content}});
    trimHistory();
}
void ChatSession::addToolResultMessage(const std::string& toolName, const std::string& result){
    this->messages.push_back({{"role", "tool"}, {"content", result}, {"name", toolName}});
    trimHistory();
}
std::vector<nlohmann::json> ChatSession::getMessagesJson() const{
    return this->messages;
}
void ChatSession::trimHistory(){
    while (this->messages.size() > this->maxSize){
        this->messages.erase(this->messages.begin());
    }
}   