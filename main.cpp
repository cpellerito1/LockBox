#include "LockBox.h"

int main(int argc, char *argv[]) {
    if (argc <= 2) {
        std::cout << "Error: please include a file and setting\nUse --help for more information" << std::endl;
        return -1;
    }

    // Initialize LockBox class
    LockBox lb;
    // Iterate through the input array
    std::string file_path;
    bool path = false;
    std::string password;
    bool pwd = false;
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
        else if (current == "-d")
            setting = 2;
        else if (current == "--help")
            lb.showHelp();
        
    }
    // Error checking
    bool errors = false;
    if (path == false) {
        std::cout << "Error: please include the path to the file" << std::endl;
        errors = true;
    }
    if (setting == -1) {
        std::cout << "Error: please include a setting, -e for encrypt or -d for decrypt" << std::endl;
        errors = true;
    }
    if (pwd == false) {
        std::cout << "Error: please include a password to encrypt/decrypt file" << std::endl;
        errors = true;
    }
    if (errors == true)
        return -1;

    // Run the encryption/decryption
    if (!lb.set_file(file_path)) {
        std::cout << "Error: unable to open file please try again" << std::endl;
        return -1;
    }


    switch (setting) {
        case 1:
            lb.encrypt(password);
            break;
    //     case 2:
    //         lb.decrypt(password);
     }




    return 0;

}
