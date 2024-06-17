//
// Created by dudu233 on 2024/6/17.
//

#include "data.h"

fstream file;

// 块的大小 单位：字节
int block_size = 512;

bool initData() {
    ofstream tmp(DATA_PATH);
    if (tmp.is_open()) {
        tmp.close();
    } else {
        return false;
    }

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
bool setPosition(long long block) {
    // 指针从头位置开始偏移
    file.seekg(block * block_size, ios::beg);
    file.seekg(block * block_size, ios::beg);
    return true;
}

char *read(long long size) {
    char *readBuf = new char[size];
    file.read(readBuf, size);
    return readBuf;
}

char *read(long long block, long long size) {
    setPosition(block);
    return read(size);
}

bool write(char *data) {
    return file.write(data, sizeof(data)).good();
}

bool write(long long block, char *data) {
    setPosition(block);
    return write(data);
}