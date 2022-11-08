#ifndef LOCKBOX
#define LOCKBOX

#define OPENCV "opencv4/opencv2"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <vector>
#include <string>
#include <functional>
#include <numeric>
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <OPENCV/imgproc.hpp>


namespace fs = std::filesystem;

class LockBox {
    private:
        std::string file_path;
        std::string file_name;
        std::string password;
        int key;
        void set_password(const std::string&);
        void set_file(const std::string &);
        std::string get_name(void);
        void initialize(void);
    
    public:
        LockBox(const std::string&, const std::string&);
        void encrypt();
        void decrypt();
        static void show_help();
};



#endif // LOCKBOX_H