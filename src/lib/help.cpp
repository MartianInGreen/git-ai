#include <iostream>

void print_help() {
    std::cout << "Usage: gia [options]\n";
    std::cout << "Options:\n";
    std::cout << "  -h, --help    Show this help message\n";
    std::cout << "  -v, --version Show version information\n";
    std::cout << "  -m, --model   Set the model to use. Uses gpt-4.1-mini by default.\n";
    std::cout << "  -t, -i, --instructions \"...\"       Include extra instructions to the model.\n"; 
    std::cout << "  --key \"...\"       Set the API key to use.\n";
    std::cout << "  --base \"...\"       Set the API base URL to use.\n";
    std::cout<< std::endl; 
}