//
// Created by dudu233 on 2024/6/17.
//

#ifndef FILESYSTEM_DIRECTORY_H
#define FILESYSTEM_DIRECTORY_H

#include "chrono"
#include "ctime"
#include "file.h"
#include "string"
#include "user.h"
#include "vector"

using std::vector;

class directory {
private:
    string name;                                // 目录名
    user master = user_root;                    // 所属用户
    char masterPrivilege;                       // 所有者权限
    char otherPrivilege;                        // 其他人权限
    chrono::system_clock::time_point createTime;// 创建时间
    chrono::system_clock::time_point modifyTime;// 修改时间
    directory *father;                          // 父目录
    vector<directory> directories;              // 子目录
    vector<file> files;                         // 目录下的文件

public:
    directory();
    directory(string name, directory *father, user master);
    directory(const directory &dir);
    ~directory();
    string getName();                               // 获取目录名
    user getUser();                                 // 获取所属用户
    vector<directory> getDirectories();             // 获取所有子目录
    vector<file> getFiles();                        // 获取所有文件
    bool addFile(file file);                        // 新增文件
    bool addDirectory(directory dir);               // 新增目录
    char getMasterPrivilege();                      // 获取所有者权限
    char getOtherPrivilege();                       // 获取其他用户权限
    bool checkMasterPrivilege(char masterPrivilege);// 检查所有者权限
    bool checkOtherPrivilege(char otherPrivilege);  // 检查其他用户权限

    void serialize(fstream &out) const;
    void deserialize(fstream &in);
};

// 根目录
extern directory dir_root;

#endif//FILESYSTEM_DIRECTORY_H
