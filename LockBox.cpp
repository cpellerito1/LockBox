#include "LockBox.h"

LockBox::LockBox() {
    initialize();
}


bool LockBox::set_file(const std::string &fp) {
    std::cout << "file: " << fp << std::endl; 
    std::ifstream f(fp);
    if (f.good()) {
        file_path = fp;
        return true;
    } else 
        return false;
}


int LockBox::convert_password(const std::string &pwd) {
    int password = 0;
    for (auto c: pwd)
        password += c;
    return password;
}


void LockBox::initialize() {
    std::string p = "./LockBox";
    if (!fs::exists(p))
        fs::create_directories(p);
    if (!fs::exists(p + "/files"))
        fs::create_directories(p + "/files");
    if (!fs::exists(p + "/data")) {
        fs::create_directories(p + "/data");
        // Also create reference file
        std::ofstream pwd(p + "/data/pwd.txt");
    }
}


void LockBox::showHelp() {
    std::cout << "How to use LockBox:\n";
    std::cout << "Format = LockBox [options]\n";
    std::cout << "Options:\n-f 'path to file'\n-e to encrypt a file\n-d to decrypt a file\n";
    std::cout << "-p 'password' used for encryption/decryption\n";
    std::cout << "If both -e and -d are specified, whichever one comes later will be what is used" << std::endl;
}


std::string LockBox::get_name() {
    // Get iterator pointing to the . in the file extension
    std::cout << "File path: " << file_path << std::endl;
    auto itr = std::find(file_path.begin(), file_path.end(), '.');
    for (;itr != file_path.begin(); itr--) {
        std::cout << "itr: " << *itr << std::endl;
    }
    if (itr != file_path.end()) {
        while (*(--itr) != '/' || itr == file_path.begin())
            ;
        if (itr != file_path.begin()) {
            std::string s(file_path.begin() + (++itr - file_path.begin()), file_path.end()-1);
            return s;
        } else {
            // This means the file was given with no path just the file name
            return file_path;
        }
    }
    return "error";
}


void LockBox::encrypt(const std::string &password) {
    // First convert string password to int representation
    int key = convert_password(password);

    // Open the 3 files we will need, the file to be encrypted, the file to write that to,
    // and the pwd file in the data folder to store the hashed password and file name associated to it
    std::fstream fin(file_path, std::fstream::in), fout, pwd("./LockBox/data/pwd.txt", std::fstream::app);
    // Get the name of the file to be encrypted
    std::string out_file = get_name();
    std::cout << "out file: " << out_file << std::endl;
    if (out_file == "error") {
        std::cout << "Error: invalid file name" << std::endl;
        return;
    }
    fout.open("./LockBox/files/e_" + out_file, std::fstream::out);

    // Now go char by char and add the key to each char
    char c;
    while (fin >> std::noskipws >> c) {
        int value = (int)c + key;

        // Write it to the outputfile 
        fout << (char)value;
    }

    // Close fin and fout
    fin.close();
    fout.close();
    // Write the output file name and the password in the format of file:password(hashed)
    // First hash the password
    std::hash<std::string> hashed;
    pwd << "e_" + out_file << ":" << hashed(password) << "\n";
    pwd.close();
}