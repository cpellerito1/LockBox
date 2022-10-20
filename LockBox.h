#ifndef LOCKBOX
#define LOCKBOX

#include <iostream>
#include <filesystem>
#include <algorithm>
#include <string.h>

namespace fs = std::filesystem;

class LockBox {
    private:
        char *file_path;
        void initialize(void);
    
    public:
        LockBox(char*);
        int encrypt(char*);
        int decrypt(char*,char*);
};



#endif // LOCKBOX_H