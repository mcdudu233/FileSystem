//
// Created by dudu233 on 2024/6/17.
//

#include "directory.h"

// 根目录
directory dir_root("", nullptr, user_root);

directory::directory(string name, directory *father, user master) {
    this->name = name;
    this->master = master;
    this->masterPrivilege = 7; // 文件所有者有全部权限
    this->otherPrivilege = 1; //其他用户仅有读取权限
    this->createTime = chrono::system_clock::now();
    this->modifyTime = chrono::system_clock::now();
    this->father = father;
}
bool directory::addFile(file file) {
    this->files.push_back(file);
    modifyTime = chrono::system_clock::now(); // 更新修改时间
    return true; // 假设文件添加总是成功
}
bool directory::addDirectory(directory dir) {
    this->directories.push_back(dir);
    modifyTime = chrono::system_clock::now(); // 更新修改时间
    return true; // 假设目录添加总是成功
}
// 获取目录名
string directory::getName() {
    return this->name;
}

// 获取所属用户
user directory::getUser() {
    return this->master;
}

// 获取所有者权限
char directory::getMasterPrivilege() {
    return this->masterPrivilege;
}

// 获取其他人权限
char directory::getOtherPrivilege() {
    return this->otherPrivilege;
}

// 检查所有者权限
bool directory::checkMasterPrivilege(char masterPrivilege) {
    return this->masterPrivilege == masterPrivilege;
}

// 检查其他人权限
bool directory::checkOtherPrivilege(char otherPrivilege) {
    return this->otherPrivilege == otherPrivilege;
}


directory::~directory() {
// 释放资源
}
