//
// Created by dudu233 on 2024/6/17.
//

#ifndef FILESYSTEM_FILE_H
#define FILESYSTEM_FILE_H

#include "chrono"
#include "ctime"
#include "string"
using namespace std;

typedef struct {
    string name;
    // TODO 位置
    int size;
    // TODO 所属用户
    char masterPrivilege;
    char otherPrivilege;
    std::chrono::system_clock::time_point createTime;
    std::chrono::system_clock::time_point modifyTime;
} FCB;

class file {
private:
    FCB fcb;

private:
    bool updateTime();// 更新文件修改时间

public:
    file();
    ~file();
};


#endif//FILESYSTEM_FILE_H
