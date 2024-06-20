//
// Created by dudu233 on 2024/6/17.
//

#include "data.h"

#include <utility>

fstream file;// 数据文件

vector<bool> *available;           // 空闲盘块 位视图法
string data_path;                  // 文件系统数据路径
int block_size = 512;              // 块的大小 单位：字节
int space_size = 128 * 1024 * 1024;// 文件系统的容量 单位：字节

bool existData(const string &name) {
    data_path = DATA_PATH + name + DATA_SUFFIX;
    if (fs::exists(data_path)) {
        return true;
    }
    return false;
}

bool initData(const string &name) {
    data_path = DATA_PATH + name + DATA_SUFFIX;

    // 文件不存在则初始化一个
    if (!existData(name)) {
        ofstream tmp(data_path, ios::out | ios::binary);
        if (tmp.is_open()) {
            if (!tmp.write(new char[space_size]{0}, space_size).good()) {
                cerr << "Write new system data fail!" << endl;
                return false;
            }
            tmp.close();
        } else {
            cerr << "Create new system data fail!" << endl;
            return false;
        }
    }

    file.open(data_path, ios::in | ios::out | ios::binary);
    if (file.is_open()) {
        return true;
    } else {
        cerr << "Open system data fail!" << endl;
        return false;
    }
}

bool closeData() {
    file.close();
    return true;
}

bool setAvailable(vector<bool> *v) {
    available = v;
    return true;
}

int getBlockSize() {
    return block_size;
}

bool setBlockSize(int size) {
    block_size = size;
    return true;
}

int getSpaceSize() {
    return space_size;
}

bool setSpaceSize(int size) {
    space_size = size;
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

fstream *getData() {
    return &file;
}