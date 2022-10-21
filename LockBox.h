#ifndef LOCKBOX
#define LOCKBOX

#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <vector>
#include <string>
#include <functional>

namespace fs = std::filesystem;

class LockBox {
    private:
        std::string file_path;
        int convert_password(const std::string&);
        std::string get_name(void);
        void initialize(void);
    
    public:
        LockBox();
        bool set_file(const std::string &);
        void encrypt(const std::string&);
        void decrypt(const std::string&);
        void showHelp();
};



#endif // LOCKBOX_H