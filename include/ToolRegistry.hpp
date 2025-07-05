#pragma once
#include <nlohmann/json.hpp>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

struct ToolInfo {
    std::string name;
    std::string description;
    nlohmann::json paramSchema;
    std::function<std::string(const nlohmann::json&)> handler;
};

class ToolRegistry {
private:
    std::unordered_map<std::string, ToolInfo> tools;
public:
    void registerTool(const std::string& name,
                      const std::string& description,
                      const nlohmann::json& paramSchema,
                      std::function<std::string(const nlohmann::json&)> handler);
    ToolInfo* getToolInfo(const std::string& name);
    std::vector<nlohmann::json> getToolDefinitionsJson() const;
};
