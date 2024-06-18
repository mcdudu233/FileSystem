//
// Created by dudu233 on 2024/6/17.
//

#include "filesystem.h"

filesystem *newfs;

filesystem *startFileSystem(const string &name, int space, int block) {
    newfs = new filesystem(name, space, block);
    setSpaceSize(space);
    setBlockSize(block);
    // 初始化文件系统数据
    if (existData(name)) {
        if (!initData(name)) {
            cerr << "Fail to init data." << endl;
        }
        // 然后加载文件系统数据
        getData()->read(reinterpret_cast<char *>(newfs), sizeof(filesystem));
        cerr << newfs->name << endl;
        cerr << newfs->space_size << endl;
    } else {
        if (!initData(name)) {
            cerr << "Fail to init data." << endl;
        }
    }
    return newfs;
}

void closeFileSystem() {
    delete newfs;
}

filesystem::filesystem() = default;

filesystem::filesystem(const string &name, int space, int block) {
    this->name = name;
    this->space_size = space;
    this->block_size = block;
}

filesystem::~filesystem() {
    // 保存文件系统数据 从第0块数据写
    getData()->write(reinterpret_cast<char *>(this), sizeof(filesystem));
    closeData();
}
