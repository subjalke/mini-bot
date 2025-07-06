#include "ToolRegistry.hpp"
#include "tools/Calculator.hpp"
#include "tools/Weather.hpp"
#include <iostream>

ToolRegistry::ToolRegistry() {
    // 注册计算器工具
    registerTool("calculator", 
        [](const nlohmann::json& args) {
            std::string expression = args["expression"];
            return Calculator::calculate(expression);
        },
        Calculator::getToolDefinition()
    );
    
    // 注册天气工具
    registerTool("get_weather",
        [](const nlohmann::json& args) {
            std::string city = args["city"];
            return Weather::getWeather(city);
        },
        Weather::getToolDefinition()
    );
}

void ToolRegistry::registerTool(const std::string& name, ToolFunction func, const nlohmann::json& definition) {
    tools[name] = func;
    definitions[name] = definition;
}

std::string ToolRegistry::executeTool(const std::string& name, const nlohmann::json& args) {
    auto it = tools.find(name);
    if (it == tools.end()) {
        return "错误：工具 '" + name + "' 未找到";
    }
    
    try {
        return it->second(args);
    } catch (const std::exception& e) {
        return "工具执行错误：" + std::string(e.what());
    }
}

std::vector<nlohmann::json> ToolRegistry::getToolDefinitions() const {
    std::vector<nlohmann::json> result;
    for (const auto& pair : definitions) {
        result.push_back(pair.second);
    }
    return result;
}

