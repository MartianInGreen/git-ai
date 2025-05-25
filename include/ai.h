#ifndef AI_H
#define AI_H

#include <string>

std::string get_ai_response(const std::string& prompt, const std::string& model, const std::string& apiKey, const std::string& apiBase, const std::string& gitDiff);

#endif // AI_H
