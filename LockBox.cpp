#include "LockBox.h"

LockBox::LockBox() {
    initialize();
}


void LockBox::set_password(const std::string &pwd) {
    int password = 0;
    for (auto c: pwd)
        password += c;

    key = password;
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
    std::string::reverse_iterator ritr = file_path.rbegin();
    std::string file_name;
    while (*ritr != '/' && ritr != file_path.rend()) {
        // Either just decrement and use the string construct to reverse or do what i did before
        std::string temp(1, *ritr);
        file_name += temp;
        ritr++;
    }
    std::reverse(file_name.begin(), file_name.end());
    return file_name;
}


void LockBox::encrypt(const std::string &password) {
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


void LockBox::decrypt(const std::string &password) {
    // First verify that the file-password matches an entry in the data file
    std::fstream pwd("./LockBox/data/pwd.txt", std::fstream::in);
    // Vector to store all the lines of the file
    std::vector<std::string> data_file;
    // String to hold the current line of the file
    std::string line;
    // Name of the file to look for
    std::string file_name = get_name();
    // Hash function to get the hashed version of the password which is what is actaully stored
    std::hash<std::string> hashed;
    std::cout << "Hashed password int: " << hashed(password) << std::endl;
    std::cout << "hashed password string: " << std::to_string(hashed(password)) << std::endl;
    // Flag for if the file-password exists
    bool exists = false;
    // Add all contents to vector
    while (getline(pwd, line)) {
        if (line.substr(file_name.size() + 1) == std::to_string(hashed(password))) {
            exists = true;
            continue;
        }
        data_file.push_back(line);
    }

    if (exists == false) {
        std::cout << "Error: invalid password" << std::endl;
        return;
    }

    // Close the pwd file
    pwd.close();
    // Now re-open it to overwrite all of the contents to remove the file-password entry
    std::fstream pwd1("./LockBox/data/pwd.txt", std::fstream::trunc);
    for (auto l: data_file)
        pwd1 << l;
    // Close it again
    pwd1.close();
    // After verifying the password, the encrypted file and the new file it will be written to need to be opened.
    std::fstream fin(file_path, std::fstream::in), fout("./LockBox/files/" + file_name, std::fstream::out);
    // Now go char by char and subtract the key from each char
    char c;
    while (fin >> std::noskipws >> c) {
        int value = c - key;

        // Write it to the outputfile 
        fout << (char)value;
    }
    // Close the files
    fin.close();
    fout.close();
}