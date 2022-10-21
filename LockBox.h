#ifndef LOCKBOX
#define LOCKBOX

#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <vector>
#include <string>

namespace fs = std::filesystem;

class LockBox {
    private:
        std::string file_path;
        void initialize(void);
        void set_password(std::string&);
    
    public:
        LockBox();
        bool set_file(const std::string &);
        int encrypt(std::string&);
        int decrypt(std::string&);
        void showHelp();
};



#endif // LOCKBOX_H