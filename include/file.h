//
// Created by dudu233 on 2024/6/17.
//

#ifndef FILESYSTEM_FILE_H
#define FILESYSTEM_FILE_H

#include "chrono"
#include "ctime"
#include "string"
using namespace std;

class file {
private:
    string name;
    // TODO 位置
    int size;
    // TODO 所属用户
    char masterPrivilege;                       // 所有者权限
    char otherPrivilege;                        // 其他人权限
    chrono::system_clock::time_point createTime;// 创建时间
    chrono::system_clock::time_point modifyTime;// 修改时间
    long long point;                            // 保存在数据文件中的位置

private:
    bool updateTime();// 更新文件修改时间

public:
    file();
    ~file();
};


#endif//FILESYSTEM_FILE_H
