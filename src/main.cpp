#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "You passed " << argc << " arguments:\n";
    for (int i = 0; i < argc; ++i) {
        std::cout << "argv[" << i << "] = " << argv[i] << "\n";
    }
    return 0;
}
