//
// Created by 86138 on 2024/6/17.
//

#ifndef FILESYSTEM_USER_H
#define FILESYSTEM_USER_H

#include "chrono"
#include "cstdlib"
#include "cstring"
#include "ctime"
#include "fstream"
#include "iostream"
#include "sstream"
#include "string"

using std::fstream;
using std::string;

class user {
private:
    int uid;        // *关键字：用户uid
    string name;    // 用户名
    string password;// 用户密码 采用MD5加密
    bool superuser; // 是否是超级用户

private:
public:
    user();
    user(string name, string password = "", bool superuser = false);
    ~user();
    int getUid();                       // 获取UID
    string getName();                   // 获取用户名
    bool setName(string name);          // 设置用户名
    string getPassword();               // 获取用户密码MD5
    bool setPassword(string password);  // 设置用户密码
    bool checkPassword(string password);// 检测密码是否一致
    bool getSuper();                    // 判断是否为超级用户
    bool setSuper(bool super);          // 设置是否为超级用户
    void serialize(fstream &out) const; // 序列化
    void deserialize(fstream &in);      // 反序列化
};

// 默认用户
extern user user_root;

#endif//FILESYSTEM_USER_H
