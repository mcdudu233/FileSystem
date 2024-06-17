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