#ifndef LOCKBOX_H
#define LOCKBOX_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <vector>
#include <string>
#include <functional>
#include <numeric>
#include <opencv4/opencv2/opencv.hpp>


namespace fs = std::filesystem;

class LockBox {
    private:
        std::string file_path;
        std::string file_name;
        std::string password;
        int key;
        bool facial_flag;
        void set_password(const std::string&);
        void set_file(const std::string &);
        std::string get_name(void);
        void initialize(void);
    
    public:
        LockBox(const std::string&, const std::string&);
        void encrypt();
        void decrypt();
        void set_face();
        bool check_face();
        static void show_help();
};



#endif // LOCKBOX_H