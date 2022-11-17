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


void LockBox::set_face_flag(bool value) {
    facial_flag = value;
}


void LockBox::set_face() {
    // Start with instructions
    std::cout << "To start facial recognition scan, first make sure you are in view and looking at your webcam.\n";
    std::cout << "The scanning process should take about 5-10 seconds, When you are ready press enter to continue" << std::endl;
    std::cin.get();
    std::fflush(stdin);
    
    // Open the camera object and make sure it is open
    cv::VideoCapture camera(0);
    if (!camera.isOpened()) {
        std::cout << "Error: unable to access webcam" << std::endl;
        exit(1);
    }

    // Create a directory to hold the images based on the name of the file being encrypted to avoid duplicates
    std::string dir("./LockBox/data/e_" + file_name);
    if (!fs::exists(dir))
        fs::create_directories(dir);

    const std::string name = "/e_" + file_name.substr(0, file_name.find_first_of('.')) + "_";
    int count = 1;
    cv::Mat frame, frame_gray;
    while (count < 6) {
        // Save frame from the camera
        camera >> frame;
        // If there is a face it will add it to the faces vector
        if (check_face(frame)) {
            // Convert file to grayscale for storage
            cv::cvtColor(frame, frame_gray, 6);
            // Save the file in the folder we just created
            cv::imwrite(dir + name + std::to_string(count++) + ".pgm", frame_gray);
        } else {
            // If there wasn't a face in the rame, output a warning message and try again
            std::cout << "Warning: please make sure you are in view of and looking at your webcam" << std::endl;
            cv::waitKey(1);
        }
    }

    // Finally, set the facial_flag value to true
    facial_flag = true;

}


void LockBox::predict_face() {
    // Open the password file
    std::fstream pwd("./LockBox/data/pwd.txt", std::fstream::in);
    bool exists;
    // First check if this file was locked using facial recognition
    std::string line;
    while (getline(pwd, line)) {
        size_t first = line.find(':');
        size_t last = line.find_last_of(':');
        if (line.substr(0, first + 1) == file_name) {
            // The file exists now check if it was locked using facial too
            exists = true;
            if (first == last || line.substr(last+1) != "face") {
                std::cout << "Error: File was not locked using facial recognition" << std::endl;
                exit(1);
            }
        }
    }

    // Now start the facial recognition by loading the pictures into a vector using a directory iterator
    std::vector<cv::Mat> training;
    std::vector<int> labels;
    const fs::path pictures("./LockBox/data/" + file_name );
    for (const auto &directory : fs::directory_iterator(pictures)) {
        training.push_back(cv::imread(directory.path(), 0));
        labels.push_back(1);
    }
    

    // Take the picture to verify
    cv::VideoCapture camera(0);
    cv::Mat image;
    while (1) {
        camera >> image;
        if (!check_face(image)) {
            std::cout << "Warning: please make sure you are in view of and looking at your webcam" << std::endl;
            cv::waitKey(1);
        } else
            break;
    }

    // After verifying the image, write it to a temp file and re-read it in, then remove the temp file
    // The image needs to be normalized and this is the easiest way of doing it
    const std::string temp("temp123456789.pgm");
    cv::cvtColor(image, image, 6);
    cv::imwrite(temp, image);
    image = cv::imread(temp, 0);
    fs::remove(temp);


    // Now predict the label of that image
    cv::Ptr<cv::face::EigenFaceRecognizer> model = cv::face::EigenFaceRecognizer::create();
    model->train(training, labels);
    int predict = model->predict(image);

    if (predict != 1) {
        std::cout << "Error: Facial recognition failed" << std::endl;
        exit(1);
    }
}


bool LockBox::check_face(cv::Mat &image) {
     // Create a cascadeclassifier to detect faces. This will be used to make sure there is a face in each from from the camera
    cv::CascadeClassifier fd;
    // Load the haarcascade for detecting faces
    fd.load("./haarcascades/haarcascade_frontalface_default.xml");
    // detectMultiScale, the method that will be used to detect faces, requires a vector to store the faces in
    std::vector<cv::Rect> faces;
    // Call multiscale to detect if there is a face in the frame
    fd.detectMultiScale(image, faces, 1.2, 3);
    if (!faces.empty())
        return true;
    else
        return false;
}


void LockBox::encrypt() {
    // First check if this was locked using facial recognition as well
    if (facial_flag)
        set_face();
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
    pwd << "e_" + file_name << ":" << hashed(password);
    // Append facial to the end of the entry if facial recognition is also set for this file
    if (facial_flag) {
        pwd << ":face\n";
    } else {
        pwd << '\n';
    }
    pwd.close();

    // End message
    std::cout << "Congrats your file has been encrypted and can be found in LockBox/files" << std::endl;
}


void LockBox::decrypt() {
    // First check if the facial flag is set
    if (facial_flag)
        predict_face();
    // Verify that the file-password matches an entry in the data file
    std::fstream pwd("./LockBox/data/pwd.txt", std::fstream::in);
    // Vector to store all the lines of the file to re-write them to the password to remove current entry
    std::vector<std::string> data_file;
    // Hash function to get the hashed version of the password which is what is actaully stored
    std::hash<std::string> hash;
    // Flag for if the file-password exists
    bool exists = false;
    // String to hold the current line of the file
    std::string line, hashed_password, locked_file_name;
    while (getline(pwd, line)) {
        // Get the name of the locked file on this line
        locked_file_name = line.substr(0, file_name.size());
        if (facial_flag) {
            // Get the hashed password on this line
            hashed_password = line.substr(file_name.size() + 1, line.find_last_of(':') - file_name.size() - 1);
            // Check if both match the password and the file name
            if (hashed_password == std::to_string(hash(password)) && locked_file_name == file_name) {
                exists = true;
                continue;
            }
        } else {
            // If the facial_flag is false, the password can be taken from file_name.size() to the end of the line
            hashed_password = line.substr(file_name.size() + 1);
            if (hashed_password == std::to_string(hash(password)) && locked_file_name == file_name) {
                exists = true;
                continue;
            }
        }
        // If the line didn't match, push it into a vector to be added back to the file later
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