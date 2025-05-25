#include <iostream>
#include <string.h>

#include "help.h"
#include "ai.h"
#include "main.h"

// Global variables

const char* inUseModelPtr;
char* extraInstructionsPtr;
const char* apiKeyPtr;
const char* apiBasePtr;

int programMode = 0;

// Function prototypes

int runGitDiffMode(std::string extraInstructionsRef); // programMode = 0
int runFeedbackMode(std::string extraInstructionsRef); // programMode = 1

void setinUseModelPtr(const char* model);
void setapiKeyPtr(const char* keyPtr);
void setapiBasePtr(const char* basePtr);

std::string get_git_diff();
std::string extract_git_commit_message(const std::string& response);
std::string escape_shell_chars(const std::string& strRef);
std::string replace_placeholder(const std::string& prompt, const std::string& instructions);

// Main function

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
                setinUseModelPtr(argv[++i]);
            }
        } else {
            // If no argument is passed, use the default model
            setinUseModelPtr(DefaultModel);
        }

        // If the argument is "--key", use the passed key
        if (strcmp(argv[i], "--key") == 0) {
            if (i + 1 < argc) {
                setapiKeyPtr(argv[++i]);
            } else {
                // Use the API key from the environment variable
                setapiKeyPtr(getenv("OPENAI_API_KEY"));
            }
        }
        
        // If the argument is "--base", use the passed base
        if (strcmp(argv[i], "--base") == 0) {
            if (i + 1 < argc) {
                setapiBasePtr(argv[++i]);
            } else {
                // Use the API base from the environment variable
                setapiBasePtr(API_BASE_URL);
            }
        }

        // Handle feedback mode
        if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--feedback") == 0) {
            programMode = 1;
        }

        // See if user has input extra instructions
        if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--instructions") == 0) {
            if (i + 1 < argc) {
                extraInstructionsPtr = argv[++i];
            }

            #ifdef _DEBUG
            std::cout << "Extra instructions: " << extraInstructionsPtr << "\n";
            #endif
        } else {
            extraInstructionsPtr = nullptr;
        }
    }

    int returnValue = 0;

    if (programMode == 0) {
        std::string extraInstructions = extraInstructionsPtr ? extraInstructionsPtr : "";
        returnValue = runGitDiffMode(extraInstructions);
    } else if (programMode == 1) {
        std::string extraInstructions = extraInstructionsPtr ? extraInstructionsPtr : "";
        returnValue = runFeedbackMode(extraInstructions);
    }

    return returnValue;
}

// Program modes

int runGitDiffMode(std::string extraInstructionsRef) {
    // Get the git diff
    // Assemble the system prompt
    std::string systemPrompt = replace_placeholder(SystemPrompt, extraInstructionsRef);

    // Get the git diff
    std::string gitDiff = get_git_diff();

    // Print out the git diff to the user
    std::string gitDiffUser = "git diff --cached --color --compact-summary";
    std::cout << "--------------------------------" << std::endl;
    std::cout << "Git diff:" << std::endl;
    std::cout << "--------------------------------" << std::endl;
    system(gitDiffUser.c_str());
    std::cout << "--------------------------------" << std::endl;

    #ifdef _DEBUG
    std::cout << "System prompt: " << systemPrompt << std::endl;
    #endif

    std::string response = get_ai_response(
        systemPrompt,
        inUseModelPtr ? inUseModelPtr : DefaultModel,
        apiKeyPtr ? apiKeyPtr : "",
        apiBasePtr ? apiBasePtr : "",
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
        // Run the git commit command with escaped message
        std::string commitCommand = "git commit -m \"" + escape_shell_chars(gitCommitMessage) + "\"";
        system(commitCommand.c_str());
    } else {
        std::cout << "Aborting commit..." << std::endl;
        return 0;
    }

    // Ask user if they want to push the commit
    std::cout << "Do you want to push the commit? (y/n): ";
    char push;
    std::cin >> push;
    if (push == 'y') {
        std::cout << "Pushing commit..." << std::endl;
        std::string pushCommand = "git push";
        system(pushCommand.c_str());
    }

    return 0;
}

int runFeedbackMode(std::string extraInstructionsRef) {
    // Get the git diff
    std::string gitDiff = get_git_diff();
    
    // Assemble the feedback prompt
    std::string feedbackPrompt = replace_placeholder(FeedbackPrompt, extraInstructionsRef);

    #ifdef _DEBUG
    std::cout << "Feedback prompt: " << feedbackPrompt << std::endl;
    #endif

    std::string response = get_ai_response(
        feedbackPrompt,
        inUseModelPtr ? inUseModelPtr : DefaultModel,
        apiKeyPtr ? apiKeyPtr : "",
        apiBasePtr ? apiBasePtr : "",
        gitDiff
    );

    // Print the feedback to the console
    std::cout << "--------------------------------" << std::endl;
    std::cout << "Code Review Feedback:" << std::endl;
    std::cout << "--------------------------------" << std::endl;
    std::cout << response << std::endl;
    std::cout << "--------------------------------" << std::endl;

    return 0;
}

// Setter functions
void setinUseModelPtr(const char* model) {
    inUseModelPtr = model;
}

void setapiKeyPtr(const char* keyPtr) {
    apiKeyPtr = keyPtr;
}

void setapiBasePtr(const char* basePtr) {
    apiBasePtr = basePtr;
}

// Helper functions

std::string replace_placeholder(const std::string& prompt, const std::string& instructions) {
    std::string result = prompt;
    size_t placeholderPos = result.find("{user_instructions}");
    if (placeholderPos != std::string::npos) {
        if (!instructions.empty()) {
            result.replace(placeholderPos, strlen("{user_instructions}"), instructions);
        } else {
            result.erase(placeholderPos, strlen("{user_instructions}"));
        }
    }
    return result;
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
    const std::string codeBlockDelimiter = "```";
    size_t pos = response.find(header);

    // If header not found, try to find code block delimiter
    if (pos == std::string::npos) {
        pos = response.find(codeBlockDelimiter);
        if (pos != std::string::npos) {
            // Skip the delimiter and any whitespace
            pos += codeBlockDelimiter.length();
            while (pos < response.length() && std::isspace(response[pos])) {
                pos++;
            }
        } else {
            // If neither header nor code block found, use the entire message
            return response;
        }
    } else {
        // Skip the header and any whitespace after it
        pos += header.length();
        while (pos < response.length() && std::isspace(response[pos])) {
            pos++;
        }
    }

    // Remove all occurrences of the code block delimiter from the response
    std::string cleanedResponse = response;
    size_t delimPos;
    while ((delimPos = cleanedResponse.find(codeBlockDelimiter)) != std::string::npos) {
        cleanedResponse.erase(delimPos, codeBlockDelimiter.length());
    }

    // Adjust position for cleanedResponse
    size_t cleanedPos = cleanedResponse.find(header);
    if (cleanedPos != std::string::npos) {
        cleanedPos += header.length();
        while (cleanedPos < cleanedResponse.length() && std::isspace(cleanedResponse[cleanedPos])) {
            cleanedPos++;
        }
        return cleanedResponse.substr(cleanedPos);
    } else {
        // Fallback: return the original substring after header, but with delimiters removed
        return cleanedResponse.substr(pos);
    }
}

std::string escape_shell_chars(const std::string& strRef) {
    std::string escaped;
    for (char c : strRef) {
        if (c == '"' || c == '\\' || c == '$' || c == '`' || c == '!' || c == '&' || c == '|' || c == ';' || c == '(' || c == ')') {
            escaped += '\\';
        }
        escaped += c;
    }
    return escaped;
}