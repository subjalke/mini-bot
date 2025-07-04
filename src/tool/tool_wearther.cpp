#include "ToolRegistry.hpp"
#include <nlohmann/json.hpp>
#include <string>

// 天气查询工具（模拟实现）
std::string weatherHandler(const nlohmann::json& args) {
    try {
        std::string city = args["city"].get<std::string>();
        
        // 模拟天气数据（实际应用中应该调用真实的天气API）
        if (city == "北京" || city == "Beijing") {
            return "北京天气：晴天，温度 20°C，湿度 45%，风速 3m/s";
        } else if (city == "上海" || city == "Shanghai") {
            return "上海天气：多云，温度 25°C，湿度 60%，风速 5m/s";
        } else if (city == "广州" || city == "Guangzhou") {
            return "广州天气：小雨，温度 28°C，湿度 80%，风速 2m/s";
        } else if (city == "深圳" || city == "Shenzhen") {
            return "深圳天气：晴天，温度 26°C，湿度 55%，风速 4m/s";
        } else {
            return "抱歉，暂不支持查询 " + city + " 的天气信息";
        }
    } catch (const std::exception& e) {
        return "错误：参数格式不正确";
    }
}

// 注册天气查询工具
void registerWeatherTool(ToolRegistry& registry) {
    nlohmann::json paramSchema = {
        {"type", "object"},
        {"properties", {
            {"city", {
                {"type", "string"},
                {"description", "要查询天气的城市名称"}
            }}
        }},
        {"required", {"city"}}
    };
    
    registry.registerTool(
        "weather",
        "查询指定城市的天气信息，包括温度、湿度、风速等",
        paramSchema,
        weatherHandler
    );
}
