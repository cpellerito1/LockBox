#include "LockBox.h"

int main(int argc, char *argv[]) {
    // End of input array
    char** END = argv + argc;
    
    if (argc <= 1) {
        std::cout << "Error: please include a file and setting" << std::endl;
        return -1;
    }

    // Get file path
    char *file_path;
    auto f = std::find(argv, END, "-f");
    if (f != END && f + 1 != END) {
        file_path = *f + 1;
    } else {
        std::cout << "Error: please include the path to the file" << std::endl;
        return -1;
    }

    // Get the setting, either -e for encrypt or -d for decrypt
    auto e = std::find(argv, END, "-e");
    auto d = std::find(argv, END, "-d");
    int setting;
    if (e != END && d == END) {
        setting = 0;
    } else if (e == END && d != END) {
        setting = 1;
    } else if (e != END && d != END) {
        std::cout << "Error: please only include one option, either -e for ecrypt or -d decrypt" << std::endl;
        return -1;
    } else if (e == END && d == END) {
        std::cout << "Error: please include a setting, -e for encrypt or -d for decrypt" << std::endl;
        return -1;
    }

    // LockBox lb(file_path);
    // // Run the encryption/decryption
    // switch (setting) {
    //     case 1:
    //         lb.encrypt();
    // }




    return 0;

}