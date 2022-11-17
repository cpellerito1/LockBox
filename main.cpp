#include "LockBox.h"

int main(int argc, char *argv[]) {
    if (argc <= 2) {
        std::cout << "Error: please include a file and setting\nUse --help for more information" << std::endl;
        return -1;
    }

    // Iterate through the input array
    std::string file_path;
    bool path = false;
    std::string password;
    bool pwd = false;
    bool facial = false;
    int setting = -1;
    for (int i = 0; i < argc; i++) {
        std::string current(argv[i]);
        if (current == "-f" && i + 1 < argc) {
            path = true;
            file_path = std::string(argv[i + 1]);
        } else if (current == "-p" && i + 1 < argc) {
            pwd = true;
            password = std::string(argv[i + 1]);
        } else if (current == "-e")
            setting = 1;
        else if (current == "-r")
            facial = true;
        else if (current == "-d")
            setting = 2;
        else if (current == "--help")
            LockBox::show_help();
        
    }
    // Error checking
    // Use bool value for errors so it can print out multiple before exiting
    bool errors = false;
    if (!path) {
        std::cout << "Error: please include the path to the file" << std::endl;
        errors = true;
    }
    if (setting == -1) {
        std::cout << "Error: please include a setting, -e for encrypt or -d for decrypt" << std::endl;
        errors = true;
    }
    if (!pwd) {
        std::cout << "Error: please include a password to encrypt/decrypt file" << std::endl;
        errors = true;
    }
    
    if (errors == true)
        return -1;
    
    // Initialize LockBox class
    LockBox lb(file_path, password);
    lb.set_face_flag(facial);

    // Run the encryption/decryption
    switch (setting) {
        case 1:
            lb.encrypt();
            break;
        case 2:
            lb.decrypt();
            break;
     }

    return 0;

}
