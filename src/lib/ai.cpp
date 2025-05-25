#include "ai.h"
#include <openai/openai.hpp>

std::string get_ai_response(const std::string& prompt, const std::string& model, const std::string& apiKey, const std::string& apiBase, const std::string& gitDiff) {
    openai::start(apiKey, "", true, apiBase);

    nlohmann::json chat_json = {
        {"model", model},
        {"messages", {
            {
                {"role", "user"},
                {"content", prompt}
            },
            {
                {"role", "user"},
                {"content", "Write the git commit message. Here is the git diff: \n" + gitDiff}
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