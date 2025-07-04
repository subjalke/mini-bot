#include "ToolRegistry.hpp"
#include <nlohmann/json.hpp>
#include <sstream>
#include <cmath>

// 计算器工具
std::string calcHandler(const nlohmann::json& args) {
    try {
        std::string operation = args["operation"].get<std::string>();
        double a = args["a"].get<double>();
        double b = args["b"].get<double>();
        
        double result = 0.0;
        if (operation == "add") {
            result = a + b;
        } else if (operation == "subtract") {
            result = a - b;
        } else if (operation == "multiply") {
            result = a * b;
        } else if (operation == "divide") {
            if (b == 0) {
                return "错误：除数不能为零";
            }
            result = a / b;
        } else if (operation == "power") {
            result = std::pow(a, b);
        } else {
            return "错误：不支持的操作 " + operation;
        }
        
        std::ostringstream oss;
        oss << result;
        return oss.str();
    } catch (const std::exception& e) {
        return "错误：参数格式不正确";
    }
}

// 注册计算器工具
void registerCalcTool(ToolRegistry& registry) {
    nlohmann::json paramSchema = {
        {"type", "object"},
        {"properties", {
            {"operation", {
                {"type", "string"},
                {"description", "计算操作：add(加法), subtract(减法), multiply(乘法), divide(除法), power(幂运算)"}
            }},
            {"a", {
                {"type", "number"},
                {"description", "第一个操作数"}
            }},
            {"b", {
                {"type", "number"},
                {"description", "第二个操作数"}
            }}
        }},
        {"required", {"operation", "a", "b"}}
    };
    
    registry.registerTool(
        "calc",
        "执行基本数学计算，支持加法、减法、乘法、除法和幂运算",
        paramSchema,
        calcHandler
    );
}  