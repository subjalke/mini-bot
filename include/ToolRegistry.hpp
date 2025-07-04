// 工具注册与管理

/*
ToolRegistry 类管理本地可调用工具函数的注册和调度。
模型通过调用这些工具，获取额外的信息或计算结果（类似 OpenAI 的函数调用能力）。
ToolRegistry 提供工具元数据（名称、参数说明等）给模型，并在运行时根据模型请求调用相应的C++函数实现。
*/
#include<string>
#include<nlohmann/json.hpp>
struct ToolInfo {
    std::string name;        // 工具函数名称（供模型调用）
    std::string description; // 描述
    nlohmann::json paramSchema; // 参数JSON模式定义 (JSON Schema 格式)
    /* 
    "type": "object",
    "properties": {
      "参数1": {
        "type": "string",
        "description": "参数1描述"
      },
      "参数2": {
        "type": "integer",
        "description": "参数2描述"
      }
    */
    std::function<std::string(const nlohmann::json&)> handler; // 工具实现函数，传入参数JSON，返回结果字符串
};
class ToolRegistry {
private:
    std::unordered_map<std::string, ToolInfo> tools; // 已注册工具映射表(name -> ToolInfo)
public:
    void registerTool(
        const std::string& name, 
        const std::string& description, 
        const nlohmann::json& paramSchema, 
        std::function<std::string(const nlohmann::json&)> handler
        );
        // 注册一个工具
        /*
        实现： 将工具名称和对应的ToolInfo存入tools映射，以名称为键方便查找。
        paramSchema 定义了此函数的参数要求（采用 JSON Schema 格式，与Ollama工具接口匹配），
        handler是实际执行的C++函数。
        */
    ToolInfo* getToolInfo(const std::string& name);
    // 根据名称获取工具信息结构指针
    /*
    用途： BotService在收到模型请求要调用某工具时，
    会通过名称从注册表查询对应的ToolInfo
    （尤其需要其中的handler以调用实现）
    */
    std::vector<nlohmann::json> getToolDefinitionsJson() const;
    //获取所有工具的JSON定义列表，供发送给模型接口
    /*
    实现： 遍历已注册工具，将每个ToolInfo转换为Ollama API期望的格式
    */
    
};
/*
模块间交互：
初始化时，ToolRegistry由各工具cpp调用registerTool添加工具。
在对话过程中，当BotService检测到模型输出包含tool_calls请求时，会使用ToolRegistry.getToolInfo(name)找到对应工具，然后调用其中的handler执行，并获取结果。
同时，在每次发送请求给模型时，BotService会调用ToolRegistry.getToolDefinitionsJson()将工具列表提供给模型，使其有权限调这些函数。
*/