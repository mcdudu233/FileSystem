//
// Created by dudu233 on 2024/6/17.
//

#ifndef FILESYSTEM_FILESYSTEM_H
#define FILESYSTEM_FILESYSTEM_H

#include "data.h"

class filesystem {
private:
    string name;   // 文件系统数据的文件名
    int space_size;// 空间大小
    int block_size;// 块大小

public:
    filesystem(string name, int space, int block);
    ~filesystem();
};


#endif//FILESYSTEM_FILESYSTEM_H
