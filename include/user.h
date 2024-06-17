//
// Created by 86138 on 2024/6/17.
//

#ifndef FILESYSTEM_USER_H
#define FILESYSTEM_USER_H

#include "chrono"
#include "ctime"
#include "string"
using namespace std;

class user {
private:
    string name; // 用户名
    string password; // 用户密码
    bool superuser; // 超级用户
    int uid; // 用户uid

public:
    bool changePassword(string password); // 改密码命令


};


#endif//FILESYSTEM_USER_H
