#include "ai.h"
#include <openai/openai.hpp>

std::string get_ai_response(const std::string& promptRef, const std::string& modelRef, const std::string& apiKeyRef, const std::string& apiBaseRef, const std::string& gitDiffRef) {
    openai::start(apiKeyRef, "", true, apiBaseRef);

    nlohmann::json chat_json = {
        {"model", modelRef},
        {"messages", {
            {
                {"role", "user"},
                {"content", promptRef}
            },
            {
                {"role", "user"},
                {"content", "Write the git commit message. Here is the git diff: \n" + gitDiffRef}
            }
        }},
        {"max_tokens", 8192},
        {"temperature", 0.5}
    };

    auto chat = openai::chat().create(chat_json);

    // Get the choices -> message -> content
    std::string response = chat["choices"][0]["message"]["content"];

    return response;
}