// 定义管理聊天记录的类定义
#include<string>
#include<vector>
#include<nlohmann/json.hpp>
/*
ChatSession 类负责维护多轮对话的历史记录，并提供消息裁剪和Token统计等功能，以防止上下文过长。主要特性包括：
对话消息存储： 使用适当的数据结构（例如std::vector）保存消息序列，每条消息包含角色和内容。典型的角色有：
"user"：用户消息,"assistant"：AI助手回复,"tool"：工具返回结果（用于函数调用机制）可以定义一个结构体如Message存储role和content字段。）
Token 计数与裁剪： 跟踪当前会话的Token数量。当新增消息使总Tokens超过阈值时，裁剪最早的消息以保持上下文长度在允许范围。settings.json中可能有配置如"max_tokens"或"clip_threshold"指定阈值
*/
// 模块调用
/*
ChatSession 提供简洁的接口供外部使用。
例如 BotService 每当有新用户输入或新AI回复时，
就调用 ChatSession.addUserMessage(...) 或 addAssistantMessage(...) 记录下来。
在发送请求给模型之前，BotService 调用 getMessagesJson() 获取历史记录的JSON表示。
裁剪逻辑在新增消息时自动进行，对外透明
*/
class ChatSession{
    public:
        ChatSession(size_t maxToekns);  //构造函数,初始化maxTokens,清空messages,设toeknCount为0;
        void addUserMessage(const std::string& content); 
        //添加一条信息进入messages:建立一个Message对象,role="user"
        //考虑:1.使用util.hpp中的Token估算工具计算此消息的Token长度，累加到tokenCount
        //2.调用内部裁剪函数（如trimHistory()），若当前tokenCount超出maxTokens，则从最早的消息开始移除，直到tokenCount降至阈值以
        void addAssistantMessage(const std::string& content);
        //与addUserMessage相似,role="assistant"
        void addToolResultMessage(const std::string& toolName, const std::string& result);
        //添加一条工具函数执行结果消息
        //创建Message {role: "tool", content: result} 并可能附加工具名，用于告知模型这是哪个函数的输出。例如可以在Message结构中新增name字段存放工具名。将此消息追加到历史，更新token计数并裁剪。
        //在Ollama工具功能中，使用角色"tool"携带结果和函数名是约定做法，模型会据此继续回答
        std::vector<nlohmann::json> getMessagesJson() const;
        void trimHistory();
    private:
        struct Message{                 // 考虑Ollama的接口规格?           
            std::string role;
            std::string content;
        };
        std::vector<Message>messages;   // 存储所有对话消息
        size_t tokenCount;              // 当前消息总的Token估算数
        size_t maxSize;               // Token阈值（从配置加载）
};