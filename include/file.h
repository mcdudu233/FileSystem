//
// Created by dudu233 on 2024/6/17.
//

#ifndef FILESYSTEM_FILE_H
#define FILESYSTEM_FILE_H

#include "chrono"
#include "ctime"
#include "string"
#include "fstream"
#include "iostream"
using namespace std;

class file {
private:
    string name;
    // TODO 位置
    int size;
    // TODO 所属用户
    char masterPrivilege;
    char otherPrivilege;
    chrono::system_clock::time_point createTime;
    chrono::system_clock::time_point modifyTime;
    long long point;   // 保存在数据文件中的位置
    fstream fileHandle;// 声明文件句柄

private:
    bool updateTime();// 更新文件修改时间

public:
    file(string &fileName);
    ~file();

public:
    bool setMasterPrivilege (char masterPrivilege); // 设置文件所有者权限
    bool setOtherPrivilege (char otherPrivilege); // 设置其他用户权限
};


#endif//FILESYSTEM_FILE_H
