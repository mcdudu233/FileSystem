//
// Created by dudu233 on 2024/6/17.
//

#include "file.h"

file::file() {
    this->createTime = chrono::system_clock::now();
    updateTime();
}

bool file::updateTime() {
    this->modifyTime = chrono::system_clock::now();
    return true;
}