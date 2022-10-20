#include "LockBox.h"

LockBox::LockBox(char *fp) {
    file_path = fp;
    initialize();
}


void LockBox::initialize() {
    fs::path p = p.root_name().append("/LockBox");
    fs::create_directories(p);
    fs::create_directories(p.append("/LockBox/files"));
    fs::create_directories(p.append("/LockBox/data"));
}


