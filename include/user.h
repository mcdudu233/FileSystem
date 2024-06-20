//
// Created by 86138 on 2024/6/17.
//

#ifndef FILESYSTEM_USER_H
#define FILESYSTEM_USER_H

#include "chrono"
#include "ctime"
#include "fstream"
#include "string"

using std::fstream;
using std::string;

class user {
private:
    int uid;        // *关键字：用户uid
    string name;    // 用户名
    string password;// 用户密码 采用MD5加密
    bool superuser; // 是否是超级用户

public:
    user();
    user(string name, string password = "", bool superuser = false);
    ~user();
    int getUid();
    bool changePassword(string password);// 改密码命令
    void serialize(fstream &out) const;  // 序列化
    void deserialize(fstream &in);       // 反序列化
};

// 默认用户
extern user user_root;

#endif//FILESYSTEM_USER_H
