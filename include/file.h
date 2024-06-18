//
// Created by dudu233 on 2024/6/17.
//

#ifndef FILESYSTEM_FILE_H
#define FILESYSTEM_FILE_H

#include "chrono"
#include "ctime"
#include "fstream"
#include "iostream"
#include "string"
#include "user.h"

namespace chrono = std::chrono;
using std::cerr;
using std::cout;
using std::endl;
using std::fstream;
using std::ifstream;
using std::ios;
using std::ofstream;
using std::runtime_error;

class file {
private:
    string name;
    int size;
    user master = user_root;
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
    file(const file &file);
    ~file();


public:
    bool setMasterPrivilege(char masterPrivilege);// 设置文件所有者权限
    bool setOtherPrivilege(char otherPrivilege);  // 设置其他用户的权限
    bool deleteFile();
    ;                                             // 删除文件
    string readFile();                            // 读取的文件内容
    char getMasterPrivilege(char masterPrivilege);// 获取所有者权限
    char getOtherPrivilege(char otherPrivilege);  // 获取其他用户权限
};


#endif//FILESYSTEM_FILE_H
