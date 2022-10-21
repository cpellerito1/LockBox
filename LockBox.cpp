#include "LockBox.h"

LockBox::LockBox() {
    initialize();
}


bool LockBox::set_file(const std::string &fp) {
    std::ifstream f(fp);
    if (f.good()) {
        file_path = fp;
        return true;
    } else 
        return false;
}


void LockBox::initialize() {
    std::string p = "./LockBox";
    if (!fs::exists(p))
        fs::create_directories(p);
    if (!fs::exists(p + "/files"))
        fs::create_directories(p + "/files");
    if (!fs::exists(p + "/data"))
        fs::create_directories(p + "/data");
}


void LockBox::showHelp() {
    std::cout << "How to use LockBox:\n";
    std::cout << "Format = LockBox [options]\n";
    std::cout << "Options:\n-f 'path to file'\n-e to encrypt a file\n-d to decrypt a file\n";
    std::cout << "-p 'password' used for encryption/decryption\n";
    std::cout << "If both -e and -d are specified, whichever one comes later will be what is used" << std::endl;
}