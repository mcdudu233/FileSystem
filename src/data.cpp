//
// Created by dudu233 on 2024/6/17.
//

#include "data.h"

fstream file;

bool initData() {
    file.open(DATA_PATH, ios::in | ios::out | ios::binary);
    if (file.is_open()) {
        return true;
    } else {
        return false;
    }
}

bool closeData() {
    file.close();
    return true;
}

// 设置数据指向的位置
bool setPosition(long long d) {
    // 指针从头位置开始偏移
    file.seekg(d, ios::beg);
    file.seekg(d, ios::beg);
    return true;
}
