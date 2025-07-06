#include "ToolRegistration.hpp"

// 注册所有工具的函数
void registerAllTools(ToolRegistry& registry) {
    registerCalcTool(registry);
    registerWeatherTool(registry);
} 