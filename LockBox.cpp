#include "LockBox.h"

LockBox::LockBox(const std::string &fp, const std::string &pwd) {
    set_file(fp);
    set_password(pwd);
    initialize();
}


void LockBox::set_password(const std::string &pwd) {
    password = pwd;
    key = std::accumulate(pwd.begin(), pwd.end(), 0);
}


void LockBox::set_file(const std::string &fp) {
    std::ifstream f(fp);
    if (f.good()) {
        file_path = fp;
        file_name = get_name();
        return;
    } else {
        std::cout << "Error: unable to open file please try again" << std::endl;
        exit(-1);
    }

    
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


void LockBox::show_help() {
    std::cout << "How to use LockBox:\n";
    std::cout << "Format = LockBox [options]\n";
    std::cout << "Options:\n-f 'path to file'\n-e to encrypt a file\n-d to decrypt a file\n";
    std::cout << "-p 'password' used for encryption/decryption\n";
    std::cout << "If both -e and -d are specified, whichever one comes later will be what is used" << std::endl;
}


std::string LockBox::get_name() {
    // Isolate the file name from the file path
    size_t index = file_path.find_last_of('/');
    // If no / then the file path is just the file name
    if (index == std::string::npos) {
        return file_path;
    } else {
        return file_path.substr(++index);
    }
}


void LockBox::encrypt() {
    // Open the 3 files we will need, the file to be encrypted, the file to write that to,
    // and the pwd file in the data folder to store the hashed password and file name associated to it
    std::fstream fin(file_path, std::fstream::in), fout("./LockBox/files/e_" + file_name, std::fstream::out), 
        pwd("./LockBox/data/pwd.txt", std::fstream::app);

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
    pwd << "e_" + file_name << ":" << hashed(password) << "\n";
    pwd.close();

    // End message
    std::cout << "Congrats your file has been encrypted and can be found in LockBox/files" << std::endl;
}


void LockBox::decrypt() {
    // First verify that the file-password matches an entry in the data file
    std::fstream pwd("./LockBox/data/pwd.txt", std::fstream::in);
    // Vector to store all the lines of the file to re-write them to the password to remove current entry
    std::vector<std::string> data_file;
    // Hash function to get the hashed version of the password which is what is actaully stored
    std::hash<std::string> hashed;
    // Flag for if the file-password exists
    bool exists = false;
    // String to hold the current line of the file
    std::string line;
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
    // Close pwd
    pwd.close();

    // Now re-open it to overwrite all of the contents to remove the file-password entry
    std::fstream pwd1("./LockBox/data/pwd.txt", std::fstream::out);
    for (auto l: data_file)
        pwd1 << l;

    // Close it again
    pwd1.close();

    // After verifying the password, the encrypted file and the new file it will be written to need to be opened.
    std::fstream fin(file_path, std::fstream::in), fout("./LockBox/files/" + file_name.substr(2), std::fstream::out);
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

    // Remove the encrypted file
    fs::remove(file_path);

    // End message
    std::cout << "Congrats, your file has been decrypted and can be found in LockBox/files" << std::endl;
}