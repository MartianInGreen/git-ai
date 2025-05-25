#include <iostream>
#include <string.h>

#include "help.h"
#include "ai.h"
#include "main.h"

const char* inUseModel;
char* extraInstructions;
const char* apiKey;
const char* apiBase;

void setInUseModel(const char* model);
std::string get_git_diff();
std::string extract_git_commit_message(const std::string& response);
void setApiKey(const char* key);
void setApiBase(const char* base);

int main(int argc, char* argv[]) {
    #ifdef _DEBUG
    std::cout << "You passed " << argc << " arguments:\n";
    for (int i = 0; i < argc; ++i) {
        std::cout << "argv[" << i << "] = " << argv[i] << "\n";
    }
    #endif

    // Parse arguments
    for (int i = 1; i < argc; ++i) {
        // If the argument is "-h" or "--help", print the help message
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_help();
            return 0;
        }
        // If the argument is "-v" or "--version", print the version
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            std::cout << "GIA version " << Version << "\n";
            return 0;
        }
        // If the argument is "-m" or "--model", use the passed model
        if (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--model") == 0) {
            if (i + 1 < argc) {
                // If an argument is passed, use it as the model
                setInUseModel(argv[++i]);
            }
        } else {
            // If no argument is passed, use the default model
            setInUseModel(DefaultModel);
        }

        // If the argument is "--key", use the passed key
        if (strcmp(argv[i], "--key") == 0) {
            if (i + 1 < argc) {
                setApiKey(argv[++i]);
            } else {
                // Use the API key from the environment variable
                setApiKey(getenv("OPENAI_API_KEY"));
            }
        }
        
        // If the argument is "--base", use the passed base
        if (strcmp(argv[i], "--base") == 0) {
            if (i + 1 < argc) {
                setApiBase(argv[++i]);
            } else {
                // Use the API base from the environment variable
                setApiBase(API_BASE_URL);
            }
        }

        // See if user has input extra instructions
        if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--instructions") == 0) {
            if (i + 1 < argc) {
                extraInstructions = argv[++i];
            }

            #ifdef _DEBUG
            std::cout << "Extra instructions: " << extraInstructions << "\n";
            #endif
        }
    }

    // Assemble the system prompt
    std::string systemPrompt = SystemPrompt;

    // Replace the {user_instructions} placeholder with the extra instructions if it exists, otherwise just remove it
    if (extraInstructions != nullptr) {
        systemPrompt.replace(systemPrompt.find("{user_instructions}"), strlen("{user_instructions}"), extraInstructions);
    } else {
        systemPrompt.erase(systemPrompt.find("{user_instructions}"), strlen("{user_instructions}"));
    }

    // Get the git diff
    std::string gitDiff = get_git_diff();

    #ifdef _DEBUG
    std::cout << "System prompt: " << systemPrompt << std::endl;
    #endif

    std::string response = get_ai_response(
        extraInstructions ? extraInstructions : "",
        inUseModel ? inUseModel : DefaultModel,
        apiKey ? apiKey : "",
        apiBase ? apiBase : "",
        gitDiff
    );
    
    #ifdef _DEBUG
    std::cout << "Response: " << response << std::endl;
    #endif

    // Extract the git commit message from the response
    std::string gitCommitMessage = extract_git_commit_message(response);

    #ifdef _DEBUG
    std::cout << "Git commit message: " << gitCommitMessage << std::endl;
    #endif

    // Print the git commit message to the console
    std::cout << "--------------------------------" << std::endl;
    std::cout << "Git commit message:" << std::endl;
    std::cout << "--------------------------------" << std::endl;
    std::cout << gitCommitMessage << std::endl;
    std::cout << "--------------------------------" << std::endl;

    // Ask the user if they want to commit the message
    std::cout << "Do you want to commit this message? (y/n): ";
    char commit;
    std::cin >> commit;
    if (commit == 'y') {
        std::cout << "Committing message..." << std::endl;
        // Run the git commit command
        std::string commitCommand = "git commit -m \"" + gitCommitMessage + "\"";
        system(commitCommand.c_str());
    }

    return 0;
}

void setInUseModel(const char* model) {
    inUseModel = model;
}

std::string get_git_diff() {
    std::string diff;
    FILE* pipe = popen("git diff --cached", "r");
    if (!pipe) {
        return "Error: Could not execute git diff command";
    }
    
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        diff += buffer;
    }
    
    pclose(pipe);
    return diff;
}

std::string extract_git_commit_message(const std::string& response) {
    const std::string header = "**Suggested commit message:**";
    size_t pos = response.find(header);
    
    if (pos == std::string::npos) {
        return "Error: No commit message found in the response";
    }
    
    // Skip the header and any whitespace after it
    pos += header.length();
    while (pos < response.length() && std::isspace(response[pos])) {
        pos++;
    }
    
    return response.substr(pos);
}

void setApiKey(const char* key) {
    apiKey = key;
}

void setApiBase(const char* base) {
    apiBase = base;
}