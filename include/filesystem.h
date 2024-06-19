//
// Created by dudu233 on 2024/6/17.
//

#ifndef FILESYSTEM_FILESYSTEM_H
#define FILESYSTEM_FILESYSTEM_H

#include "data.h"
#include "directory.h"
#include "file.h"
#include "user.h"

// 返回的目录和文件列表
typedef struct {
    string name;
} List;

class filesystem {
private:
    string path;   // 当前所在的路径
    string name;   // 文件系统数据的文件名
    int space_size;// 空间大小
    int block_size;// 块大小

    vector<user> users;// 所有用户
    directory tree;    // 根目录

public:
    filesystem(const string &name, int space, int block);
    ~filesystem();
    void serialize(fstream &out) const;
    void deserialize(fstream &in);

public:
    vector<List> ls();           // 列出当前文件夹下的文件
    vector<List> ls(string path);// 列出某个文件夹下的文件
};

#endif//FILESYSTEM_FILESYSTEM_H
