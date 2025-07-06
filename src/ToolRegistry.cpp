#include "ToolRegistry.hpp"

void ToolRegistry::registerTool(
    const std::string& name, 
    const std::string& description, 
    const nlohmann::json& paramSchema, 
    std::function<std::string(const nlohmann::json&)> handler
    ){
    this->tools[name] = {name, description, paramSchema, handler};
}

ToolInfo* ToolRegistry::getToolInfo(const std::string& name) {
    auto it = tools.find(name);
    if (it != tools.end()) {
        return &(it->second);
    }
    return nullptr;
}

std::vector<nlohmann::json> ToolRegistry::getToolDefinitionsJson() const {
    std::vector<nlohmann::json> defs;
    for (const auto& kv : tools) {
        const ToolInfo& info = kv.second;
        nlohmann::json def = {
            {"name", info.name},
            {"description", info.description},
            {"parameters", info.paramSchema}
        };
        defs.push_back(def);
    }
    return defs;
}

