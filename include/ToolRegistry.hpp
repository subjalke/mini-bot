#pragma once
#include <string>
#include <functional>
#include <map>
#include <nlohmann/json.hpp>
#include <vector>

class ToolRegistry {
public:
    using ToolFunction = std::function<std::string(const nlohmann::json&)>;
    
    ToolRegistry();
    void registerTool(const std::string& name, ToolFunction func, const nlohmann::json& definition);
    std::string executeTool(const std::string& name, const nlohmann::json& args);
    std::vector<nlohmann::json> getToolDefinitions() const;
    
private:
    std::map<std::string, ToolFunction> tools;
    std::map<std::string, nlohmann::json> definitions;
};
