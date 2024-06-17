//
// Created by dudu233 on 2024/6/17.
//

#include "directory.h"

// 根目录
directory dir_root("", nullptr, user_root);

directory::directory(string name, directory *father, user master) {
    this->name = name;
    this->master = master;
    this->masterPrivilege = 7;
    this->otherPrivilege = 4;
    this->createTime = chrono::system_clock::now();
    this->modifyTime = chrono::system_clock::now();
    this->father = father;
}

// 获取目录名
string directory::getName() {
    return name;
}

// 获取所属用户
user directory::getUser() {
    return master;
}

// 获取所有子目录
vector<directory> directory::getDirectories() {
    return directories;
}

// 获取所有文件
vector<file> directory::getFiles() {
    return files;
}
// 获取所有者权限
char directory::getMasterPrivilege() {
    return masterPrivilege;
}

// 获取其他人权限
char directory::getOtherPrivilege() {
    return otherPrivilege;
}

// 检查所有者权限
char directory::checkMasterPrivilege(char masterPrivilege) {
    return this->masterPrivilege == masterPrivilege;
}

// 检查其他人权限
char directory::checkOtherPrivilege(char otherPrivilege) {
    return this->otherPrivilege == otherPrivilege;
}


directory::~directory() {}
