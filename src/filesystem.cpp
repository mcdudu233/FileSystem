//
// Created by dudu233 on 2024/6/17.
//

#include "filesystem.h"

filesystem::filesystem(string name, int space, int block) {
    this->name = name;
    this->space_size = space;
    this->block_size = block;

    setBlockSize(block);
    // 初始化文件系统数据
    if (!initData(name)) {
        cerr << "Fail to init data." << endl;
    }
}

filesystem::~filesystem() {
    closeData();
}
