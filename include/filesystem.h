//
// Created by dudu233 on 2024/6/17.
//

#ifndef FILESYSTEM_FILESYSTEM_H
#define FILESYSTEM_FILESYSTEM_H

#include "data.h"
#include "directory.h"
#include "file.h"
#include "user.h"

class filesystem {
public:
    string name;   // 文件系统数据的文件名
    int space_size;// 空间大小
    int block_size;// 块大小

    user master = user_root;  // 根用户
    directory tree = dir_root;// 根目录

public:
    filesystem();
    filesystem(const string &name, int space, int block);
    ~filesystem();
    void serialize(fstream &out) const;
    void deserialize(fstream &in);
};

// 外部可调用的文件系统变量
extern filesystem *newfs;
filesystem *startFileSystem(const string &name, int space, int block);
void closeFileSystem();

#endif//FILESYSTEM_FILESYSTEM_H
