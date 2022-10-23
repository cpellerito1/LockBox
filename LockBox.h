#ifndef LOCKBOX
#define LOCKBOX

#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <vector>
#include <string>
#include <functional>
#include <stack>

namespace fs = std::filesystem;

class LockBox {
    private:
        std::string file_path;
        int key;
        std::string get_name(void);
        void initialize(void);
    
    public:
        LockBox();
        void set_password(const std::string&);
        bool set_file(const std::string &);
        void encrypt(const std::string&);
        void decrypt(const std::string&);
        void showHelp();
};



#endif // LOCKBOX_H