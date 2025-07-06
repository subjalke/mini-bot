#ifndef TOOL_REGISTRATION_HPP
#define TOOL_REGISTRATION_HPP
#pragma once  // 放在最顶

#include "ToolRegistry.hpp"
// 声明所有工具的注册函数
void registerCalcTool(ToolRegistry& registry);
void registerWeatherTool(ToolRegistry& registry);

// 注册所有工具的函数
void registerAllTools(ToolRegistry& registry);

#endif // TOOL_REGISTRATION_HPP 