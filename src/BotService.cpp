#include "BotService.hpp"
#include <iostream>

BotService::BotService(ChatSession& session, OllamaClient& client)
    : session(session), client(client) {}

std::string BotService::processUserMessage(const std::string& message) {
    session.addUserMessage(message);
    auto messages = session.getMessagesJson();
    std::string response;
    bool done = false;
    client.sendChatRequest(messages, {}, [&](const nlohmann::json& chunk) -> bool {
        if (chunk.contains("message")) {
            auto& msg = chunk["message"];
            if (msg.contains("content") && !msg["content"].is_null()) {
                std::string content = msg["content"];
                response += content;
                std::cout << content << std::flush;
            }
        }
        if (chunk.contains("done") && chunk["done"] == true) {
            done = true;
        }
        return !done;
    });
    if (!response.empty()) {
        session.addAssistantMessage(response);
    }
    return response;
}